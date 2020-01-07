#include "fun.h"

/****************************************************
 * 通过帧数据解析报文，返回一个附带报文信息的结构体
 * 参数：
 *      unsigned char *buff：待解析的帧数据
 *      int size ：待解析的帧数据的长度
 * 返回值：
 *      返回一个附带解析信息的结构体
 * 
 * 
 * *************************************************/
MyProtocolType ProtocolType(unsigned char *buff, int size)
{
    MyProtocolType TempProtocolType;

    unsigned char buf[1500] = "";

    for (int i = 0; i < size; i++)
        buf[i] = *(buff + i);

    unsigned short type = 0;

    //获取 目的 mac地址
    sprintf(TempProtocolType.dst_mac, "%02x:%02x:%02x:%02x:%02x:%02x", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    //获取 源 mac地址
    sprintf(TempProtocolType.src_mac, "%02x:%02x:%02x:%02x:%02x:%02x", buf[0 + 6], buf[1 + 6], buf[2 + 6], buf[3 + 6], buf[4 + 6], buf[5 + 6]);
    //获取 协议类型
    type = ntohs(*(unsigned short *)(buf + 12));

    printf("协议类型：type = %#x\n", type);

    printf("%s >> %s\n", TempProtocolType.src_mac, TempProtocolType.dst_mac);
    if (type == 0x0800)
    {
        printf("本协议为IP协议\n");

        sprintf(TempProtocolType.type, "%s", "IP");

        printf("源IP :%d:%d:%d:%d \n 目的IP :%d:%d:%d:%d\n", buf[26], buf[27], buf[28], buf[29], buf[30], buf[31], buf[32], buf[33]);

        sprintf(TempProtocolType.src_ip, "%d:%d:%d:%d", buf[26], buf[27], buf[28], buf[29]);

        sprintf(TempProtocolType.dst_ip, "%d:%d:%d:%d", buf[30], buf[31], buf[32], buf[33]);

        if (buf[14 + 8 + 1] == 0x01)
        {
            printf("本协议为ICMP\n");

            if (buf[14 + (buf[14] & 0x0f) * 4 + 1] == 0x08)

                sprintf(TempProtocolType.type, "%s", "ICMP_REQ"); //ICMP回显请求

            else

                sprintf(TempProtocolType.type, "%s", "ICMP_RES"); //ICMP回显应答
        }
        else if (buf[14 + 8 + 1] == 0x02)
        {
            sprintf(TempProtocolType.type, "%s", "NULL"); //不支持

            // printf("本协议为IGMP\n");

            //     if( buf[14+(buf[14]&0x0f)*4 + 1] == 0x08)

            //     sprintf(TempProtocolType.type,"%s","IGMP_REQ"); //IGMP回显请求

            //     else

            //     sprintf(TempProtocolType.type,"%s","IGMP_RES"); //IGMP回显应答
        }
        else if (buf[14 + 8 + 1] == 0x06)
        {
            printf("本协议为TCP\n");

            sprintf(TempProtocolType.type, "%s", "TCP");

            printf("源端口:%hu\n目的端口:%hu\n", ntohs((*(unsigned short *)buf + 14 + 20)), ntohs((*(unsigned short *)buf + 14 + 20 + 2)));

            TempProtocolType.src_port = ntohs((*(unsigned short *)buf + 14 + 20));

            TempProtocolType.src_port = ntohs((*(unsigned short *)buf + 14 + 20 + 2));
        }
        else if (buf[14 + 8 + 1] == 0x11)
        {
            printf("本协议为UDP\n");

            sprintf(TempProtocolType.type, "%s", "UDP");

            printf("源端口:%hu\n目的端口:%hu\n", ntohs((*(unsigned short *)buf + 14 + 20)), ntohs((*(unsigned short *)buf + 14 + 20 + 2)));

            TempProtocolType.src_port = ntohs((*(unsigned short *)buf + 14 + 20));

            TempProtocolType.src_port = ntohs((*(unsigned short *)buf + 14 + 20 + 2));
        }
    }
    else if (type == 0x0806)
    {
        printf("本协议为ARP协议\n");

        if (buf[22] == 0x0001)

            sprintf(TempProtocolType.type, "%s", "ARP_REQ"); //ARP请求

        else

            sprintf(TempProtocolType.type, "%s", "ARP_RES"); //ARP应答

        printf("源IP :%d:%d:%d:%d \n 目的IP :%d:%d:%d:%d\n", buf[28], buf[29], buf[30], buf[31], buf[38], buf[39], buf[40], buf[41]);

        sprintf(TempProtocolType.src_ip, "%d:%d:%d:%d", buf[28], buf[29], buf[30], buf[31]);

        sprintf(TempProtocolType.dst_ip, "%d:%d:%d:%d", buf[38], buf[39], buf[40], buf[41]);
    }
    else if (type == 0x8035)
    {
        printf("本协议为RARP协议\n");

        if (buf[22] == 0x0003)

            sprintf(TempProtocolType.type, "%s", "RARP_REQ"); //RARP请求

        else

            sprintf(TempProtocolType.type, "%s", "RARP_RES"); //RARP应答

        printf("源IP :%d:%d:%d:%d \n 目的IP :%d:%d:%d:%d\n", buf[28], buf[29], buf[30], buf[31], buf[38], buf[39], buf[40], buf[41]);

        sprintf(TempProtocolType.src_ip, "%d:%d:%d:%d", buf[28], buf[29], buf[30], buf[31]);

        sprintf(TempProtocolType.dst_ip, "%d:%d:%d:%d", buf[38], buf[39], buf[40], buf[41]);
    }
    else

        sprintf(TempProtocolType.type, "%s", "NULL");

    return TempProtocolType;
}
/****************************************************
 * 通过传入的两个IP地址，两个子网掩码，IP的长度后，可以
 * 判断两个IP地址是否同一网段
 * 参数：
 *      unsigned char *first：第一个IP地址
 *      unsigned char *second：第二个IP地址
 *      char *firstmask：第一个IP地址的子网掩码
 *      char *secondmask：第二个IP地址的子网掩码
 *      int size：IP地址的字节数
 * 返回值：
 *      返回： 1 则两个IP是同一网段
 *             0 则两个IP不是同一网段
 * *************************************************/
int IsSameSegment(unsigned char *first, unsigned char *second, unsigned char *firstmask, unsigned char *secondmask, int size)
{
    int First[size], Second[size], FirstMask[size], SecondMask[size];

    if (strcmp(firstmask, secondmask) != 0)
        return 0;
    sscanf(first, "%d.%d.%d.%d", First[0], First[1], First[2], First[3]);
    sscanf(second, "%d.%d.%d.%d", Second[0], Second[1], Second[2], Second[3]);
    sscanf(firstmask, "%d.%d.%d.%d", FirstMask[0], FirstMask[1], FirstMask[2], FirstMask[3]);
    sscanf(secondmask, "%d.%d.%d.%d", SecondMask[0], SecondMask[1], SecondMask[2], SecondMask[3]);
    for (int i = 0; i < size; i++)
    {
        if ((First[i] & FirstMask[i]) != (Second[i] & SecondMask[i]))
        {
            return 0;
        }
    }

    return 1;
}

/***************************************************
 * 传入网卡名称，获得网卡名称对应得IP地址，如果出错，返回
 * -1 并打印错误信息;
 *参数：    
 *      char *name：网卡名称    
 *      char *ipaddr:获得的IP地址所存放的位置
 * 返回值：
 *        如果出错返回-1
 *        如果不出错返回0
 * 
 * *************************************************/
int GetLocalIp(const char *name, char *ipaddr)
{

    int sock_get_ip;

    struct sockaddr_in *sin;
    struct ifreq ifr_ip;

    if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket create failse...GetLocalIp!/n");
        return -1;
    }

    memset(&ifr_ip, 0, sizeof(ifr_ip));
    strncpy(ifr_ip.ifr_name, name, sizeof(ifr_ip.ifr_name) - 1);

    if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0)
    {
        return -1;
    }
    sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
    strcpy(ipaddr, inet_ntoa(sin->sin_addr));

    printf("local ip:%s /n", ipaddr);
    close(sock_get_ip);

    return 0;
}

