#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char const *argv[]){
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		return 0;
	}
	
	int fd = open(argv[2],O_RDWR|O_CREAT,0664);
	if(fd < 0){
		perror("open");
		remove(argv[2]);
		return 0;
	}
	unsigned char buf[1500] = "";

	char ip_str[16] = "";

	struct sockaddr_in from_addr;

	socklen_t from_len = sizeof(from_addr);

	int len = sprintf(buf,"%c%c%s%c%s%c",0,2,argv[2],0,"octet",0);

	struct sockaddr_in dst_addr;

	bzero(&dst_addr,sizeof(dst_addr));

	dst_addr.sin_family = AF_INET;

	dst_addr.sin_port = htons(69);
	char buf_val[1500] = "";

	dst_addr.sin_addr.s_addr = inet_addr(argv[1]);

	sendto(sockfd,buf,len,0,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
	ssize_t ret1 = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&from_addr,&from_len);
	if(buf[1] == 4)
	{
		printf("**************\n");
	int i = 0;
	int b = 0;

	while(1){
	
	
		if(buf[1] == 4){
			bzero(buf_val,sizeof(buf_val));
			if(read(fd,buf_val,512) > 0){
				printf("%s\n",buf_val);
		int len = sprintf(buf,"%c%c%c%c%s",0,3,b,i,buf_val);
		sendto(sockfd,buf,len,0,(struct sockaddr *)&from_addr,sizeof(from_addr));
		
	
			}
			else if(strlen(buf_val) == 0)
			{
				printf("传输成功\n");
				return 0;

			}
	

	}

		bzero(buf,sizeof(buf));
		ssize_t ret = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&from_addr,&from_len);
	i++;
	}

	

	}
	close(fd);

	close(sockfd);

}

