#include	"unp.h"

// TCP当前时间查询客户程序的一个实现
// 当该客户与其服务器建立一个 TCP 连接后，服务器以直观可读格式简单地送回当前时间和日期

int main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

  // 1. socket 函数创建一个网际(AF_INET)字节流(SOCK_STREAM)套接字
	// socket 函数返回一个描述符，用来标识该套接字
  // AF_INET 与 IPv4 协议相关
  // AF_INET 与 IPv6 协议相关
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

  // 2.指定服务器的ip地址和端口号
	bzero(&servaddr, sizeof(servaddr)); // bzero 比 memset 更好记忆
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(13);	/* daytime server , htons: 主机到网络短整数*/
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) // IP地址为第一个命令行的参数的值(argv[1])
		err_quit("inet_pton error for %s", argv[1]);

  // 3. 建立与服务器的连接  SA = struct sockaddr
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

  // 4. 读入并输出服务器的应答, 当 read 返回 0 表明对端关闭连接，返回负值时终止循环
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) // fputs输出结果
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

  // 5. exit 关闭该进程所有打开的描述符
	exit(0);
}
