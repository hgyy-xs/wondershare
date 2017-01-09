#include <sys/types.h>   
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>
#include <linux/if.h>   
  
#define BUFLEN 256   
#define ifName "lo"

unsigned char CRC8(unsigned char *pData, int nLength)
{
    unsigned char crc;
    unsigned char i;
    crc = 0;
    while(nLength--)
    {
       crc ^= *pData++;
       for(i = 0;i < 8;i++)
       {
           if(crc & 0x80)
           {  
               crc = (crc << 1) ^ 0x31;
           }else crc <<= 1;
       }
    }
   return crc;
}

int main(int argc, char** argv)
{  
    struct sockaddr_in peeraddr, myaddr;  
    int sockfd;  
    char msg[strlen(argv[1])+1];
 
//  printf("argv:%s\n",argv[1]);
    memset(msg,0,sizeof(msg));
   /* char msg[]="wondershare"; */
    strcpy(msg,argv[1]);
    char sendbuf[100];  
    unsigned int socklen;
    char ip[32];  
  	printf("input msg:%s\n",msg );
    /* 创建 socket 用于UDP通讯 */  
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
    if (sockfd < 0) {  
        printf("socket creating error\n");  
        exit(1);  
    }  
    socklen = sizeof(struct sockaddr_in);  
  
    /* 设置对方的端口和IP信息 */  
    memset(&peeraddr, 0, socklen);  
    peeraddr.sin_family = AF_INET;  
    peeraddr.sin_port = htons(7838);  
  
    /* 设置自己的端口和IP信息 */  
    memset(&myaddr, 0, socklen);  
    myaddr.sin_family = AF_INET;  
    myaddr.sin_port = htons(0);  
    myaddr.sin_addr.s_addr = INADDR_ANY;
  
    /* 绑定自己的端口和IP信息到socket上 */  
    if (bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1) {  
        printf("Bind error\n");  
        exit(0);  
    }  
 
/*     if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)  
    {
        printf("SO_BINDTODEVICE");
        close(sockfd);
        exit(EXIT_FAILURE);
    }*/
    /* 发送组播消息 */  
    for (;;) {  
        int i,j;
        unsigned char crc_8;

 /*       //224.0.0.0 ~ 239.255.255.255
        sprintf(ip, "226.56.%d.%d", i, msg[i]);
        inet_pton(AF_INET, ip, &peeraddr.sin_addr);
        sendbuf[0] = msg[i];
        while(0 != sendto(sockfd, sendbuf, 0, 0, (struct sockaddr *) &peeraddr, sizeof(struct sockaddr_in)) )
*/		
        /*发送包长以及CRC   格式： 226.58.CRC.length*/

        crc_8=CRC8(msg,sizeof(msg));
printf("crc_8:%x\n",crc_8);
        sprintf(ip,"226.58.%d.%d",(int)crc_8,(int)sizeof(msg));
        inet_pton(AF_INET,ip,&peeraddr.sin_addr);
        sendbuf[0]=(char)sizeof(msg);
        for(j=0;j<2;j++)
        {
	        while(0 != sendto(sockfd,sendbuf, 0, 0, (struct sockfd *)&peeraddr, sizeof(struct sockaddr_in)))
	        {
	        	printf("sendto length error!\n");
	        	exit(0);
	        }
	    }
        /*发送数据*/
        for (i=0; i<sizeof(msg)-1; i++)
        {
            //224.0.0.0 ~ 239.255.255.255
            sprintf(ip, "226.56.%d.%d", i, msg[i]);
            //peeraddr.sin_addr.s_addr = (msg[i]<<12) | 0x04eb;
            inet_pton(AF_INET, ip, &peeraddr.sin_addr);
            /* 发送消息 */  
            sendbuf[0] = msg[i];
            //for (j=0;j<2;j++)
            //{
                while (0 != sendto(sockfd, sendbuf, 0, 0, (struct sockaddr *) &peeraddr, sizeof(struct sockaddr_in)) ) 
                {  
                    printf("sendto error!\n");  
                    exit(0);  
                }
		printf("send msg_size:%d\n",(int)sizeof(msg));
            //}  
            //fprintf(stderr, " %02x", msg[i]);
            usleep(10000);
            
        }
        fprintf(stderr, "\n");
        sleep(1);
    }  
}  
