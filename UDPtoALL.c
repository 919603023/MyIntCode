#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h>
int main()
	
{
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	char buf[1500] = "";
	struct sockaddr_in sender;
	bzero(&sender,sizeof(sender));
	sender.sin_family = AF_INET;
	sender.sin_port = htons(8080);
	int yes = 1;
	setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&yes,4);
	inet_pton(AF_INET,"192.168.1.255",&sender.sin_addr.s_addr);
		printf("Me : ");

		sendto(fd,"d",sizeof("d"),0,(struct sockaddr *)&sender,sizeof(sender));
	
	
		
	
	
	
	
	
}
