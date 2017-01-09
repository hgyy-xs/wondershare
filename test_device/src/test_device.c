#include <stdio.h> //For standard things
#include <unistd.h>
#include <string.h>    //memset
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include <endian.h>

#define ifName			"eth0"
#define SNIFFER_CH		"4"
#define BUF_SIZ			1024*4

#define DEST_MAC0 0x01
#define DEST_MAC1 0x00
#define DEST_MAC2 0x5e

struct rx_frinfo {
	//unsigned char		*pskb;
	unsigned int		pktlen;
	unsigned char		*da;
	unsigned char		*sa;
	unsigned char		*pframe;
	unsigned char		to_fr_ds;
	unsigned short		seq;
	unsigned short		tid;
	unsigned char		rx_rate;
	unsigned char		rx_bw;
};

#define GetAddr1Ptr(pbuf)       ((unsigned char *)(pbuf) + 4)
#define GetAddr2Ptr(pbuf)       ((unsigned char *)(pbuf) + 10)
#define GetAddr3Ptr(pbuf)       ((unsigned char *)(pbuf) + 16)
#define GetAddr4Ptr(pbuf)       ((unsigned char *)(pbuf) + 24)
#define GetSequence(pbuf)		(le16toh(*(unsigned short *)((unsigned char*)(pbuf) + 22)) >> 4)

//在monitor模式时，内核在MAC帧前面提供额外的信息
struct ieee80211_radiotap_header {
        unsigned char        it_version;    /* 8bit: set to 0 */
        unsigned char        it_pad;        /* 8bit */
        unsigned short       it_len;        /* 16bit(Little Endian): entire length */
        unsigned int         it_present;    /* 32bit(Little Endian): fields present */
} __attribute__((__packed__));
#define FRTODS_MASK		0x03
#define TYPE_MASK		0x0C
#define SUBTYPE_MASK	0xF0
#define TYPE_DATA		0x08
#define GetToDs(pbuf)		(((*(unsigned short *)(pbuf)) & (_TO_DS_)) != 0)
#define GetFrDs(pbuf)		(((*(unsigned short *)(pbuf)) & (_FROM_DS_)) != 0)

static int get_rtheader_len(unsigned char *buf, size_t len)
{
	struct ieee80211_radiotap_header *rt_header;
	unsigned short rt_header_size;

	rt_header = (struct ieee80211_radiotap_header *)buf;
	/* check the radiotap header can actually be present */
	if (len < sizeof(struct ieee80211_radiotap_header))
		return -1;
	 /* Linux only supports version 0 radiotap format */
	 if (rt_header->it_version)
		return -1;
	rt_header_size = le16toh(rt_header->it_len);
	 /* sanity check for allowed length and radiotap length field */
	if (len < rt_header_size)
		return -1;
	return rt_header_size;
}

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
//parse mac frame
static void ProcessPacket(unsigned char *buffer, int size, int rt_header_len)
{
	struct rx_frinfo frinfo;
	unsigned char type;
	unsigned char subtype;

	unsigned char da[6],sa[6];
	unsigned char index;
	static unsigned char rx_len,crc_8;
	unsigned char rx_buf[1024];
	static unsigned char rx_flag[1024];
	memcpy(da,buffer,6);																																											
//	memcpy(sa,buffer+6,6);
/*	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",da[0],da[1],da[2],da[3],da[4],da[5]);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",sa[0],sa[1],sa[2],sa[3],sa[4],sa[5]);*/

//	printf("da[4]:%d\n%c\n",da[4],da[5] );
	 /*解析包长以及CRC   格式： 226.58.CRC.length*/
	if((*da==1)&&(*(da+1)==0)&&(*(da+2)==94)&&(*(da+3)==58))
	{
		/*printf("get length data!\n");*/
		rx_len=da[5];
		crc_8=da[4];
		printf("get crc_8:%x\n",crc_8);
		printf("get rx_len:%d\n",rx_len);
	}
	//printf("get rx_len:%d\n",rx_len);
	if((*da==1)&&(*(da+1)==0)&&(*(da+2)==94)&&(*(da+3)==56))
	{
		index=da[4];
		rx_buf[index]=da[5];
		if(index==rx_len-2 && CRC8(rx_buf,rx_len)==crc_8)
		{
			/*CRC8(rx_buf,re_len);*/
			/*printf("recv_crc:%x\n",CRC8(rx_buf,(int)rx_len));*/

			rx_buf[rx_len-1]=0;
			printf("recvfrom the client : %s\n",rx_buf);
			/*printf("into rx_buf demo!the rx_len:%d\n",rx_len);*/
		}

		/*printf("index=:%d\n",index );
		printf("rx_buf:%c\n",rx_buf[index]);*/
	}

	//printf("%02x:%02x:%02x:%02x:%02x:%02x\n",da[0],da[1],da[2],da[3],da[4],da[5]);	

	//printf("stage 1\n");
	/*frinfo.pframe = buffer + rt_header_len; //在monitor模式得到的RAW包，要偏移rt_header_len个字节，以此得到MAC帧头
	type = *(frinfo.pframe) & TYPE_MASK;
	subtype = (*(frinfo.pframe) & SUBTYPE_MASK) >> 4;*/
//printf("stage 2\n");
//	frinfo.pktlen = size - rt_header_len;
//	if ((type != TYPE_DATA)/* || (frinfo.pktlen < 50)*/)
//		return;
/*	frinfo.to_fr_ds = *(frinfo.pframe + 1) & FRTODS_MASK;
	if (frinfo.to_fr_ds == 1) 
	{
		frinfo.sa = GetAddr2Ptr(frinfo.pframe);
		frinfo.da = GetAddr3Ptr(frinfo.pframe);
	} 
	else if (frinfo.to_fr_ds == 2) 
	{
		frinfo.sa = GetAddr3Ptr(frinfo.pframe);
		frinfo.da = GetAddr1Ptr(frinfo.pframe);
	}*/
	
//	printf("stage 3\n");
 //ethernet_t *ethhead = (ethernet_t*)buffer;
/*	unsigned char da[6],sa[6];
	memcpy(da,buffer,6);
	memcpy(sa,buffer+6,6);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",da[0],da[1],da[2],da[3],da[4],da[5]);
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",sa[0],sa[1],sa[2],sa[3],sa[4],sa[5]);*/
//	printf("buffer:%02x:%02x:%02x:%02x:%02x:%02x:\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
/*	if ((!frinfo.da) || (!frinfo.sa))
		return ;*/
	/*	SimpleConfigV1 -- Multicast packets	*/
/*	printf("into process packet\n");
	if (frinfo.da[0] == DEST_MAC0 && frinfo.da[1] == DEST_MAC1 && frinfo.da[2] == DEST_MAC2 && frinfo.da[3] == 0x38)
	{
	    printf("\n===>[%d/%d]%02x %02x %02x\n", rt_header_len, size, frinfo.da[3], frinfo.da[4], frinfo.da[5]);
  	}*/
	
}

