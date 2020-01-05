#include "fun.h"
int main()
{
    //创建套接字
    int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));

    int len = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
    
}