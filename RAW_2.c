#include<stdio.h>
#include<string.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/ether.h>
int main()
{
	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	printf("fd = %d\n",fd);
	while(1){
		unsigned char buf[1500] = "";
		int len = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
		char src_mac[18] = "";
		char dst_mac[18] = "";
		unsigned short type = 0;
		sprintf(dst_mac,"%02x:%02x:%02x:%02x:%02x:%02x",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);

		sprintf(src_mac,"%02x:%02x:%02x:%02x:%02x:%02x",buf[0+6],buf[1+6],buf[2+6],buf[3+6],buf[4+6],buf[5+6]);
		type = ntohs(*(unsigned short *)(buf+12));
		printf("协议类型：type = %#x\n",type);
		printf("%s >> %s\n",src_mac,dst_mac);
		if(type == 0x0800)
		{
			printf("本协议为IP协议\n");
			
			printf("源IP :%d:%d:%d:%d \n 目的IP :%d:%d:%d:%d\n",buf[26],buf[27],buf[28],buf[29],buf[30],buf[31],buf[32],buf[33]);

			if(buf[14+8+1] == 0x01)
			{
					printf("本协议为ICMP\n");
					
			}
			else if(buf[14+8+1] == 0x06)
			{
				printf("本协议为TCP\n");
				printf("源端口:%hu\n目的端口:%hu\n",ntohs((*(unsigned short *)buf+14+20)),ntohs((*(unsigned short *)buf+14+20+2)));	
			}
			else if(buf[14+8+1] == 0x11)
			{
				printf("本协议为UDP\n");

				printf("源端口:%hu\n目的端口:%hu\n",ntohs((*(unsigned short *)buf+14+20)),ntohs((*(unsigned short *)buf+14+20+2)));	
			}
		}
		else if(type == 0x0806)
		{
			printf("本协议为ARP协议\n");
			printf("源IP :%d:%d:%d:%d \n 目的IP :%d:%d:%d:%d\n",buf[28],buf[29],buf[30],buf[31],buf[38],buf[39],buf[40],buf[41]);
			
		}
		else if(type == 0x8035)
		{
			printf("本协议为RARP协议\n");
			printf("源IP :%d:%d:%d:%d \n 目的IP :%d:%d:%d:%d\n",buf[28],buf[29],buf[30],buf[31],buf[38],buf[39],buf[40],buf[41]);
			
		}
		putchar(10);
	}
	close(fd);
return 0;
}
