#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/ether.h>
int main()
{
	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	printf("fd = %d\n",fd);
	close(fd);
return 0;
}
