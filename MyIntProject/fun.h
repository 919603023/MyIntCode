#ifndef _FUN_H_
#define _FUN_H_

#include<stdlib.h>

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

#define config_eth_msg 0

#define config_route 1

#define config_filter_msg 2

typedef struct
{
  unsigned char type[10];

  in_port_t src_port;

  in_port_t dst_port;

  struct in_addr src_ip;

  struct in_addr dst_ip;

  unsigned char src_mac[6];

  unsigned char dst_mac[6];

} MyProtocolType;

//config_eth_msg网卡信息
typedef struct Eth_MsgNode
{

  char Eth_Name[10]; //数据域

  struct in_addr Eth_Ip;

  struct in_addr Eth_Mask;

  unsigned char Eth_Mac[6];

  struct Eth_MsgNode *front;
  
  struct Eth_MsgNode *next;

} Config_Eth_Msg;

//config_route路由表文件
typedef struct Route_MsgNode
{

  char Route_Name[10]; //数据域

  struct in_addr Route_Ip;

  struct in_addr Route_Mask;

  unsigned char Route_Mac[6];

  struct Route_MsgNode *front;

  struct Route_MsgNode *next;

}Config_Route_Msg;

//config _filter_msg过滤规则
typedef struct Filter_MsgNode
{

  char Filter_Name[10]; //数据域

  unsigned char Filter_Type[10];

  in_port_t Filter_Src_Port;

  in_port_t Filter_Dst_Port;

  struct in_addr Filter_Src_Ip;

  struct in_addr Filter_Dst_Ip;

  unsigned char Filter_Src_Mac[6];

  unsigned char Filter_Dst_Mac[6];

  struct Filter_MsgNode *front;

  struct Filter_MsgNode *next;

} Filter_Msg;

extern MyProtocolType ProtocolType(unsigned char *buff, int size);

extern int IsSameSegment(unsigned char *first, unsigned char *second, unsigned char *firstmask, unsigned char *secondmask, int size);

#endif