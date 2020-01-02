#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void *deal_client(void *arg);

int main(int argc, char const *argv[])
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[1]));
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd,(struct sockaddr *)&my_addr,sizeof(my_addr));

	listen(sockfd,1000);

	while(1)
	{
		struct sockaddr_in client_addr;
		socklen_t len = sizeof(client_addr);
		int new_fd = accept(sockfd,(struct sockaddr*)&client_addr,&len);

		pthread_t pid;
		pthread_create(&pid,NULL,deal_client,&new_fd);
		pthread_detach(pid);
	}
	close(sockfd);
	return 0;
}

void *deal_client(void *arg)
{
	int new_fd = *(int *)arg;

	unsigned char buf[1024] = "";
	memset(buf,0,sizeof(buf));
	char recv_file_name[128] = "";
	char file_name[256] = "./html/";
	char head[]="HTTP/1.1 200 OK\r\n"					\
			"Content-Type: text/html\r\n"		\
			"\r\n";
						
	char err[]=	"HTTP/1.1 404 Not Found\r\n"		\
			"Content-Type: text/html\r\n"		\
			"\r\n"								\
			"<HTML><BODY>File not found</BODY></HTML>";

	recv(new_fd,buf,sizeof(buf),0);

	sscanf(buf,"GET /%s",recv_file_name);
	strcat(file_name,recv_file_name);//要打开的文件

	int fd = open(file_name,O_RDONLY);
	if (fd < 0)
		send(new_fd,err,strlen(err),0);
	else 
		send(new_fd,head,strlen(head),0);
	int len = 0;
	char buf_text[2048] = "";
	memset(buf_text,0,sizeof(buf_text));
	while(len = read(fd,buf_text,sizeof(buf_text)))
	{
		send(new_fd,buf_text,len,0);
		memset(&buf_text,0,sizeof(buf_text));
		if (len < 2048)
			break;
	}
	close(fd);
	close(new_fd);
}