/***************************************************
 * 传入网卡名称，获得网卡名称对应得Mask子网掩码，如果出错，返回
 * -1 并打印错误信息;
 *参数：    
 *      char *name：网卡名称
 *      char *ipmask:获得的子网掩码所存放的位置    
 * 返回值：
 *        如果出错返回-1
 *        如果不出错返回0
 * 
 * *************************************************/
int GetLocalMask(const char *name, char *ipmask)
{

    int sock_get_mask;

    struct sockaddr_in *sin;
    struct ifreq ifr_ip;

    if ((sock_get_mask = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket create failse...GetLocalIp!/n");
        return -1;
    }

    memset(&ifr_ip, 0, sizeof(ifr_ip));
    strncpy(ifr_ip.ifr_name, name, sizeof(ifr_ip.ifr_name) - 1);

    if (ioctl(sock_get_mask, SIOCGIFADDR, &ifr_ip) < 0)
    {
        return -1;
    }
    sin = (struct sockaddr_in *)&ifr_ip.ifr_netmask;
    strcpy(ipmask, inet_ntoa(sin->sin_addr));

    printf("local mask:%s /n", ipmask);
    close(sock_get_mask);

    return 0;
}

/***************************************************
 * 传入网卡名称，获得网卡名称对应得MAC地址，如果出错，返回
 * -1 并打印错误信息;
 *参数：    
 *      char *name：网卡名称
 *      char *mac_addr：获得的MAC地址所存放的位置
 * 返回值：
 *        如果出错返回-1
 *        如果不出错返回0
 * 
 * *************************************************/
int GetLocalMac(const char *name, char *mac_addr)
{
    int sock_mac;

    struct ifreq ifr_mac;

    sock_mac = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_mac == -1)
    {
        perror("create socket falise...mac/n");
        return -1;
    }

    memset(&ifr_mac, 0, sizeof(ifr_mac));
    strncpy(ifr_mac.ifr_name, name, sizeof(ifr_mac.ifr_name) - 1);

    if ((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
    {
        printf("mac ioctl error/n");
        return -1;
    }

    sprintf(mac_addr, "%02x%02x%02x%02x%02x%02x",
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);

    printf("local mac:%s /n", mac_addr);

    close(sock_mac);
    return 0;
}
/***************************************************
 * 获取所有网卡名称，可同时获取IP地址，子网掩码。如果出错，返回
 * -1 并打印错误信息;
 *参数：    
 *      
 * 返回值：
 *        如果出错返回-1
 *        如果不出错返回0
 * 
 * *************************************************/
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
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            printf("n>>> interfaceName: %sn", ifa->ifa_name);

            sin = (struct sockaddr_in *)ifa->ifa_addr;
            printf(">>> ipAddress: %sn", inet_ntoa(sin->sin_addr));

            //    sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
            //     printf(">>> broadcast: %sn", inet_ntoa(sin->sin_addr));

            sin = (struct sockaddr_in *)ifa->ifa_netmask;
            printf(">>> subnetMask: %sn", inet_ntoa(sin->sin_addr));
        }
    }

    freeifaddrs(ifList);

    return 0;
}



