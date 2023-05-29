/*
 * Copyright © 2011 Benjamin Franzke
 * Copyright © 2010 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>
#include <vector>

#include <wayland-client.h>
#include <wayland-egl.h>
#include <cairo/cairo.h>
#include <png.h>
#include <iostream>

#include "xdg-shell-client-protocol.h"
#include "os-compatibility.h"
#include "zalloc.h"

using namespace std;

struct display
{
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_shell *shell;
    struct wl_shm *shm;
    struct xdg_wm_base *xdg_shell;
    bool has_xrgb;
};

struct buffer
{
    struct wl_buffer *buffer;
    void *shm_data;
    int busy;
};

struct window
{
    struct display *display;
    int width, height;
    struct wl_surface *surface;
    struct wl_shell_surface *shell_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct buffer buffers[2];
    struct buffer *prev_buffer;
    struct wl_callback *callback;
};

static int running = 1;

const int rect_x = 0;
const int rect_y = 0;
const int rect_width = 100;
const int rect_height = 100;

static void
redraw(void *data, struct wl_callback *callback, uint32_t time);

static void
buffer_release(void *data, struct wl_buffer *buffer)
{
    struct buffer *mybuf = (struct buffer *)data;

    mybuf->busy = 0;
}

static const struct wl_buffer_listener buffer_listener = {
    buffer_release};

static int
create_shm_buffer(struct display *display, struct buffer *buffer,
                  int width, int height, uint32_t format)
{
    struct wl_shm_pool *pool;
    int fd, size, stride;
    void *data;

    stride = width * 4;
    size = stride * height;

    fd = os_create_anonymous_file(size);
    if (fd < 0)
    {
        fprintf(stderr, "creating a buffer file for %d B failed: %s\n",
                size, strerror(errno));
        return -1;
    }

    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    pool = wl_shm_create_pool(display->shm, fd, size);
    buffer->buffer = wl_shm_pool_create_buffer(pool, 0,
                                               width, height,
                                               stride, format);
    wl_buffer_add_listener(buffer->buffer, &buffer_listener, buffer);
    wl_shm_pool_destroy(pool);
    close(fd);

    buffer->shm_data = data;

    return 0;
}

static void handle_configure(void *data, struct xdg_surface *surface, uint32_t serial)
{
    xdg_surface_ack_configure(surface, serial);
}

static const struct xdg_surface_listener surface_listener = {
    .configure = handle_configure};

static void
handle_ping(void *data, struct xdg_wm_base *xdg_surface,
            uint32_t serial)
{
    xdg_wm_base_pong(xdg_surface, serial);
}

static const struct xdg_wm_base_listener xdg_surface_listener = {
    handle_ping};

static struct window *
create_window(struct display *display, int width, int height)
{
    struct window *window;
    struct wl_region *region;

    window = (struct window *)zalloc(sizeof *window);
    if (!window)
        return NULL;

    window->callback = NULL;
    window->display = display;
    window->width = width;
    window->height = height;
    window->surface = wl_compositor_create_surface(display->compositor);
    window->xdg_surface = xdg_wm_base_get_xdg_surface(display->xdg_shell,
                                                      window->surface);
    if (window->xdg_surface)
        xdg_surface_add_listener(window->xdg_surface, &surface_listener, NULL);

    if (display->xdg_shell)
        xdg_wm_base_add_listener(display->xdg_shell, &xdg_surface_listener, NULL);

    window->xdg_toplevel = xdg_surface_get_toplevel(window->xdg_surface);

    xdg_toplevel_set_parent(window->xdg_toplevel, NULL);
    xdg_toplevel_set_maximized(window->xdg_toplevel);

    region = wl_compositor_create_region(display->compositor);
    wl_region_add(region, 0, 0, 0, 0);
    wl_surface_set_input_region(window->surface, region);
    wl_region_destroy(region);

    return window;
}

static void
destroy_window(struct window *window)
{
    if (window->callback)
        wl_callback_destroy(window->callback);

    if (window->buffers[0].buffer)
        wl_buffer_destroy(window->buffers[0].buffer);
    if (window->buffers[1].buffer)
        wl_buffer_destroy(window->buffers[1].buffer);

    xdg_surface_destroy(window->xdg_surface);
    wl_shell_surface_destroy(window->shell_surface);
    wl_surface_destroy(window->surface);
    free(window);
}

static struct buffer *
window_next_buffer(struct window *window)
{
    struct buffer *buffer;
    int ret = 0;

    if (!window->buffers[0].busy)
        buffer = &window->buffers[0];
    else if (!window->buffers[1].busy)
        buffer = &window->buffers[1];
    else
        return NULL;

    if (!buffer->buffer)
    {
        ret = create_shm_buffer(window->display, buffer,
                                window->width, window->height,
                                WL_SHM_FORMAT_ARGB8888);

        if (ret < 0)
            return NULL;

        /* paint the padding */
        memset(buffer->shm_data, 0xff, window->width * window->height * 4);
    }

    return buffer;
}

