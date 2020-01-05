#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
char head[]="HTTP/1.1 200 OK\r\n"                   \
            "Content-Type: text/html\r\n"       \
            "\r\n";
                
                
char err[]= "HTTP/1.1 404 Not Found\r\n"        \
        "Content-Type: text/html\r\n"       \
        "\r\n"                              \
        "<HTML><BODY>File not found</BODY></HTML>";
void* deal_client(void *arg);
int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in my_addr;
    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port=htons(atoi(argv[1]));
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt));

    bind(sockfd,(struct sockaddr *)&my_addr,sizeof(my_addr));

    listen(sockfd,10);

    while(1)
    {
        struct sockaddr_in c_addr;
        socklen_t c_len = sizeof(c_addr);
        int new_fd = accept(sockfd,(struct sockaddr *)&c_addr,&c_len);
        if(new_fd < 0)
            continue;
        char ip_str[16]="";
        inet_ntop(AF_INET,&c_addr.sin_addr.s_addr,ip_str,16);
        printf("%s %hu已连接\n",ip_str,ntohs(c_addr.sin_port));

        //创建线程
        pthread_t tid;
        pthread_create(&tid,NULL,deal_client, (void *)new_fd);
        pthread_detach(tid);
    }

    close(sockfd);
    return 0;
}
void* deal_client(void *arg)
{
    int new_fd = (int)arg;

    //获取浏览器的请求（只有一次）
    unsigned char msg[512]="";
    recv(new_fd,msg,sizeof(msg),0);

    //解析msg中的文件名
    char file_name[128]="./html/";
    sscanf(msg,"GET /%s", file_name+7);
    //printf("file_name=##%s###\n",file_name);

    //打开文件
    int fd = open(file_name,O_RDONLY);
    if(fd < 0)
    {
        send(new_fd,err,strlen(err),0);
    }
    else
    {
        send(new_fd,head,strlen(head),0);

        //不停的读取本地文件 发送给浏览器
        int len = 0;
        do
        {
            unsigned char buf[512]="";
            len = read(fd,buf,sizeof(buf));
            send(new_fd,buf,len,0);
        }while(len == 512);

        close(fd);
    }
    
    close(new_fd);
}