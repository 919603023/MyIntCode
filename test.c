#include<stdio.h>
typedef struct 
{
    int a ;
    int b[2] ;

}stu;
stu fun(){
    stu a;
    a.a = 1;
    a.b[0] = 2;
    a.b[1] = 3;
    return a;
}
int main()
{
    stu a;
    a = fun();
    printf("%d \n",a.a);
    printf("%d \n",a.b[0]);
    printf("%d \n",a.b[1]);
}