static void
paint_pixels(void *image, int width, int height, uint32_t time)
{
    uint32_t *pixel = (uint32_t *)image;
    uint32_t *pSrc = (uint32_t *)image;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            *pixel++ = 0x00000000;
        }
    }
    pixel = pSrc;

    // 打开PNG文件
    //FILE *file = fopen("/home/zwh/Desktop/PrintWatermarkText.png", "rb");
    FILE *file = fopen("/home/zwh/Desktop/test.png", "rb");
    if (!file)
    {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    // 创建PNG结构体和信息结构体
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        std::cerr << "Failed to create PNG read structure" << std::endl;
        fclose(file);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        std::cerr << "Failed to create PNG info structure" << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(file);
        return;
    }

    // 设置PNG错误处理
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        std::cerr << "Failed to set PNG error handler" << std::endl;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(file);
        return;
    }

    // 初始化PNG读取
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 0);

    // 读取PNG信息
    png_read_info(png_ptr, info_ptr);

    // 获取PNG图像属性
    int Pngwidth = png_get_image_width(png_ptr, info_ptr);
    int Pngheight = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // 确定要读取的像素格式
    png_bytep row_pointers[Pngheight];
    int pixel_size = 4;
    if (color_type == PNG_COLOR_TYPE_RGB)
    {
        pixel_size = 3;
    }
    else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
        bit_depth = 8;
    }

    // 为每一行分配内存
    for (int i = 0; i < Pngheight; i++)
    {
        row_pointers[i] = new png_byte[Pngwidth * pixel_size];
    }

    // 读取PNG像素数据
    png_read_image(png_ptr, row_pointers);

    // 输出每个像素的RGBA值
    for (int y = 0; y < Pngheight && y < height; y++)
    {
        for (int x = 0; x < Pngwidth && x < width; x++)
        {
            png_byte *pixelData = &(row_pointers[y][x * pixel_size]);
            *(pixel + x) = ((uint32_t)pixelData[3] << 24) | ((uint32_t)pixelData[0] << 16) | ((uint32_t)pixelData[1] << 8) | (uint32_t)pixelData[2];
        }
        pixel += width;
    }

    // 释放内存和关闭文件
    for (int i = 0; i < Pngheight; i++)
    {
        delete[] row_pointers[i];
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);

    //    pixel = pSrc + rect_y*width+rect_x;
    //    for(int h = 0; h < rect_height; ++h)
    //    {
    //        for(int w = 0; w < rect_width; ++w)
    //        {
    //            *(pixel + w) = 0xff << 24 | 0xff << 16 | 0x00 << 8 | 0xff;
    //        }

    //        pixel += width;
    //    }
}

// static struct wl_callback_listener frame_listener;

static struct wl_callback_listener frame_listener = {
    redraw};

static void
redraw(void *data, struct wl_callback *callback, uint32_t time)
{
    window *window = (struct window *)data;
    buffer *buffer;

    buffer = window_next_buffer(window);
    if (!buffer)
    {
        fprintf(stderr,
                !callback ? "Failed to create the first buffer.\n" : "Both buffers busy at redraw(). Server bug?\n");
        abort();
    }

    paint_pixels(buffer->shm_data, window->width, window->height, time);

    wl_surface_attach(window->surface, buffer->buffer, 0, 0);
    wl_surface_damage(window->surface,
                      0, 0, window->width, window->height);

    if (callback)
        wl_callback_destroy(callback);

    xdg_toplevel_set_parent(window->xdg_toplevel, NULL);
    xdg_toplevel_set_maximized(window->xdg_toplevel);

    window->callback = wl_surface_frame(window->surface);

    // wl_callback_add_listener(window->callback, &frame_listener, window);

    wl_surface_commit(window->surface);
    buffer->busy = 1;
}

static void
shm_format(void *data, struct wl_shm *wl_shm, uint32_t format)
{
    struct display *d = (struct display *)data;

    if (format == WL_SHM_FORMAT_XRGB8888)
        d->has_xrgb = true;
}

struct wl_shm_listener shm_listener = {
    shm_format};

