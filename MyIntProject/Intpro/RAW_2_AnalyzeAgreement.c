#include<stdio.h>
#include<string.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/ether.h>
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


typedef struct interface{
	char name[20];		//�ӿ�����
	unsigned char ip[4];		//IP��ַ
	unsigned char mac[6];		//MAC��ַ
	unsigned char netmask[4];	//��������
	unsigned char br_ip[4];		//�㲥��ַ
	int  flag;			//״̬
}INTERFACE;

int interface_num=0;//接口数量

INTERFACE net_interface[16];//接口数据
int get_interface_num();

int MyMacCmp(char *buf);

void getinterface();

int main()
{
	    char src_mac[18] = "";
		char dst_mac[18] = "";
		char ens33_mac[6] = {0x00,0x0C,0x29,0x75,0x13,0x7B};
		char ens39_mac[6] = {0x00,0x0C,0x29,0x75,0x13,0x85};
		
		char pc_mac[6] = {0x54,0xee,0x75,0x95,0x8b,0x6f};
		char Arm_mac[6]= {0x00,0x53,0x50,0x00,0x2C,0x63};
    	getinterface();
	
	
	
	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	printf("fd = %d\n",fd);
	
	while(1){


		unsigned char buf[1500] = "";
		int len = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
		
		unsigned short type = 0;
		
		type = ntohs(*(unsigned short *)(buf+12));
		int Num = MyMacCmp(buf);
		if(Num == -1)
		{
			continue;
		}
		if(Num  == 1)
		{
		memcpy(buf,Arm_mac,6);
		memcpy(buf+6,ens39_mac,6);
		struct sockaddr_ll sll;
	    struct ifreq ethreq;
		strncpy(ethreq.ifr_name,"ens39",IFNAMSIZ);
		ioctl(fd,SIOCGIFINDEX,&ethreq);
		
		bzero(&sll,sizeof(sll));
		sll.sll_ifindex = ethreq.ifr_ifindex;
			
		printf("ens33\n");
		printf("%d\n",len);
		sendto(fd,buf,len,0,(struct sockaddr *)&sll,sizeof(sll));
		}else
		{
			memcpy(buf,pc_mac,6);
			memcpy(buf+6,ens33_mac,6);
			
			struct sockaddr_ll sll;
			struct ifreq ethreq;
			
		strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
		ioctl(fd,SIOCGIFINDEX,&ethreq);
		
		bzero(&sll,sizeof(sll));
		sll.sll_ifindex = ethreq.ifr_ifindex;
		printf("ens39\n");
sendto(fd,buf,len,0,(struct sockaddr *)&sll,sizeof(sll));
		}
		
		
		
		//连PC
		// if(memcmp(buf,ens33_mac,6) == 0  )
		// {
			
		// }
		// else if(memcmp(buf,ens39_mac,6) == 0)
		// {
			
		// }
		
		
	}
	close(fd);
return 0;
}


int get_interface_num(){
	return interface_num;
}



void getinterface(){
	struct ifreq buf[16];    /* ifreq结构数组 */
	struct ifconf ifc;                  /* ifconf结构 */
	
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	 /* 初始化ifconf结构 */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
 
    /* 获得接口列表 */
    if (ioctl(sock_raw_fd, SIOCGIFCONF, (char *) &ifc) == -1){
        perror("SIOCGIFCONF ioctl");
        return ;
    }
    interface_num = ifc.ifc_len / sizeof(struct ifreq); /* 接口数量 */
    printf("interface_num=%d\n\n", interface_num);
 	char buff[20]="";
	int ip;
	int if_len = interface_num;
    while (if_len-- > 0){ /* 遍历每个接口 */
        printf("%s\n", buf[if_len].ifr_name); /* 接口名称 */
        sprintf(net_interface[if_len].name, "%s", buf[if_len].ifr_name); /* 接口名称 */
		printf("-%d-%s--\n",if_len,net_interface[if_len].name);
        /* 获得接口标志 */
        if (!(ioctl(sock_raw_fd, SIOCGIFFLAGS, (char *) &buf[if_len]))){
            /* 接口状态 */
            if (buf[if_len].ifr_flags & IFF_UP){
                printf("UP\n");
				net_interface[if_len].flag = 1;
            }
            else{
                printf("DOWN\n");
				net_interface[if_len].flag = 0;
            }
        }else{
            char str[256];
            sprintf(str, "SIOCGIFFLAGS ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
 
        /* IP地址 */
        if (!(ioctl(sock_raw_fd, SIOCGIFADDR, (char *) &buf[if_len]))){
			printf("IP:%s\n",(char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			bzero(buff,sizeof(buff));
			sprintf(buff, "%s", (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			inet_pton(AF_INET, buff, &ip);
			memcpy(net_interface[if_len].ip, &ip, 4);
		}else{
            char str[256];
            sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
 
        /* 子网掩码 */
        if (!(ioctl(sock_raw_fd, SIOCGIFNETMASK, (char *) &buf[if_len]))){
            printf("netmask:%s\n",(char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			bzero(buff,sizeof(buff));
			sprintf(buff, "%s", (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			inet_pton(AF_INET, buff, &ip);
			memcpy(net_interface[if_len].netmask, &ip, 4);
        }else{
            char str[256];
            sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
 
        /* 广播地址 */
        if (!(ioctl(sock_raw_fd, SIOCGIFBRDADDR, (char *) &buf[if_len]))){
            printf("br_ip:%s\n",(char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			bzero(buff,sizeof(buff));
			sprintf(buff, "%s", (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			inet_pton(AF_INET, buff, &ip);
			memcpy(net_interface[if_len].br_ip, &ip, 4);
        }else{
            char str[256];
            sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }

        /*MAC地址 */
		if (!(ioctl(sock_raw_fd, SIOCGIFHWADDR, (char *) &buf[if_len]))){
			printf("MAC:%02x:%02x:%02x:%02x:%02x:%02x\n\n",
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[0],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[1],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[2],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[3],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[4],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[5]);
			memcpy(net_interface[if_len].mac, (unsigned char *)buf[if_len].ifr_hwaddr.sa_data, 6);
		}else{
            char str[256];
            sprintf(str, "SIOCGIFHWADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
    }//–while end
    close(sock_raw_fd);   //关闭socket
}


int MyMacCmp(char *buf)
{
	for(int i = 0;i < interface_num; i++)
	{
			if(memcmp(buf,net_interface[i].mac,6) == 0)
			{
				return i;
			}
	}
	return -1;
}
int MyIpCmp(char *buf)
{
	for(int i = 0;i < interface_num; i++)
	{
			if(memcmp(buf,net_interface[i].ip,6) == 0)
			{
				return i;
			}
	}
	return -1;
}