#include	"unp.h"

/*
一、IO复用
 IO 复用：使得内核一旦发现进程指定的一个或多个I/O条件就绪
 (也就是说输入已准备好被读取，或者描述符已能承接更多的输出)，它就通知进程

 IO复用的典型应用...
 1> 当客户处理多个描述符时，必须使用IO复用
 2> 一个客户同时处理多个套接字，不过比较少见
 3> 如果一个TCP服务器既要处理监听套接字，又要处理已连接套接字
 4> 如果一个服务器既要处理TCP,又要处理UDP
 5> 如果一个服务器要处理多个服务或者多个协议

 使用select的优势在于我们可以等待多个描述符就绪

二、select 函数
 // 若有就绪描述符则为其数目，若超时则为0， 若出错则为-1
 int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, 
 			const struct timeval *timeout)

timeout参数的三种可能
1> 永远等待下去，参数设置为空指针
2> 等待一段固定时间
3> 根本不等待，参数设置为0
 
 void FD_ZERO(fd_set *fdset);
 void FD_SET(int fd, fd_set *fd_set);
 void FD_CLR(int fd, fd_set *fd_set);
 int FD_ISSET(int fd, fd_set *fdset);
 FD_SETSIZE 常值是数据类型 fd_set 中的描述符总数，其值通常是1024

 使用 select 时常见的两个编程错误：
 1> 忘了对最大描述符加1
 2> 忘了描述符集是 值-结果 参数

 描述符就绪条件
 （1）一个套接字准备好读
 1> 
 2> 该连接的读半部关闭(也就是接收了FIN的TCP连接)。对这样的套接字的读操作将不阻塞并返回0(返回EOF)
 3> 
 4>

 （2）套接字准备好写
 1> 
 2>
 3>
 4>
*/

// 使用 select 的 str_cli 函数的实现
void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;

	// fd_set  用于检查可读性的描述符集
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	// 初始化
	FD_ZERO(&rset);

	// 无限循环中...
	for ( ; ; ) {
		// fileno函数把标准IO文件指针转换为对应的描述符,select 和 poll 只工作在描述符上
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);

		// 计算较大值
		maxfdp1 = max(fileno(fp), sockfd) + 1;

		// 在这里，写集合和异常集合指针都是空指针
		// timeout 也是空指针
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		// select 返回时，套接字是可读的
		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}

		// 如果标准输入可读时...
		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
