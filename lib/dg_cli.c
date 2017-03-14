#include	"unp.h"

//  dg_cli函数: 客户处理循环
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		// 对于一个UDP套接字，如果其进程首次调用 sendto 时它没有绑定一个本地端口，那么内核就在此时为它选择一个临时端口
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		// 第五和第六个参数是NULL
		// 这告知内核我们并不关心应答数据报由谁发送。这将存在风险
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}