static void
registry_handle_global(void *data, struct wl_registry *registry,
                       uint32_t id, const char *interface, uint32_t version)
{
    struct display *d = (struct display *)data;

    if (strcmp(interface, "wl_compositor") == 0)
    {
        d->compositor = (struct wl_compositor *)
            wl_registry_bind(registry,
                             id, &wl_compositor_interface, 1);
    }
    else if (strcmp(interface, "wl_shm") == 0)
    {
        d->shm = (struct wl_shm *)wl_registry_bind(registry,
                                                   id, &wl_shm_interface, 1);
        wl_shm_add_listener(d->shm, &shm_listener, d);
    }
    else if (strcmp(interface, "wl_shell") == 0)
    {
        d->shell = (struct wl_shell *)wl_registry_bind(registry,
                                                       id, &wl_shell_interface, 1);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        d->xdg_shell = (struct xdg_wm_base *)wl_registry_bind(registry, id, &xdg_wm_base_interface, version);
    }
}

static void
registry_handle_global_remove(void *data, struct wl_registry *registry,
                              uint32_t name)
{
}

static const struct wl_registry_listener registry_listener = {
    registry_handle_global,
    registry_handle_global_remove};

static struct display *
create_display(void)
{
    struct display *display;

    display = (struct display *)malloc(sizeof *display);
    if (display == NULL)
    {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    display->display = wl_display_connect(NULL);
    assert(display->display);

    display->has_xrgb = false;
    display->registry = wl_display_get_registry(display->display);
    wl_registry_add_listener(display->registry,
                             &registry_listener, display);
    wl_display_roundtrip(display->display);
    if (display->shm == NULL)
    {
        fprintf(stderr, "No wl_shm global\n");
        exit(1);
    }

    wl_display_roundtrip(display->display);

    /*
     * Why do we need two roundtrips here?
     *
     * wl_display_get_registry() sends a request to the server, to which
     * the server replies by emitting the wl_registry.global events.
     * The first wl_display_roundtrip() sends wl_display.sync. The server
     * first processes the wl_display.get_registry which includes sending
     * the global events, and then processes the sync. Therefore when the
     * sync (roundtrip) returns, we are guaranteed to have received and
     * processed all the global events.
     *
     * While we are inside the first wl_display_roundtrip(), incoming
     * events are dispatched, which causes registry_handle_global() to
     * be called for each global. One of these globals is wl_shm.
     * registry_handle_global() sends wl_registry.bind request for the
     * wl_shm global. However, wl_registry.bind request is sent after
     * the first wl_display.sync, so the reply to the sync comes before
     * the initial events of the wl_shm object.
     *
     * The initial events that get sent as a reply to binding to wl_shm
     * include wl_shm.format. These tell us which pixel formats are
     * supported, and we need them before we can create buffers. They
     * don't change at runtime, so we receive them as part of init.
     *
     * When the reply to the first sync comes, the server may or may not
     * have sent the initial wl_shm events. Therefore we need the second
     * wl_display_roundtrip() call here.
     *
     * The server processes the wl_registry.bind for wl_shm first, and
     * the second wl_display.sync next. During our second call to
     * wl_display_roundtrip() the initial wl_shm events are received and
     * processed. Finally, when the reply to the second wl_display.sync
     * arrives, it guarantees we have processed all wl_shm initial events.
     *
     * This sequence contains two examples on how wl_display_roundtrip()
     * can be used to guarantee, that all reply events to a request
     * have been received and processed. This is a general Wayland
     * technique.
     */

    if (!display->has_xrgb)
    {
        fprintf(stderr, "WL_SHM_FORMAT_XRGB32 not available\n");
        exit(1);
    }

    return display;
}

static void
destroy_display(struct display *display)
{
    if (display->shm)
        wl_shm_destroy(display->shm);

    if (display->shell)
        wl_shell_destroy(display->shell);

    if (display->compositor)
        wl_compositor_destroy(display->compositor);

    if (display->xdg_shell)
        xdg_wm_base_destroy(display->xdg_shell);

    wl_registry_destroy(display->registry);
    wl_display_flush(display->display);
    wl_display_disconnect(display->display);
    free(display);
}

static void
signal_int(int signum)
{
    running = 0;
}

int main(int argc, char **argv)
{
    struct sigaction sigint;
    struct display *display;
    struct window *window;
    int ret = 0;

    display = create_display();
    window = create_window(display, 1920, 1080);
    if (!window)
        return 1;

    sigint.sa_handler = signal_int;
    sigemptyset(&sigint.sa_mask);
    sigint.sa_flags = SA_RESETHAND;
    sigaction(SIGINT, &sigint, NULL);

    /* Initialise damage to full surface, so the padding gets painted */
    wl_surface_damage(window->surface, 0, 0,
                      window->width, window->height);

    redraw(window, NULL, 0);

    while (running && ret != -1)
        ret = wl_display_dispatch(display->display);

    fprintf(stderr, "simple-shm exiting\n");

    destroy_window(window);
    destroy_display(display);

    return 0;
}
