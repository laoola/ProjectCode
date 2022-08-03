/******************************************************************************
 * Copyright 2020-2022 Co., Ltd.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief       监控鼠标键盘事件
 *
 * @author     zhangwenhao
 * @date       2022/06/01
 * @history
 *****************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>

bool isPress = false;

// Virtual button codes that are not defined by X11.
#define Button1			1
#define Button2			2
#define Button3			3
#define WheelUp			4
#define WheelDown		5
#define WheelLeft		6
#define WheelRight		7
#define XButton1		8
#define XButton2		9

bool filterWheelEvent(int detail)
{
    return detail != WheelUp && detail != WheelDown && detail != WheelLeft && detail != WheelRight;
}

void handleRecordEvent(XRecordInterceptData* data)
{
    if (data->category == XRecordFromServer)
    {
        xEvent * event = (xEvent *)data->data;
        switch (event->u.u.type)
        {
        case ButtonPress:
            if (filterWheelEvent(event->u.u.detail))
            {
                isPress = true;
                printf("%d %d Press\n",
                       event->u.keyButtonPointer.rootX,
                       event->u.keyButtonPointer.rootY);
            }

            break;
        case MotionNotify:
            //if (isPress)
            {
                printf("%d %d notify\n",
                       event->u.keyButtonPointer.rootX,
                       event->u.keyButtonPointer.rootY);
            }

            break;
        case ButtonRelease:
            if (filterWheelEvent(event->u.u.detail))
            {
                isPress = false;
                printf("%d %d release\n",
                       event->u.keyButtonPointer.rootX,
                       event->u.keyButtonPointer.rootY);
            }

            break;
        case KeyPress:
            printf("%d Press\n",((unsigned char*) data->data)[1]);

            break;
        case KeyRelease:
            printf("%d Release\n",((unsigned char*) data->data)[1]);

            break;
        default:
            printf("%d \n", event->u.u.type);
            break;
        }
    }

    fflush(stdout);
    XRecordFreeData(data);
}

void callback(XPointer ptr, XRecordInterceptData* data)
{
    handleRecordEvent(data);
}


void *EventMonitor(void *pParam)
{
    Display* display = XOpenDisplay(0);
    if (display == 0)
    {
        fprintf(stderr, "unable to open display\n");
        return NULL;
    }

    // Receive from ALL clients, including future clients.
    XRecordClientSpec clients = XRecordAllClients;
    XRecordRange* range = XRecordAllocRange();
    if (range == 0)
    {
        fprintf(stderr, "unable to allocate XRecordRange\n");
        return NULL;
    }

    // Receive KeyPress, KeyRelease, ButtonPress, ButtonRelease and MotionNotify events.
    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = KeyPress;
    range->device_events.last  = MotionNotify;

    // And create the XRECORD context.
    XRecordContext context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
    if (context == 0)
    {
        fprintf(stderr, "XRecordCreateContext failed\n");
        return NULL;
    }
    XFree(range);

    XSync(display, True);

    Display* display_datalink = XOpenDisplay(0);
    if (display_datalink == 0)
    {
        fprintf(stderr, "unable to open second display\n");
        return NULL;
    }

    if (!XRecordEnableContext(display_datalink, context,  callback, (XPointer) NULL))
    {
        fprintf(stderr, "XRecordEnableContext() failed\n");
        return NULL;
    }

    return NULL;
}

int main()
{
    pthread_t hEventMonitor;
    void *thread_result = NULL;
    int res = 0;

    pthread_create(&hEventMonitor, NULL, EventMonitor, NULL);

    res = pthread_join(hEventMonitor, &thread_result);
    if (res != 0) {
        printf("1：等待线程失败");
    }
    //输出获取到的 myThread 线程的返回值
    printf("%s\n", (char*)thread_result);
    //尝试再次获取 myThread 线程的返回值
    res = pthread_join(hEventMonitor, &thread_result);
    if (res == ESRCH) {
        printf("2：等待线程失败，线程不存在");
    }

    return 0;
}
