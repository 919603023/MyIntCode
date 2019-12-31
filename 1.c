#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
int main()
{
	int fd = socket(AF_INET,SOCK_DGRAM,0);			
	 struct sockaddr_in server_addr;
	 bzero(&server_addr,sizeof(server_addr));
	 server_addr.sin_family = AF_INET;
         server_addr.sin_port = htons(8080);
	 inet_pton(AF_INET,"10.0.121.237",&server_addr.sin_addr.s_addr);
	 sleep(1);
         sendto(fd,"我是你爸爸",strlen("我是你爸爸"),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
 	char buf[128] = "";
	struct sockaddr_in from_addr;
       	socklen_t addr_len = sizeof(from_addr);
	int len = recvfrom(fd,buf,sizeof(buf),0,(struct sockaddr *)&from_addr,&addr_len);
	printf("ln = %d,buf = %s\n",len,buf);						 	char ip_str[16] = "";
	inet_ntop(AF_INET,&from_addr.sin_addr.s_addr,ip_str,16);					printf("IP:%s,port = %u\n",ip_str,ntohs(from_addr.sin_port));
	close(fd);
}

