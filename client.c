
/*
#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
*/
/*
功能：用于测试向服务器端发送心跳包
当前：
1.客户端
a.完成每3秒向服务器发送包裹；
b.开一线程监听端口那边传来的数据。
c.绑定554端口(rtsp视频要求端口)

2.服务器
a.监听5000端口传过来的数据

使用：
1、客户端
./client 192.168.2.220 5000
客户端自定义向对于ip和端口的服务器
2、服务器
./serve		
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/time.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<sys/ioctl.h>
#include	<net/ethernet.h>
#include	<net/if.h>
#include	<net/if_arp.h>
#include	<signal.h>
#include	<netinet/ip.h>
#include	<sys/wait.h>
#include	<dirent.h>
#include	<errno.h>

#include <pthread.h>
#include "sqlite_main.h"

#define SERVPORT 8000
//#define MAXDATASIZE 100 /*每次最大数据传输量 */
#define		SERIAL				"3001101E0009060300001ABC"//摄像机序列号
#define		LOCAL_IP			"192.168.2.101"				//摄像机IP
unsigned int				LOCAL_PORT=10086;					//摄像机端口
int				WAN_PORT=10086;											//外网端口（映射端口）
#define		HTTP_MAX_RECV_BUFF		10240
#define 	LOCAL_NETMASK          "255.255.255.0"		//子网掩码
#define		VERSION								"AR003E_090109_CN"	//版本
#define		MAXVERSION						24									//版本长度
#define		MAXSN									24									//序列号长度
#define		MAXIP									16
#define		MAXDATASIZE			100
#define		CTL_SERVER_IP		"192.168.2.11"//""


//the process use to listen the socket
/*******************************************
 函数：void *lis_procss(void *arg)
 功能：监听socket端口
 输入：
 输出：
修改人：
版本号：
*******************************************/
void *lis_procss(void *arg);

//the process use to send the heartpackage
/*******************************************
 函数：void *thread_heartpackage_procss(void *arg)
 功能：向服务器socket端口发送心跳包10秒一次
 输入：
 输出：
修改人：
版本号：
*******************************************/
void *thread_heartpackage_procss(void *arg);

char	send_buf_heart[MAXDATASIZE];
char	g_recv_buf[100];

/*
*   组装心跳包
*/
void fill_heart(char *buf,int len,char *ip,int port,int server_port,char *serial,char *submask)
{
	char begin[256];
	char *end;
	static struct sockaddr_in addr;
	static struct sockaddr_in sub;
//	printf("ip=%s,port=%d,server_port=%d,serial=%s\n",ip,port,server_port,serial);
	addr.sin_addr.s_addr = inet_addr(ip);
	sub.sin_addr.s_addr = inet_addr(submask);
	memset(begin,0x00,sizeof(begin));
	begin[4]=4;
	begin[6]=56+MAXIP;
	begin[0]=begin[6]+4;                     //总长度 
	begin[8]=17;
	end = &begin[9];
	memcpy(end,serial,strlen(serial));     //填充系列号
	end += MAXSN;
	memcpy(end,(char *)&addr.sin_addr.s_addr,4);
	end += 4;
	//test
	memcpy(end,(char *)&sub.sin_addr.s_addr,4);
	printf("the end is :%x\n",sub.sin_addr.s_addr);
	end += 4;          

	memcpy(end,(char *)&port,2);     //web端口
	end += 2;

	memcpy(end,VERSION,MAXVERSION);  //版本信息
	end += MAXVERSION;
	memcpy(buf,begin,len);
}



