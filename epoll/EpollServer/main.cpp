#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EVENTS 1024
#define BUFFER 4096
#define SERV_PORT 6000

void recvdata(int fd, int events, void *arg);
void senddata(int fd, int events, void *arg);

struct myevent_s
{
    int fd;
    int events;
    void *arg;
    void (*call_back)(int fd, int events, void *arg);
    int status;
    char buf[BUFFER];
    int len;
    long last_active;
};

int g_efd;
struct myevent_s g_events[MAX_EVENTS+1];

void eventset(struct myevent_s *ev, int fd, void (*call_back)(int fd, int events, void *arg), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;

    if(ev->len <= 0)
    {
        memset(ev->buf, 0, sizeof(ev->buf));
        ev->len = 0;
    }

    ev->last_active = time(NULL);

    return;
}

void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    int op = 0;
    epv.data.ptr = ev;
    epv.events = ev->events = events;

    if(ev->status == 0)
    {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }

    if(epoll_ctl(efd, op, ev->fd, &epv)<0)
    {
        printf("event add failed [fd=%d],events[%d]\n", ev->fd, events);
    }
    else
    {
        printf("event add OK [fd=%d],events[%0X]\n", ev->fd, events);
    }

    return ;
}

void eventdel(int efd,struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    if(ev->status != 1) //如果fd没有添加到监听树上，就不用删除，直接返回
        return;
    epv.data.ptr = NULL;
    ev->status = 0;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);
    return;
}

void AcceptConnect(int localFd,int events,void *arg)
{
    struct sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int clientFd, i;
    if((clientFd = accept(localFd, (struct sockaddr *)&cin, &len)) == -1)
    {
        if(errno != EAGAIN && errno != EINTR)
        {
            sleep(1);
        }
        printf("%s:accept,%s\n",__func__, strerror(errno));
        return;
    }
    do
    {
        for(i = 0; i < MAX_EVENTS; i++) //从全局数组g_events中找一个空闲元素，类似于select中找值为-1的元素
        {
            if(g_events[i].status ==0)
                break;
        }
        if(i == MAX_EVENTS) // 超出连接数上限
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break;
        }
        int flag = 0;
        if((flag = fcntl(clientFd, F_SETFL, O_NONBLOCK)) < 0) //将cfd也设置为非阻塞
        {
            printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
            break;
        }
        eventset(&g_events[i], clientFd, recvdata, &g_events[i]); //找到合适的节点之后，将其添加到监听树中，并监听读事件
        eventadd(g_efd, EPOLLIN, &g_events[i]);
    }while(0);

    printf("******new connect[%s:%d],[time:%ld],pos[%d]******\n",inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);

    return;
}

void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = recv(fd, ev->buf, sizeof(ev->buf), 0);    //读取客户端发过来的数据

    eventdel(g_efd, ev);                            //将该节点从红黑树上摘除

    if (len > 0)
    {
        ev->len = len;
        ev->buf[len] = '\0';                        //手动添加字符串结束标记
        printf("revc date[%d]:\n%s\n", fd, ev->buf);

        eventset(ev, fd, senddata, ev);             //设置该fd对应的回调函数为senddata
        eventadd(g_efd, EPOLLOUT, ev);              //将fd加入红黑树g_efd中,监听其写事件

    }
    else if (len == 0)
    {
        close(ev->fd);
        /* ev-g_events 地址相减得到偏移元素位置 */
        printf("[fd=%d] pos[%ld], closed\n", fd, ev-g_events);
    }
    else
    {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }
    return;
}

void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = send(fd, ev->buf, ev->len, 0);    //直接将数据回射给客户端

    eventdel(g_efd, ev);                    //从红黑树g_efd中移除

    if (len > 0)
    {
        printf("send[fd=%d], [%d]\n%s\n", fd, len, ev->buf);
        eventset(ev, fd, recvdata, ev);     //将该fd的回调函数改为recvdata
        eventadd(g_efd, EPOLLIN, ev);       //重新添加到红黑树上，设为监听读事件
    }
    else
    {
        close(ev->fd);                      //关闭链接
        printf("send[fd=%d] error %s\n", fd, strerror(errno));
    }
    return ;
}

void initlistensocket(int efd, short port)
{
    struct sockaddr_in sin;

    int sockAccept = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sockAccept, F_SETFL, O_NONBLOCK);                //将socket设为非阻塞

    memset(&sin, 0, sizeof(sin));               //bzero(&sin, sizeof(sin))
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    bind(sockAccept, (struct sockaddr *)&sin, sizeof(sin));

    listen(sockAccept, 20);

    /* void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg);  */
    eventset(&g_events[MAX_EVENTS], sockAccept, AcceptConnect, &g_events[MAX_EVENTS]);

    /* void eventadd(int efd, int events, struct myevent_s *ev) */
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]);  //将lfd添加到监听树上，监听读事件

    return;
}

int main(int argc, char *argv[])
{
    int port=SERV_PORT;

    g_efd = epoll_create(MAX_EVENTS + 1); //创建红黑树,返回给全局 g_efd
    if(g_efd <= 0)
        printf("create efd in %s err %s\n", __func__, strerror(errno));

    initlistensocket(g_efd, port); //初始化监听socket

    struct epoll_event events[MAX_EVENTS + 1];  //定义这个结构体数组，用来接收epoll_wait传出的满足监听事件的fd结构体
    printf("server running:port[%d]\n", port);

    int i;
    while(1)
    {
        //调用eppoll_wait等待接入的客户端事件,epoll_wait传出的是满足监听条件的那些fd的struct epoll_event类型
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS+1, 1000);
        if (nfd < 0)
        {
            printf("epoll_wait error, exit\n");
            exit(-1);
        }
        for(i = 0; i < nfd; i++)
        {
            //evtAdd()函数中，添加到监听树中监听事件的时候将myevents_t结构体类型给了ptr指针
            //这里epoll_wait返回的时候，同样会返回对应fd的myevents_t类型的指针
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            //如果监听的是读事件，并返回的是读事件
            if((events[i].events & EPOLLIN) &&(ev->events & EPOLLIN))
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            //如果监听的是写事件，并返回的是写事件
            if((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }

    return 0;
}
