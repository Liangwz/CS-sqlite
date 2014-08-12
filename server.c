#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
//#include <glib.h>

#define SERVPORT 5000 /*服务器监听端口号 */
#define BACKLOG 10 /* 最大同时连接请求数 */



void cmd_run_info(char *Cmd, char *InfBuf)
{
	FILE *fp = NULL;  
	char *pter = NULL; 
	char tmpbuf[30] = "";
	if (NULL == InfBuf)
	{
		printf("fun:cmd_run_info error-->InfBuf is not exit\n");
		return ;
	}
	
	fp = popen(Cmd, "r");
	memset(tmpbuf,0x00,sizeof(tmpbuf));
	memset(InfBuf,0x00,sizeof(InfBuf));
	while (!feof(fp)) {
		pter = fgets(tmpbuf, sizeof(tmpbuf), fp);
		strcat(InfBuf,tmpbuf);
		memset(tmpbuf,0x00,sizeof(tmpbuf));	
	} 
	pclose(fp);
}


int main()
{
	int sockfd,client_fd; /*sock_fd：监听socket；client_fd：数据传输socket */
	struct sockaddr_in my_addr; /* 本机地址信息 */
	struct sockaddr_in remote_addr; /* 客户端地址信息 */
	char send_buf[100];
	char tmp_buf[50];
	int sin_size;
	char rcv_buf[100] = {0};
	int i;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket创建出错！\n"); exit(1);
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind出错！\n");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen出错！\n");
		exit(1);
	}
	
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, (socklen_t*)&sin_size)) == -1)
		{
			perror("accept出错\n");
			continue;
		}
		printf("received a connection from ip:%s \n", inet_ntoa(remote_addr.sin_addr));
		printf("received a connection from port:%d \n", ntohs(remote_addr.sin_port));
		if (!fork())
		{ /* 子进程代码段 */
			cmd_run_info("date +%Y%m%d",send_buf);//getch the time information
			cmd_run_info("date +%H:%M:%S",tmp_buf);//getch the time information
			memcpy(send_buf+strlen(send_buf)-1, " ", sizeof(" "));//remove the '\n'
			memcpy(send_buf+strlen(send_buf), tmp_buf, strlen(tmp_buf)-1);//remove the '\n'
			printf("show me the date feedback:%s\n",send_buf);
			if (send(client_fd, send_buf, strlen(send_buf)+1, 0) == -1)
			{
				perror("send Error!\n");
			}
// 			if (send(client_fd, "Hello, you are connected! \n", 28, 0) == -1)
// 			{
// 				perror("send出错！\n");
// 			}
			memset(send_buf,0x00,sizeof(send_buf));
			close(client_fd);
			exit(0);
		}
		
 		if (!fork())
		{ 
			/* 子进程代码段 */
			if (recv(client_fd, rcv_buf, 300, 0) == -1)
			{
				perror("recv Error!");			
			}
			else
			{
				printf("recv:%s\n",rcv_buf);
				memset(rcv_buf,0x00,sizeof(rcv_buf));
			}			
			close(client_fd);
			exit(0);
		}	
			close(client_fd);
	}

}