void doRecvfrom()
{

	int NumTotalPkts, k;
	int sockopt, sockfd;
	uint8_t buf[BUF_SIZ];
	ssize_t numbytes;
	struct ifreq ifopts;	/* set promiscuous mode */
	int rt_header_len = 0;
	//sockaddr_ll addr;

	//Create a raw socket that shall sniff
	/* Open PF_PACKET socket, listening for EtherType ETHER_TYPE */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) //ipv4，原始socket，接受所有的数据帧
	{
		printf("listener: socket"); 
		return ;
	}
	/* Set interface to promiscuous mode - do we need to do this every time? */
	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);						//设置混杂模式
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);								//通过ioctl与内核交互，得到网络接口信息

	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
	/* Allow the socket to be reused - incase connection is closed prematurely */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt) == -1) 
	{
		printf("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	/* Bind to device */
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMSIZ-1) == -1)	
	{
		printf("SO_BINDTODEVICE");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	printf("all ready!\n");
	NumTotalPkts=0;
	while(1)
	{
		numbytes = recvfrom(sockfd, buf, BUF_SIZ, 0, NULL, NULL);
		//printf("%d\n",numbytes);
		if (numbytes > 1500)
			continue;
		if (numbytes <= 0)
		{
		    perror("recvfrom");
		    continue;
		}
		NumTotalPkts++;
		//printf("%d\n",NumTotalPkts );
/*		rt_header_len = get_rtheader_len(buf, (size_t)numbytes);
		printf("%d\n",rt_header_len );
		if (rt_header_len < 1)
		{
		    continue;
	    }*/
		//printf("ready to into ProcessPacket\n");
		ProcessPacket(buf, numbytes, rt_header_len);
	}
	
	close(sockfd);
	printf("Finished");
    return;
}



int main()
{
/*    char cmdstr[256];
	sprintf(cmdstr,"iwconfig %s mode monitor\n",ifName);
	system(cmdstr);
	sprintf(cmdstr,"echo \"%s 0 0\" > /proc/net/rtl8188eu/wlan0/monitor\n", SNIFFER_CH);
	system(cmdstr);	*/

	doRecvfrom();

/*	sprintf(cmdstr,"iwconfig %s mode auto\n",ifName);
	system(cmdstr);*/
	return 0;
}




