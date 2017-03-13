#include	"unp.h"

// str_echo 函数，在套接字上回射数据
void str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	// 如果客户关闭连接，那么接收到客户的FIN将导致服务器子进程的read函数返回0
	// 导致 str_echo 函数的返回...从而终止子进程
	while ( (n = read(sockfd, buf, MAXLINE)) > 0) { // read 从套接字读入数据
		Writen(sockfd, buf, n); // writen函数把其中内容回射给客户。
		printf("writen函数把其中内容回射给客户。");
	}
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}
