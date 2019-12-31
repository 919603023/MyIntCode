#include<stdio.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
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
//伪头部结构体
typedef struct{
	u_int32_t saddr;
	u_int32_t daddr;
	u_int8_t flag;
	u_int8_t type;
	u_int16_t len;
}WEI_HEAD;

unsigned short checksum(unsigned short *buf,int nword)
{
	unsigned long sum;
	for(sum = 0;nword > 0;nword--)
	{
		sum+=htons(*buf);
		buf++;

	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return ~sum;
}
void MakeData(char *data,int data_len,char *dst__ip,char *src__ip)
{
	printf("***%s***%s*****%s\n",data,dst__ip,src__ip);
unsigned char dst_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char srt_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
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
	ip_hd->id = htons(0);//标识
	ip_hd->frag_off = htons(0);//标志
	ip_hd->ttl = 128;//TTL
	ip_hd->protocol = 17;//upd协议
	ip_hd->check = htons(0);//IP 校验
	ip_hd->saddr = inet_addr(src__ip);//ubuntu的IP
	ip_hd->daddr = inet_addr(dst__ip);//win10的IP
	ip_hd->check = htons(checksum((unsigned short *)ip_hd,20/2));
//组UDP头
	struct udphdr *udp_hd = (struct udphdr *)(msg+14+20);
	udp_hd->source = htons(8080);//源端口
	udp_hd->dest = htons(2425);//目的端口
	udp_hd->len = htons(8+data_len);//UDP报文头长度+UDP数据长度
	udp_hd->check = htons(0);
	memcpy(msg+14+20+8,data,data_len);//将用户数据放入
	//定义伪头部
	unsigned char wei_head[256] = {}; 
	WEI_HEAD *head = (WEI_HEAD *)wei_head;
	head->saddr = inet_addr(src__ip);
	head->daddr = inet_addr(dst__ip);
	head->flag = 0;
	head->type = 17;
	head->len = htons(8+data_len);
	//将帧数据d中的UDP+data复制到wei_head 尾部
	memcpy(wei_head+12,msg+14+20,8+data_len);
	//udp校验
	udp_hd->check = htons(checksum((unsigned short *)wei_head,(12+8+data_len)/2));

	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	int len = 14 + 20 + 8 + data_len;
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;

	sendto(fd,msg,len,0,(struct sockaddr *)&sll,sizeof(sll));
	printf("*****************sendto over\n");
	close(fd);	
	
}
int main()
{
char data[1500];
char name[128];
char hostname[128];
char data_data[512];
//获取键盘输入
printf("你是谁\n");
fgets(name,sizeof(name),stdin);
name[strlen(name)-1] = 0;
printf("主机名\n");
fgets(hostname,sizeof(hostname),stdin);
hostname[strlen(hostname)-1] = 0;
while(1)
{
	
printf("想说什么\n");
fgets(data_data,sizeof(data_data),stdin);
data[strlen(data_data)-1] = 0;

sprintf(data,"1_lbt6_65#128#3CF011F4C149#0#0#0#4001#9:1577805872:28903:LAPTOP-FJM73KF7:32:%s",data_data);
//udp数据长度必须是偶数（方便校验）
int data_len = strlen(data) + strlen(data)%2;
MakeData(data,data_len,"10.0.121.237","10.0.121.217");

}
	
	return 0;
}	
