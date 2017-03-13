#include	"unp.h"
#include	<time.h>

int main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	// 1. socket(): 创建一个socket描述符
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// 2. bind(): 把服务器众所周知的端口捆绑到套接字
	// IP地址为INADDR_ANY, 服务器主机如果有多个网络接口，服务器进程就可以在任意网络接口上接受客户连接。
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port        = htons(13);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	// 3. listen() 把套接字转换成监听套接字
	// LISTENQ: 指定系统内核允许在这个监听描述符上排队的最大客户连接数。
	Listen(listenfd, LISTENQ);

	// 无限循环
	for ( ; ; ) {
		// 4. accept()。 进入睡眠，等待某个客户连接的到达并被内核接受
		// TCP 采用三次握手来建立连接，完毕后 accept 返回
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);

		// snprintf 要求第二个参数指定目的缓冲区的大小，可确保该缓冲区不溢出
		//不安全版本： sprintf, gets, strcat, strcpy
		//安全版本： snprintf, fgets, strncat/strlcat, strncpy/strlcpy
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		// Write 把结果字符串写给客户
        Write(connfd, buff, strlen(buff));

        // close() 终止连接, 每个方向上发送一个 FIN
		Close(connfd);
	}
}
