#include	"unp.h"

/*
	根据给定名字来查找相应服务
	struct servent *getservbyname(const char *servname, const char *protoname)

	根据给定端口号和可选协议查找相应服务
	struct servent *getservbyport(int port, const char *protoname)
*/

struct about_servent {
	char *s_name; // official service name
	char **s_aliases; // alias list 
	int s_port; // 端口号, network byte order. 主要关心的字段，以网络字节返回，把它存放到套接字地址结构时绝对不能调用 htons
	char *s_proto; // protocal to use
};

// 使用 gethostbyname 和 getservbyname 的时间获取客户程序
// 尝试连接多宿服务器主机的每个IP地址，直到有一个连接成功或所有地址尝试完毕为止
int main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	struct in_addr		**pptr; // struct in_addr 用来表示一个32位的IPv4地址
	struct in_addr		*inetaddrp[2];
	struct in_addr		inetaddr;
	struct hostent		*hp;
	struct servent		*sp;

	if (argc != 3)
		err_quit("usage: daytimetcpcli1 <hostname> <service>");

	// gethostbyname, argv[1] 主机名 <hostname>
	if ( (hp = gethostbyname(argv[1])) == NULL) {
		// gethostbyname 查找失败，尝试使用 inet_aton 函数，确定其参数是否已是 ASCII 格式的地址
		// inet_aton 将一个字符串IP地址转换为一个32位的网络序列IP地址
		if (inet_aton(argv[1], &inetaddr) == 0) {
			err_quit("hostname error for %s: %s", argv[1], hstrerror(h_errno));
		} else { // 构造一个由相应地址构成的单元素列表
			inetaddrp[0] = &inetaddr;
			inetaddrp[1] = NULL;
			pptr = inetaddrp;
		}
	} else {
		// char **h_addr_list;
		// char ** 转换成 in_addr **
		pptr = (struct in_addr **) hp->h_addr_list;
	}

	// getservbyname, argv[2] 服务名 <service>
	if ( (sp = getservbyname(argv[2], "tcp")) == NULL)
		err_quit("getservbyname error for %s", argv[2]);

	for ( ; *pptr != NULL; pptr++) {
		// Socket
		sockfd = Socket(AF_INET, SOCK_STREAM, 0);

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = sp->s_port; // sp->s_port 获取服务器的端口号
		memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));

		// char* sock_ntop(const struct sockaddr* sa, socklen_t salen)
		printf("trying %s\n", Sock_ntop((SA *) &servadd./r, sizeof(servaddr)));

		// connect
		if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == 0)
			break;		/* success */ // 调用成功，终止循环
		err_ret("connect error");
		close(sockfd); // connect 调用失败，描述符必须关闭，不能再用
	}

	// 如果循环终止的原因是没有一个connect调用成功，那就终止程序运行。
	if (*pptr == NULL)
		err_quit("unable to connect");

	// 读取服务器的应答
	while ( (n = Read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	exit(0);
}
