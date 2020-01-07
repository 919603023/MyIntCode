#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ether.h>
#include <stdlib.h>

#include <stdio.h>

#include <netinet/ip.h>

#include <netinet/udp.h>

#include <sys/socket.h>

#include <netpacket/packet.h>

#include <net/if_arp.h>

#include <arpa/inet.h>

#include <netinet/ether.h>

#include <sys/ioctl.h>

#include <unistd.h>

#include <netpacket/packet.h>

#include <net/if.h>

#include <string.h>

#include <pthread.h>

#include <net/ethernet.h>

#include <ifaddrs.h>

#ifndef _FUN_H_
#define _FUN_H_














#define NuLL -1
#define ICMP 0
#define IGMP 1
#define TCP 2
#define UDP 3
#define ARP_GO 4
#define ARP_BACK 5
#define RARP_GO 6
#define RARP_BACK 7

#define DELETE 0
#define FIND 1

#define UNICAST 0
#define BROADCAST 1
#define ONEUNICAST 3
typedef struct
{

	int eth;

	int type;

	unsigned short src_port;

	unsigned short dst_port;

	unsigned char src_ip[4];

	unsigned char dst_ip[4];

	unsigned char src_mac[6];

	unsigned char dst_mac[6];

} MYBUF;

typedef struct interface
{
	char name[20];			  //�ӿ�����
	unsigned char ip[4];	  //IP��ַ
	unsigned char mac[6];	 //MAC��ַ
	unsigned char netmask[4]; //��������
	unsigned char br_ip[4];   //�㲥��ַ
	int flag;				  //״̬
} INTERFACE;

typedef struct  arplist
{
    int eth;
    unsigned char ip[4];
    unsigned char mac[6];
	struct arplist * front;
	struct arplist * next;
} ARPLIST;

//config_route路由表文件
typedef struct Route_MsgNode
{

  unsigned char Route_Ip[4];

  unsigned char Route_Mac[6];

  struct Route_MsgNode *front;

  struct Route_MsgNode *next;

}Config_Route_Msg;

extern int get_interface_num();

extern int MyMacCmp(char *buf);

extern void getinterface();


extern int AnalyzeAgreement(char *buf);

extern int AND(unsigned char *first, unsigned char *secend);

extern int IsSameSegment();

extern int SendTo(int len, char *buf, int eth, int fd);

extern unsigned int BinaryAnd(unsigned int first, unsigned int second);

extern unsigned int GetIpNet(int eth);

extern int SendArp(int eth, int flag, int fd,unsigned char *ip);

extern void InsertArp_listToList(ARPLIST* Node,ARPLIST** Head);

extern int ArpDispose(char *ip,char *mac,char *mac_back,int flag);

extern int interface_num ; //接口数量

extern MYBUF mybuf;

extern INTERFACE net_interface[16]; //接口数据

extern ARPLIST *HEAD;


#endif