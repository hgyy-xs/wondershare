#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <errno.h>  
#include <linux/if_ether.h>  
#include <net/if.h>  
#include <sys/ioctl.h>  
#include <string.h>  
  
#define ETH_NAME    "br-lan"  
  
int check_nic(void)  
{  
    struct ifreq ifr;  
    int skfd = socket(AF_INET, SOCK_DGRAM, 0);  //套接字，用来打开一个网络通讯端口，相当于一个文件描述符
  
    strcpy(ifr.ifr_name, ETH_NAME);  
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)  
    {  
        close(skfd);  
        return -1;  
    }  
    if(ifr.ifr_flags & IFF_RUNNING) //插拔网线会造成IFF_RUNNNING的改变
    {  
        printf("link up\n");  
        close(skfd);  
        return 0;   
    }else
        {  
        printf("link down\n");  
        close(skfd);  
        return -1;  
       }  
}  
  
  
int main(void)
{
    pid_t pid;
    int i;
    pid = fork();//第一步：创建子进程
    if (pid < 0)
      {
          printf("error fork\n");
          exit(1);//非正常运行导致退出
      }
    else if (pid > 0)
    {
        printf("父进程正常退出\n");
        exit(0);    //父进程正常退出
    }
    setsid();   //第二步：在子进程中创建新的会话  (第一子进程成为新的进程组长，并与控制终端分离)
    chdir("/"); //第三步：更改子进程工作目录为根目录
    umask(0);   //第四步：重设文件权限掩码
    close(0);   //关闭输入文件描述符，因向终端打印信息，输出文件描述符不予关闭
/*    for ( i = 0; i < getdtablesize(); ++i)   //第五步：关闭文件描述符
    {
        close(i);   //由于子进程失去与终端的联系，因此关闭输入(0)、输出(1)、报错(2)的文件描述符
    }*/
    /*进入守护进程工作区*/
    while(1)
    {
        check_nic();
        sleep(1);
    }
/*    check_nic();
    return 0;*/
    exit(0);  
}  