/*****************************
功能：初始化一个socket
输入：无
返回：client_socket
*******************************/
int socket_client_prepare(char *ip, int port)
{
	int socketfd;
	struct sockaddr_in ser_addr;
	
	int addr_len = sizeof(ser_addr);
	//打开socketfd获得一个文件描述符ipv4协议数据流类型，无协议
	//int socketfd(int domain, int type, int protocol);
	
	
	if ((socketfd = socket(PF_INET,SOCK_STREAM,0)) <  0) 
	{
		perror("socket Create Error!\n");
		exit(1);
	}
	ser_addr.sin_family = PF_INET;
// 	ser_addr.sin_port   = htons(tempIP->port);
// 	ser_addr.sin_addr.s_addr = inet_addr(tempIP->ip);
	ser_addr.sin_port   = htons(port);
	ser_addr.sin_addr.s_addr = inet_addr(ip);
	
	if (connect(socketfd,(struct sockaddr *)&ser_addr,addr_len) < 0)
	{
		perror("socket connect Error!\n");
		exit(1);
	}
	return socketfd;
}






int main(int argc, char *argv[])
{
	char heartpackage[]="heartpackage";
	int opt,len_opt;
	pthread_t	rcv_thread, send_thread;
	int iTimes = 0;
	struct timeval t1,t2;
	int sockfd;
	struct hostent *host;
	struct sockaddr_in serv_addr ,clie_addr;
	int send_size;
	char cDbRcv[300] = {0};
	
	if (argc < 3)
	{
		fprintf(stderr,"Please enter the server's hostname and port like :192.X 5000! \n");
		exit(1);
	}
//	fill_heart(send_buf_heart,100,LOCAL_IP,LOCAL_PORT,WAN_PORT,SERIAL,LOCAL_NETMASK);	
	strcpy(send_buf_heart, "Time require");
	if((host=gethostbyname(argv[1]))==NULL) 
	{
		herror("gethostbyname Error\n");
		exit(1);
	}	
	printf("show the argv 1:%s 2:%d\n",argv[1],atoi(argv[2]));
// 	printf("before\n");
// 	printf("after\n");
 	sockfd = socket_client_prepare(argv[1],atoi(argv[2]));


// 	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
// 	{
// 		perror("socket Create Error!\n");
// 		exit(1);
// 	}
// read_sqlite_data(NULL,NULL,cDbRcv);
// printf("client rcv:%s\n",cDbRcv);
// 	//端口复用SO_REUSEADDR
// 	opt = 1;
// 	len_opt = sizeof(opt);
// 	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, len_opt);
// 
// 	serv_addr.sin_family=AF_INET;
// 	printf("set the port\n");
// 	serv_addr.sin_port=htons((u_short)atoi(argv[2]));
// 	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
// 	bzero(&(serv_addr.sin_zero),8);
// 		
// //bind
// 	clie_addr.sin_family=AF_INET;
// 	clie_addr.sin_port=htons(554);
// 	clie_addr.sin_addr.s_addr = INADDR_ANY;
// 	bzero(&(clie_addr.sin_zero),8);
// 	if (bind(sockfd, (struct sockaddr *)&clie_addr, sizeof(struct sockaddr)) == -1)
// 	{
// 		perror("bind  Error\n");
// 		exit(1);
// 	}
// //bind	
// 	if (connect(sockfd, (struct sockaddr *)&serv_addr, 
// 	sizeof(struct sockaddr)) == -1)
// 	{
// 		perror("connect Error!\n");
// 		exit(1);
// 	}


// 	if( pthread_create(&rcv_thread, NULL, lis_procss, (void *)&sockfd) != 0)
// 	{
// 		printf("creat b_thread failed!\n");					
// 	}
// 	
// 	if( pthread_create(&send_thread, NULL, thread_heartpackage_procss, (void *)&sockfd) != 0)
// 	{
// 		printf("creat b_thread failed!\n");					
// 	}	

			if( pthread_create(&send_thread, NULL, thread_heartpackage_procss, NULL) != 0)
			{
				printf("creat b_thread failed!\n");					
			}	
			if( pthread_create(&rcv_thread, NULL, lis_procss, NULL) != 0)
			{
				printf("creat b_thread failed!\n");					
			}


//	if ((send_size = send(sockfd, send_buf_heart, strlen(send_buf_heart)+1, 0)) == -1)
//	{
//		perror("send出错！");	
//	}	
// 	if ((send_size = send(sockfd, heartpackage, strlen(heartpackage)+1, 0)) == -1)
// 	{
// 		perror("send出错！");	
// 	}
// read_sqlite_data(NULL,NULL,cDbRcv);
read_sqlite_data("Hello","hello",cDbRcv);
	if ((send_size = send(sockfd, cDbRcv, strlen(cDbRcv)+1, 0)) == -1)
	{
		perror("send出错！");	
	}
// 	gettimeofday(&t2, NULL); 	
// 	t1 = t2;
// 	while(1)
// 	{
// 		gettimeofday(&t2, NULL); 	
// 		if(1 	==	(t2.tv_sec - t1.tv_sec) )
// 		{
// 			iTimes++;
// 			t1 = t2;	
// 			printf("now i will send:%s\n",send_buf_heart);			
// 			if ((send_size = send(sockfd, send_buf_heart, strlen(send_buf_heart)+1, 0)) == -1)
// 			{
// 				perror("send出错！");	
// 			}	
// 		}		
// 		if(iTimes == 10)
// 		{
// 			close(sockfd);		
// 			break;			
// 		}
// 	}	
		if(pthread_join(rcv_thread, 0) != 0)
		{
			printf("join b_thread failed!\n");							
		}	
		if(pthread_join(send_thread, 0) != 0)
		{
			printf("join b_thread failed!\n");							
		}		
	close(sockfd);
	return 0;
}

