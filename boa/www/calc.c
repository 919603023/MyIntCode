#include<stdlib.h>
#include<stdio.h>
int main(int argc,char *argv[])

{
    printf("content-type:text/html\n\n");

      

 //   printf("**%s**\n",data);
        char *len = getenv("CONTENT_LENGTH");
    char data[128] = "";
    fgets(data,atoi(len)+1,stdin);

    int data1 = 0 , data2 = 0;
    char ch = 0;
    sscanf(data,"%d%c%d",&data1,&ch,&data2);
    (ch == '+' )? (printf("%d\n",data1+data2)):(printf("%d\n",data1-data2));
    
    return 0;
}