int ReadFile(void **Head, int Type)
{
     Config_Route_Msg *Route_temp = (Config_Route_Msg *)malloc(sizeof(Config_Route_Msg));
    char text[1000] = {0};
    char *FileName ="Config_Route_Msg";

    FILE *fp;
    if ((fp = fopen(FileName, "r")) == NULL)
    {
        perror("fail to fopen");
        return -1;
    }
    while (1)
    {

        if (fgets(text, 500, fp) == NULL)
        {
            break;
        }
        else if (text[0] == "#")
        {
            continue;
        }
        if (strchr(text, "{") != NULL)
        {
            
        }
        if (strstr(text, "Eth_Name") != NULL)
     {
                   MyStrcpy(Route_temp->Route_Name,strchr(text,':')+1,strchr(text,'\n'));
                      //  strncpy(Eth_temp->Eth_Name, strstr(text, "Eth_Name") + 9,strlen(text)-14);
                        printf("*");
                        printf("Eth_Name:%s\n",Route_temp->Route_Name);
    }        

             
             else  if (strstr(text, "Route_Ip") != NULL)
               {
                        
                        MyStrcpy(a,strchr(text,':')+1,strchr(text,'\n'));
                        Route_temp->Route_Ip.s_addr = inet_addr(a);
                         
                        printf("Route_Ip:%s\n",inet_ntoa(Route_temp->Route_Ip));
               }

             else  if (strstr(text, "Route_Mask") != NULL)
                {
                        
                        MyStrcpy(a,strchr(text,':')+1,strchr(text,'\n'));
                        Route_temp->Route_Mask.s_addr = inet_addr(a);
                         
                        printf("Route_Mask:%s\n",inet_ntoa(Route_temp->Route_Mask));
                    
                       
                }
             else  if (strstr(text, "Route_Mac") != NULL)
                {       strncpy(Route_temp->Route_Mac, strstr(text, "Route_Mac") + 8,6);
                        printf("Route_Mac:%s\n",strstr(text, "Route_Mac") + 8);
                }
            if (strchr(text, '}') != NULL)
            {
                return;
            }
        

        return 0;
    }
}


