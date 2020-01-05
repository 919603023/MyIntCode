#include<stdio.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
char *GetLocalMac(const char *name,char *mac_addr)  
{  
	    int sock_mac;  
	          
	       struct ifreq  ifr_mac;  
	                
	             
	           sock_mac = socket( AF_INET, SOCK_STREAM, 0 );  
		       if( sock_mac == -1)  
			           {  
					           perror("create socket falise...mac/n");  
						           return "";  
							       }  
		             
		           memset(&ifr_mac,0,sizeof(ifr_mac));     
			       strncpy(ifr_mac.ifr_name, name, sizeof(ifr_mac.ifr_name)-1);     
			         
			           if( (ioctl( sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)  
					       {  
						               printf("mac ioctl error/n");  
							               return "";  
								           }  
				         
				       sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",  
						                   (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],  
								               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],  
									                   (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],  
											               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],  
												                   (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],  
														               (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);  
				         
					         
					       close( sock_mac );  
					           return  mac_addr;  
}
int getSubnetMask()
{
	     struct sockaddr_in *sin = NULL;
	          struct ifaddrs *ifa = NULL, *ifList;
		   
		      if (getifaddrs(&ifList) < 0)
			           {
					          return -1;
						       }

		           for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
				       {
					               if(ifa->ifa_addr->sa_family == AF_INET)
							               {
							
					    char a[6];				       printf("n>>> interfaceName: %s\n", ifa->ifa_name);
	GetLocalMac(ifa->ifa_name,a);
											                 sin = (struct sockaddr_in *)ifa->ifa_addr;
				           printf("**********%s /n",a);      
													            printf(">>> ipAddress: %s\n", inet_ntoa(sin->sin_addr));

														               sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
															                   printf(">>> broadcast: %s\n", inet_ntoa(sin->sin_addr));
																	    
																	                sin = (struct sockaddr_in *)ifa->ifa_netmask;
																			            printf(">>> subnetMask: %sn", inet_ntoa(sin->sin_addr));
																				             }
						            }
			    
			        freeifaddrs(ifList);

				     return 0;
				      }
int main()
{
	getSubnetMask();
}
