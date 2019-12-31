#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<pthread.h>
void *Read(void *arg)
{
	int val1,val2,val3,val4;
	short val5;
	int fd = (int )(long)arg;
	char buf[1500] = "";
	char buf_data[1500] = "";
	char buf_val[1500] = "";
	struct sockaddr_in sender;
	bzero(&sender,sizeof(sender));
	sender.sin_family = AF_INET;
	sender.sin_port = htons(8080);
	inet_pton(AF_INET,"10.0.121.237",&sender.sin_addr.s_addr);
		printf("Me : ");

	while(1)
	{
		bzero(buf,sizeof(buf));
		bzero(buf_data,sizeof(buf_data));
		fflush(stdin);
		gets(buf);
	 	 strcpy(buf_data ,strtok(buf," "));
		
		if(strcmp(buf_data,"SENDTO") == 0)
		{
		sscanf(buf+7,"%d.%d.%d.%d:%hu\n",&val1,&val2,&val3,&val4,&val5);
			sprintf(buf_val,"%d.%d.%d.%d",val1,val2,val3,val4);
			inet_pton(AF_INET,buf_val,&sender.sin_addr.s_addr);
			sender.sin_port = htons(val5);
			printf("目标IP 已经切换为 : %d.%d.%d.%d:%d\n",val1,val2,val3,val4,val5);
		printf("Me : ");
		}
		else
		{

			
		printf("Me : ");
		sendto(fd,buf_data,sizeof(buf_data),0,(struct sockaddr *)&sender,sizeof(sender));
		
		}
	}
	
	
		
	
	
	
	
	
}
int main()
{
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(60000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(fd,(struct sockaddr *)&addr,sizeof(addr));
	pthread_t tid;
	pthread_create(&tid,NULL,Read,(void *)fd);
	pthread_detach(tid);
	while(1)
	{
		char buf[128] = "";
		char ip_str[16] = "";
		struct sockaddr_in from_addr;
		socklen_t socklen = sizeof(from_addr);
		recvfrom(fd,buf,sizeof(buf),0,(struct sockaddr *)&from_addr,&socklen);
		inet_ntop(AF_INET,&from_addr.sin_addr.s_addr,ip_str,16);
		printf("IP:%s:%u : %s\n",ip_str,ntohs(from_addr.sin_port),buf);
	}
	close(fd);	
}
