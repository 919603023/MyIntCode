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

	int len = sprintf(buf,"%c%c%s%c%s%c",0,1,argv[2],0,"octet",0);

	struct sockaddr_in dst_addr;

	bzero(&dst_addr,sizeof(dst_addr));

	dst_addr.sin_family = AF_INET;

	dst_addr.sin_port = htons(69);

	dst_addr.sin_addr.s_addr = inet_addr(argv[1]);

	sendto(sockfd,buf,len,0,(struct sockaddr *)&dst_addr,sizeof(dst_addr));

	while(1){

		bzero(buf,sizeof(buf));

		ssize_t ret = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&from_addr,&from_len);

		if(ret < 0 || buf[1] == 5)
	
		{
		
			perror("recvfrom");
	
			remove(argv[2]);
	
			break;

		}


		if(buf[1] == 3){

			if(write(fd,buf+4,512) < 0){
	
				perror("write");
		
				break;
	
			}

			buf[1] = 4;

			

			sendto(sockfd,buf,4,0,(struct sockaddr *)&from_addr,sizeof(from_addr));

			if(ret < 512){
	
				printf("传输已完成\n");
	
				break;

			}

		}

	}

	close(fd);

	close(sockfd);

}

