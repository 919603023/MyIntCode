#include<stdio.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
	char head[]="HTTP/1.1 200 OK\r\n"					\
			"Content-Type: text/html\r\n"		\
			"\r\n";
	char err[]=	"HTTP/1.1 404 Not Found\r\n"		\
		"Content-Type: text/html\r\n"		\
		"\r\n"								\
		"<HTML><BODY>File not found</BODY></HTML>";
void *deal_client(void *arg)
{
	
	int new_fd = (int )arg;
	unsigned char buf1[500] = "";
	unsigned char file_name[500] = "./html/";
	
	

	recv(new_fd,buf1,sizeof(buf1),0);
	sscanf(buf1,"GET /%s",file_name+7);
	if(file_name[7] == 0)
		strcpy(file_name,"./html/index.html");
	
	int fd = open(file_name,O_RDONLY);
	if(fd < 0)
	
		send(new_fd,err,strlen(err),0);

	
	else
	{
		send(new_fd,head,strlen(head),0);
	
		

	
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
	
	
	return NULL;
}
int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
if(	bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))< 0)
	return 0;
	listen(sockfd,1000);
	int i = 0;
	while(1)
	{
		
		struct sockaddr_in client_addr;
		socklen_t len = sizeof(client_addr);
		int new_fd = accept(sockfd,(struct sockaddr *)&client_addr,&len);
		if(new_fd < 0)
		continue;	
		char ip_str[16] = "";
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ip_str,16);
		printf("ip:%s port ：%hu\n",ip_str,ntohs(client_addr.sin_port));
		pthread_t tid;
		pthread_create(&tid,NULL,deal_client,(void *)new_fd);
		pthread_detach(tid);
	}
	close(sockfd);
	return 0;
}
