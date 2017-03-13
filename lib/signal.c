/* include signal */
#include	"unp.h"

/*
	signal原型:
	void ( *signal(int signo, void(*func)(int)) )(int);

	为了简化起见,定义 Sigfunc 类型
	typedef void Sigfunc(int)

	它说明信号处理函数是仅有一个整数参数且不返回值的函数。 

	signal的函数原型变为
	Sigfunc *signal(int signo, Sigfunc *func);
	


	第一个参数: 信号名
	第二个参数: 指向函数的指针
*/
Sigfunc *
signal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func; // 设置 sa_handler

	// sa_mask 设为 空集, 意味着在该信号处理函数运行期间，除了被捕获的信号外，不阻塞额外的信号。
	sigemptyset(&act.sa_mask); 
	act.sa_flags = 0;

	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;	/* SunOS 4.x */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;		/* SVR4, 44BSD */
#endif
	}

	// 调用 sigaction 函数
	// 对于大多数信号来说，调用 sigaction 函数并指定信号发生时所调用的函数就是捕获信号所需做的全部工作
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);

	//  并将相应信号的 oact.sa_handler 作为 signal 函数的返回值
	return(oact.sa_handler);
}
/* end signal */

Sigfunc *
Signal(int signo, Sigfunc *func)	/* for our signal() function */
{
	Sigfunc	*sigfunc;

	if ( (sigfunc = signal(signo, func)) == SIG_ERR)
		err_sys("signal error");
	return(sigfunc);
}