void GetEth_Msg(FILE *fp,Config_Eth_Msg** Head)
{
    Config_Eth_Msg *Eth_temp = (Config_Eth_Msg *)malloc(sizeof(Config_Eth_Msg));
    
    
     while (1)
        {
            char text[500],a[100];
            
            if (fgets(text, 500, fp) == NULL)
            {
                break;
            }
            else if (strchr(text, '#') != NULL)
            {
                continue;
            }  
            
               if (strstr(text, "Eth_Name") != NULL)
               {
                   MyStrcpy(Eth_temp->Eth_Name,strchr(text,':')+1,strchr(text,'\n'));
                      //  strncpy(Eth_temp->Eth_Name, strstr(text, "Eth_Name") + 9,strlen(text)-14);
                        printf("*");
                        printf("Eth_Name:%s\n",Eth_temp->Eth_Name);
               }        

             
             else  if (strstr(text, "Eth_Ip") != NULL)
               {
                        
                        MyStrcpy(a,strchr(text,':')+1,strchr(text,'\n'));
                        Eth_temp->Eth_Ip.s_addr = inet_addr(a);
                         
                        printf("Eth_Ip:%s\n",inet_ntoa(Eth_temp->Eth_Ip));
               }

             else  if (strstr(text, "Eth_Mask") != NULL)
                {
                        
                        MyStrcpy(a,strchr(text,':')+1,strchr(text,'\n'));
                        Eth_temp->Eth_Mask.s_addr = inet_addr(a);
                         
                        printf("Eth_Mask:%s\n",inet_ntoa(Eth_temp->Eth_Mask));
                    
                       
                }
             else  if (strstr(text, "Eth_Mac") != NULL)
                {      
                    MyStrcpy(a,strchr(text,':')+1,strchr(text,'\n'));
                    MyGetMac(Eth_temp->Eth_Mac,a);
                      
                        printf("Eth_Mac:%s\n",Eth_temp->Eth_Mac);
                }
            if (strchr(text, '}') != NULL)
            {
                return;
            }
                     
    }
    Eth_temp->front = NULL;
    Eth_temp->next = NULL;
    
    InsertEth_MsgToList(Eth_temp,Head);

}


void GetRoute_Msg(int fp,Config_Route_Msg** Head)
{
    Config_Route_Msg *Route_temp = (Config_Route_Msg *)malloc(sizeof(Config_Route_Msg));
    char text[500],a[200];

     while (1)
        {

            if (fgets(text, 500, fp) == NULL)
            {
                break;
            }
            else if (text[0] == "#")
            {
                continue;
            }  
            
                     
    }
    Route_temp->front = NULL;
    Route_temp->next = NULL;
    InsertRoute_MsgToList(Route_temp,Head);
}

