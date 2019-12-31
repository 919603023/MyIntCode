#include<stdio.h>
#include<libnet.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(){
	char device[ ]="eth0";
	char send_msg[1000] = "";
	char err_buf[100] ="";
	libnet_t *lib_net = NULL;
	libnet_t *plibnet_app;
	int lens = 0;
//	libnet_ptag_t lib_t = 0;
	unsigned char srcmac[6] = "0x11,0x11,0x11,0x11,0x11,0x11";
	unsigned char dstmac[6] = "0xff:0xff:0xff:0xff:0xff:0xff";
	char *src_ip_str = "10.0.121.237";
	char *dst_ip_str = "10.0.121.217";
	char errbuf[100];
	plibnet_app=libnet_init(LIBNET_LINK_ADV, NULL, errbuf);
//	unsigned long src_ip,dst_ip = 0;
	unsigned long srcip = inet_addr("10.0.121.237");
	unsigned long desip = inet_addr("10.0.121.217");
	libnet_build_arp(ARPHRD_ETHER,ETHERTYPE_IP,6,4,ARPOP_REPLY,(u_int8_t*)&srcmac,(u_int8_t*)&srcip,(u_int8_t*)&dstmac,(u_int8_t*)&desip,NULL,0,plibnet_app,0);
	libnet_build_ethernet(dstmac,srcmac,0x0806,NULL,0,plibnet_app,0);
	for(;;){
		libnet_write(plibnet_app);
		printf("Send ARP Packet\n");
		sleep(1);
	}
	libnet_destroy(plibnet_app);
	return 0 ;
}

