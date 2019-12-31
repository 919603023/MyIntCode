#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("10.0.121.213");
	connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	send(sockfd,"haha",strlen("haha"),0);
	unsigned char buf[128] = "";
	recv(sockfd,buf,sizeof(buf),0);
	printf("buf = %s\n",buf);
	close(sockfd);
}