void GetFilter_Msg(int fp,Filter_Msg** Head)
{
    Filter_Msg *Filter_temp = (Filter_Msg *)malloc(sizeof(Filter_Msg));
    char text[500];
     while (1)
        {

            if (fgets(text, 500, fp) == NULL)
            {
                break;
            }
            else if (text[0] == "#")
            {
                continue;
            }  
            if (strchr(text, "}") != NULL)
            {
               if (strstr(text, "Filter_Name") != NULL)
                        strcpy(Filter_temp->Filter_Name, strstr(text, "Filter_Name") + 12);
               if (strstr(text, "Filter_Type") != NULL)
                        strcpy(Filter_temp->Filter_Type, strstr(text, "Filter_Type") + 12);
               if (strstr(text, "Filter_Src_Port") != NULL)
                        sscanf(text,":%hu",Filter_temp->Filter_Src_Port);
               if (strstr(text, "Filter_Dst_Port") != NULL)
                        sscanf(text,":%hu",Filter_temp->Filter_Dst_Port);
                   if (strstr(text, "Filter_Src_Ip") != NULL)
                        strcpy(Filter_temp->Filter_Src_Ip, strstr(text, "Filter_Src_Ip") + 14); 
                        if (strstr(text, "Filter_Dst_Ip") != NULL)
                        strcpy(Filter_temp->Filter_Dst_Ip, strstr(text, "Filter_Dst_Ip") + 14);
                        if (strstr(text, "Filter_Src_Mac") != NULL)
                        strcpy(Filter_temp->Filter_Src_Mac, strstr(text, "Filter_Src_Mac") + 15);
                        if (strstr(text, "Filter_Dst_Mac") != NULL)
                        strcpy(Filter_temp->Filter_Dst_Mac, strstr(text, "Filter_Dst_Mac") + 15);
                            
            
            }
                     
    }
    Filter_temp->front = NULL;
    Filter_temp->next = NULL;
    InsertFilter_MsgToList(Filter_temp,Head);
}


void InsertEth_MsgToList(Config_Eth_Msg* Node,Config_Eth_Msg** Head)
{
        if (*Head == NULL)
    {
        *Head = Node;    
    }
    else
    {
             //定义一个指针变量保存第一个结点的地址
            Config_Eth_Msg *p = *Head;

            //比较p的下一个结点的数据与新插入的结点的数据的关系
            //如果一直是小，则一直后移,直到找到的大的，则将新插入的结点插入到p的后面
            //如果链表到头了，将其插入到最后面
            while (p->next != NULL )
            {
                p = p->next;
            }

           Node->front = p;
        //将新插入结点的地址保存在最后一个结点的next指针里面
        p->next = Node;
        }
}
void InsertRoute_MsgToList(Config_Route_Msg* Node,Config_Route_Msg** Head)
{
       if (*Head == NULL)
    {
        *Head = Node; 
    }
    else
    {
             //定义一个指针变量保存第一个结点的地址
            Config_Route_Msg *p = *Head;

            //比较p的下一个结点的数据与新插入的结点的数据的关系
            //如果一直是小，则一直后移,直到找到的大的，则将新插入的结点插入到p的后面
            //如果链表到头了，将其插入到最后面
            while (p->next != NULL )
            {
                p = p->next;
            }
           Node->front = p;
        //将新插入结点的地址保存在最后一个结点的next指针里面
        p->next = Node;
        }
}
void InsertFilter_MsgToList(Filter_Msg* Node,Filter_Msg** Head)
{
           if (*Head == NULL)
    {
        *Head = Node;
        
    }
    else
    {
       
             //定义一个指针变量保存第一个结点的地址
            Filter_Msg *p = *Head;

            //比较p的下一个结点的数据与新插入的结点的数据的关系
            //如果一直是小，则一直后移,直到找到的大的，则将新插入的结点插入到p的后面
            //如果链表到头了，将其插入到最后面
            while (p->next != NULL )
            {
                p = p->next;
            }

           Node->front = p;
        //将新插入结点的地址保存在最后一个结点的next指针里面
        p->next = Node;
        }
}

void GetEth_MsgList(Config_Eth_Msg *head)
{
    
    while(head->next != NULL)
    {
        
        printf("Eth_Name:%s\n",head->Eth_Name);
        printf("Eth_Ip:%s\n",head->Eth_Ip);
        printf("Eth_Mask:%s\n",head->Eth_Mask);
        printf("Eth_Mac:%s\n",head->Eth_Mac);
        head = head->next;
    }
}

void GetRoute_MsgList(Config_Route_Msg *head)
{
    
    while(head->next != NULL)
    {
        
        printf("Route_Name:%s\n",head->Route_Name);
        printf("Route_Ip:%s\n",head->Route_Ip);
        printf("Route_Mask:%s\n",head->Route_Mask);
        printf("Route_Mac:%s\n",head->Route_Macs);
        head = head->next;
    }
}

void GetFilter_MsgList(Filter_Msg *head)
{
    
    while(head->next != NULL)
    {
        
        printf("Filter_Name:%s\n",head->Filter_Name);
        printf("Filter_Type:%s\n",head->Filter_Type);
        printf("Filter_Src_Ip:%s\n",head->Filter_Src_Ip);
        printf("Filter_Dst_Ip:%s\n",head->Filter_Dst_Ip);
        printf("Filter_Src_Mac:%s\n",head->Filter_Src_Mac);
        printf("Filter_Dst_Mac:%s\n",head->Filter_Dst_Mac);
        head = head->next;
    }
}


