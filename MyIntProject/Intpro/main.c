#include"fun.h"

int main()
{

	getinterface(); // 获取自身网卡信息
	
	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	printf("fd = %d\n", fd);


	
	
	
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
			//如果是ARP请求，使用函数查看是否有该IP的记录，
                        //如果有该IP的记录，则使用插入节点函数，插入节点
                         //如果返回-1证明列表插进去了，如果是1证明查到了ARP记录
			if(ArpDispose(mybuf.src_ip,NULL,mybuf.dst_mac,FIND) == -1)
			{
			continue;
                        }
			
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
                 //如果查到，将目的buf中的目的mac赋值为此IP的mac
                //如果查不到，应该发送ARP请求，并跳过本次循环
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

