#include<stdio.h>
#include<sys/socket.h>
#include<netpacket/packet.h>
#include<net/if_arp.h>
#include<arpa/inet.h>
#include<netinet/ether.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<netpacket/packet.h>
#include<net/if.h>
#include<string.h>
#include<pthread.h>
#include<net/ethernet.h>
int main()
{
//获取键盘输入
printf("请输入一个字符串：");
char data[128] = "";
fgets(data,sizeof(data),stdin);
data[strlen(data)-1] = 0;
int data_len = strlen(data);
//udp数据长度必须是偶数（方便校验）
int data_len = strlen(data)+
data_len = strlen(data) + strlen(data)%2;

unsigned char dst_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char srt_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dst_ip[6] = {10,0,121,237};
unsigned char srt_ip[6] = {10,0,121,253};
unsigned char msg[1500] = "";
struct ether_header *eth_hd  = (struct ether_header *)msg;
memcpy(eth_hd->ether_dhost,dst_mac,6);
memcpy(eth_hd->ether_shost,srt_mac,6);
eth_hd->ether_type = htons(0x0800);
//组IP头
struct iphdr *ip_hd = (struct iphdr *)(msg+14);
ip_hd->version = 4;
ip_hd->ihl = 5;
ip_hd->tos = 0;
ip_hd->tot_len = htons(20+8+data_len);
ip_hd->id = htons(0);
ip_hd->frag_off = htons(0);
ip_hd->ttl = 128;
ip_hd->protocol = 17;//upd协议
ip_hd->check = htons(0);//upd 校验
ip_hd->saddr = inet_addr("10.0.121.166");//ubuntu的IP
ip_hd->daddr = inet_addr("10.0.121.237");//win10的IP

	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	int len = 42;
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	while(1)
	{
	
	sendto(fd,msg,len,0,(struct sockaddr *)&sll,sizeof(sll));
	printf("sendto over\n");	
	sleep(1);
	}
	
	close(fd);	
	return 0;
}	
