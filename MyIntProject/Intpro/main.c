#include"fun.h"

int main()
{
	char Arm_ip[4]= {0xC0,0xA8,0x01,0xED};
	char pc_mac[6] = {0x54, 0xEE, 0x75, 0x95, 0x8B, 0x6F};
	char Arm_mac[6] = {0x00, 0x53, 0x50, 0x00, 0x09, 0x2B};
	getinterface(); // 获取自身网卡信息
	
	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf("fd = %d\n", fd);


	
	
	
SendArp(1,ONEUNICAST,fd,Arm_ip);
SendArp(1,ONEUNICAST,fd,Arm_ip);
SendArp(1,ONEUNICAST,fd,Arm_ip);

SendArp(1,ONEUNICAST,fd,Arm_ip);
	while (1)
	{

		

		unsigned char buf[1500] = "";
		
		int len = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
		
		
		if (AnalyzeAgreement(buf) == -1)
		{
			continue;
		}

		if(mybuf.type == ARP_BACK)
		{
			
			if(ArpDispose(mybuf.src_ip,NULL,mybuf.dst_mac,FIND) == -1)
			

			// SendArp(1,UNICAST,fd,NULL);
			continue;
			
		}
		//是否是同一网段
		int Ethnum;
		

		Ethnum = IsSameSegment();

		if (Ethnum == -1)
		{
			//没有同一网段
			continue;
			//路由表查表
		}
		
		//有同一网段，返回所出去的网卡

		//查ARP表
		if(ArpDispose(mybuf.dst_ip,NULL,mybuf.dst_mac,FIND) == -1)
		{
			continue;
		}
		//如果有，返回网卡
		
		//如果没有，那就广播发ARP

		//广播后，获得目的MAC

		//查防火墙过滤表

		//固定ARM——MAC

		//固定PC———MAC

		if (Ethnum == 2)
		{
			memcpy(buf,mybuf.dst_mac, 6);
			memcpy(buf + 6, net_interface[Ethnum].mac, 6);
			SendTo(len, buf, Ethnum, fd);
			printf("to ens39\n");
			printf("协议类型：%d\n", mybuf.type);
		}
		else if (Ethnum == 1)
		{
			memcpy(buf, mybuf.dst_mac, 6);
			memcpy(buf + 6, net_interface[Ethnum].mac, 6);
			SendTo(len, buf, Ethnum, fd);
			printf("to ens33\n");
			printf("协议类型：%d\n", mybuf.type);
		}
		bzero(&mybuf, sizeof(mybuf));
	}
	close(fd);
	return 0;
}

