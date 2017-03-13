#include	"unp.h"

// 调用 wait 的 SIGCHLD 信号处理函数
// 必须在 fork 第一个子进程之前完成，且只做一次
void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	// wait 调用，获取子进程的PID和终止状态
	// 防止子进程变成僵死进程
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}
