
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

const int MAXLINE = 5;
int count = 1;

static int make_socket_non_blocking(int fd)
{
    int flags, s;

    flags = fcntl (fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (fd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

void sockconn()
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char buf[100];
    unsigned int value = 1;

    host = gethostbyname("192.168.118.54");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket error\r\n");
        return;
    }

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6000);
    server_addr.sin_addr = *((struct in_addr*) host->h_addr);

    int cn = connect(sockfd, (struct sockaddr *) &server_addr,
                     sizeof(server_addr));
    if (cn == -1)
    {
        printf("connect error errno=%d\r\n", errno);

        return;
    }

    sprintf(buf, "this is client msg %d", count);
    count++;
    write(sockfd, buf, strlen(buf));
    close(sockfd);

    printf("client send %s\r\n", buf);

    return;
}

int main(void)
{

    for (int i = 0; i < 1; i++)
    {
        sockconn();
    }

    return 0;
}