//the process use to listen the socket
/*******************************************
 函数：void *lis_procss(void *arg)
 功能：监听socket端口
 输入：
 输出：
修改人：
版本号：
*******************************************/
// void *lis_procss(void *arg)
// {
// 	int recv_size;
// //	char rcv_buf[100] ;
// 	int sockfd_sever = *(int *)arg;
// 	memset(g_recv_buf,0x00,sizeof(g_recv_buf));
// // 	while(1)
// // 	{
// 		if ((recv_size=recv(sockfd_sever, g_recv_buf, 72, 0)) ==-1)
// 		{
// 			perror("recv Error!\n");
// 		}		
// 		else
// 		{
// //			sleep(1);
// 			printf("here recv buf is:%s\n",g_recv_buf);			
// 		}
// //	}
// }
void *lis_procss(void *arg)
{
// 	int recv_size,i;
// //	char rcv_buf[100] ;
// // 	int sockfd_sever = *(int *)arg;
// 
// 
// 	for (i=0; i<10; i++)
// 	{
// 		int sockfd_sever = socket_client_prepare("127.0.0.1",5000);
// 		printf("Now will send package!\n");
// 		if (send(sockfd_sever, "heartpackage", strlen("heartpackage")+1, 0) == -1)
// 		{
// 			perror("send Eroor!\n");
// 		}
// 		sleep(1);
// 		close(sockfd_sever);
// 	}


	int recv_size;
	while(1)
	{
		int sockfd_sever = socket_client_prepare("127.0.0.1",5000);
		memset(g_recv_buf,0x00,sizeof(g_recv_buf));
		
		if ((recv_size=recv(sockfd_sever, g_recv_buf, 72, 0)) ==-1)
		{
			perror("recv Error!\n");
		}		
		else
		{
			printf("here recv buf is:%s\n",g_recv_buf);			
		}
		sleep(1);
		close(sockfd_sever);
	}

}

//the process use to send the heartpackage
/*******************************************
 函数：void *thread_heartpackage_procss(void *arg)
 功能：向服务器socket端口发送心跳包10秒一次
 输入：
 输出：
修改人：
版本号：
*******************************************/
void *thread_heartpackage_procss(void *arg)
{
	int recv_size,i;
//	char rcv_buf[100] ;
// 	int sockfd_sever = *(int *)arg;


	for (i=0; i<10; i++)
	{
		int sockfd_sever = socket_client_prepare("127.0.0.1",5000);
		printf("Now will send package!\n");
		if (send(sockfd_sever, "heartpackage", strlen("heartpackage")+1, 0) == -1)
		{
			perror("send Eroor!\n");
		}
		sleep(1);
		close(sockfd_sever);
	}

}



