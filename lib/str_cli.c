#include	"unp.h"

// str_cli函数， 客户处理循环
void str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	// Fgets 读入一行文本
	// 键入 EOF 字符时，fgets返回一个空指针，于是 str_cli 函数返回
	while ( Fgets(sendline, MAXLINE, fp) != NULL ) {

		// Writen 把该行发送给服务器
		Writen(sockfd, sendline, strlen(sendline));

		// Readline 从服务器读入回射行
		if (Readline(sockfd, recvline, MAXLINE) == 0) {
			// prematurely: 过早地
			err_quit("str_cli: server terminated prematurely");
		} else {
			printf("readline 从服务器读入回射行");
		}
			
		// fputs 把它写到标准输出
		Fputs(recvline, stdout);
	}
}
