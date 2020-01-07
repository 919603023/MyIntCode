#include <stdio.h>
#include <stdlib.h>
typedef struct Route_MsgNode
{

    unsigned char Route_Ip[4]; //目的IP地址

    unsigned char Route_Netmask[4]; //子网掩码

    unsigned char Route_NextHop[4]; //下一跳，发向下一个IP指向的网段

    struct Route_MsgNode *front;

    struct Route_MsgNode *next;

} CONFIG_ROUTE_MSG;
int ReadFile(void **Head, int Type)
{
    CONFIG_ROUTE_MSG *Route_temp = (CONFIG_ROUTE_MSG *)malloc(sizeof(CONFIG_ROUTE_MSG));
    char text[1000] = {0};
    char *FileName = "Config_Route_Msg";
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

            if (strstr(text, "Route_Ip") != NULL)
            {

                char buf[4];
                char buff[100];
                sscanf(text, "%[^:]:%d.%d.%d.%d", buff, &buf[0], &buf[1], &buf[2], &buf[3]);
                printf("Route_IP:");
                for (int i = 0; i < 4; i++)
                {
                    printf("%d.", buf[i]);
                }
                printf("\n");
            }
            else if (strstr(text, "Route_Netmask") != NULL)
            {

                char buf[4];
                char buff[100];
                sscanf(text, "%[^:]:%d.%d.%d.%d", buff, &buf[0], &buf[1], &buf[2], &buf[3]);
                printf("Route_Netmask:");
                for (int i = 0; i < 4; i++)
                {
                    printf("%d.", buf[i]);
                }
                printf("\n");
            }
            else if (strstr(text, "Route_NextHop") != NULL)
            {
                char buf[4];
                char buff[100];
                sscanf(text, "%[^:]:%d.%d.%d.%d", buff, &buf[0], &buf[1], &buf[2], &buf[3]);
                printf("Route_NextHop:");
                for (int i = 0; i < 4; i++)
                {
                    printf("%d.", buf[i]);
                }
                printf("\n");
            }
            if (strchr(text, '}') != NULL)
            {
                return 0;
            }
        }
    }
}