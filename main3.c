#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>

#define MAX_LINE 80
#define BUFFER_SIZE 50
char buffer[BUFFER_SIZE];


void handle_SIGINT() {
	write(STDOUT_FILENO, buffer, strlen(buffer));
	exit(0);
}

void setup(char inputBuffer[], char *args[], int *background)
{
	int length, /* 命令的字符数目 */
		i,      /* 循环变量 */
		start,  /* 命令的第一个字符位置 */
		ct;     /* 下一个参数存入args[]的位置 */

	ct = 0;

	/* 读入命令行字符，存入inputBuffer */
	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

	start = -1;
	if (length == 0) exit(0);            /* 输入ctrl+d，结束shell程序 */
	if (length < 0) {
		perror("error reading the command");
		exit(-1);           /* 出错时用错误码-1结束shell */
	}
	/* 检查inputBuffer中的每一个字符 */
	for (i = 0; i<length; i++) {
		switch (inputBuffer[i]) {
		case ' ':
		case '\t':               /* 字符为分割参数的空格或制表符(tab)'\t'*/
			if (start != -1) {
				args[ct] = &inputBuffer[start];
				ct++;
			}
			inputBuffer[i] = '\0'; /* 设置 C string 的结束符 */
			start = -1;
			break;

		case '\n':                 /* 命令行结束 */
			if (start != -1) {
				args[ct] = &inputBuffer[start];
				ct++;
			}
			inputBuffer[i] = '\0';
			args[ct] = NULL; /* 命令及参数结束 */
			break;

		default:             /* 其他字符 */
			if (start == -1)
				start = i;
			if (inputBuffer[i] == '&') {
				*background = 1;          /*置命令在后台运行*/
				inputBuffer[i] = '\0';
			}
		}
	}
	for(i = 0; i < ct; i++) {
		cout << args[i] << endl;
	}
	args[ct] = NULL; /* 命令字符数 > 80 */
}

int main(void)
{
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	sigaction(SIGINT, &handler, NULL);

	strcpy(buffer,"Caught Control C\n");

	char inputBuffer[MAX_LINE]; /* 这个缓存用来存放输入的命令*/
	int background;             /* ==1时，表示在后台运行命令，即在命令后加上'&' */
	char *args[MAX_LINE / 2 + 1];/* 命令最多40个参数 */

	while (1) {            /* 程序在setup中正常结束*/
		background = 0;
		printf("COMMAND->"); //输出提示符，没有换行，仅将字符串送入输出缓存
		fflush(stdout);		 //若要输出输出缓存内容用fflush(stdout);头文件stdio.h
		setup(inputBuffer, args, &background);       /* 获取下一个输入的命令 */
		pid_t pid;
		pid = fork();
		if(pid == 0) {
			execvp(args[0], args);
		}
		else{
			if(background == 0) {
				wait(0);
			}
		}
	}
}
