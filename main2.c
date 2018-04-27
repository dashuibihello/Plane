#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PERM S_IRUSR|S_IWUSR
#define MAX_SEQUENCE 10

typedef struct {
	long fib_sequence[MAX_SEQUENCE];
	int sequence_size;
} shared_data;

int main(int argc, char **argv)
{
	int i = 0, seq_size, segment_id;

	//接受命令行上传递的参数，执行错误检查以保证参数不大于MAX_SEQUENCE
	pid_t pid;
	if (argc != 2) {
		fprintf(stderr, "Usage: ./shm-fib <sequence size>\n");
		exit(1);
	}
	seq_size = atoi(argv[1]);
	if (seq_size > MAX_SEQUENCE) {
		fprintf(stderr, "sequence size must be < %d\n", MAX_SEQUENCE);
		exit(1);
	}

	//创建一个大小为shared data 的共享内存段
	shared_data* shared_memory;
	
	// 将共享内存段附加到地址空间
	if ((segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), PERM)) == -1) {
		fprintf(stderr, "Unable to create shared memory segment\n");
		exit(1);
	}
	if ((shared_memory = (shared_data *)shmat(segment_id, 0, 0)) == (shared_data *)-1) {
		fprintf(stderr, "Unable to create shared memory segment\n");
		return 0;
	}
	
	//在命令行将命令行参数值赋予shared data。 
	shared_memory->sequence_size = seq_size;

	//创建子进程
	pid = fork();

	if (pid == 0) {
		shared_memory->fib_sequence[0] = 0;
		shared_memory->fib_sequence[1] = 1;
		for (i = 2; i < shared_memory->sequence_size; i++) {
			shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i - 2] +
				shared_memory->fib_sequence[i - 1];
		}
		shmdt((void *)shared_memory);
	}

	else {
		//调用系统调用wait等待子进程结束
		wait(NULL);

		//输出共享内存段中Fibonacci序列的值
		for (i = 0; i < shared_memory->sequence_size; i++) {
			printf("Parent: %d->%ld\n", i, shared_memory->fib_sequence[i]);
		}

		//释放并删除共享内存段
		shmctl(segment_id, IPC_RMID, NULL);
	}
	return 0;
}