void MyStrcpy(char *s1, char *s2,char *end)
{
     
     //直到s2遇到第一个\0循环结束
     while (*s2 != '\0' && s2 != end)
     {
          //如果不是\0，将s2中的每一个字符赋值给s1
          *s1 = *s2;

          //赋值完毕后，两个指针向后移动接着判断下一个字符
          s1++;
          s2++;
          
     }
     *s1 = '\0';
}

void MyGetMac(char *BackMac, const char *Mac)
{
    for(int i = 0 ; i < 6 ; i++)
    {
         short tmp = ((My_atoi(*(Mac+i))  <<  4)  | (My_atoi(*(Mac+i+1)) & 0x0f));
         short *temp = tmp ;
      *(BackMac+i)=*((char*)temp);
    }
}

unsigned short My_atoi(char c)
{
   if(c == 'a'||c == 'A')
       return 10;
       
   else if(c == 'b'||c =='B')
        return 11;  

   else if(c == 'c'||c =='C')
        return 12;

  else  if(c == 'd'||c =='D')
        return 13; 

   else if(c ==  'e'||c =='E')
        return 14; 

   else if(c == 'f'||c =='F')
        return 15;     
    else
    {

        return c-48;
    }
} 




//ARP
int SendArp()
{
unsigned char dst_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char srt_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dst_ip[6] = {10,0,121,237};
unsigned char srt_ip[6] = {10,0,121,253};
unsigned char msg[1500] = "";
struct ether_header *eth_hd  = (struct ether_header *)msg;
memcpy(eth_hd->ether_dhost,dst_mac,6);
memcpy(eth_hd->ether_shost,srt_mac,6);
eth_hd->ether_type = htons(0x0806);
struct arphdr *arp_hd = (struct arphdr*)(msg+14);
arp_hd->ar_hrd = htons(1);
arp_hd->ar_pro = htons(0x0800);
arp_hd->ar_hln = 6;
arp_hd->ar_pln = 4;
arp_hd->ar_op = htons(1);
memcpy(arp_hd->__ar_sha,srt_mac,6);
memcpy(arp_hd->__ar_sip,srt_ip,4);
memcpy(arp_hd->__ar_tha,dst_mac,6);
memcpy(arp_hd->__ar_tip,dst_ip,4);

	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	int len = 42;
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
    pthread_t tid;
	pthread_create(&tid,NULL,Rec,(void *)&fd);
	pthread_detach(tid);
    int  i;
	while(i < 255 )
	{
		msg[41] = i ;
		sendto(fd,msg,len,0,(struct sockaddr *)&sll,sizeof(sll));
		i++;
	}
	
	close(fd);	
	return 0;
}	


void *Rec(void * arg)
{
while(1)
{	
	int fd  =  *(int *)arg;
		unsigned char recv_buf[1500] = "";
		recvfrom(fd,recv_buf,sizeof(recv_buf),0,NULL,NULL);
		if(ntohs(*(unsigned short *)(recv_buf+12)) == 0x0806)
		{
			if(ntohs(*(unsigned short *)(recv_buf+14+6)) == 2)
			{
				char mac[18] = "";
				sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",recv_buf[6],recv_buf[7],recv_buf[8],recv_buf[9],recv_buf[10],recv_buf[11]);
				char ip[16] = "";
				sprintf(ip,"%d.%d.%d.%d",recv_buf[28],recv_buf[29],recv_buf[30],recv_buf[31]);
				printf("%s --> %s\n",ip,mac);
			}
		}
}
}

int interface_num=0;//接口数量
INTERFACE net_interface[MAXINTERFACES];//接口数据

/******************************************************************
函	数:	int get_interface_num()
功	能:	获取接口数量
参	数:	无
*******************************************************************/
int get_interface_num(){
	return interface_num;
}

/******************************************************************
函	数:	int getinterface()
功	能:	获取接口信息
参	数:	无
*******************************************************************/
void getinterface(){
	struct ifreq buf[MAXINTERFACES];    /* ifreq结构数组 */
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