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
	int i, seq_size;
	pid_t pid;
	int segment_id;
	shared_data* shared_memory;
	if (argc != 2) {
		fprintf(stderr, "Usage: ./shm-fib <sequence size>\n");
		return -1;
	}
	seq_size = atoi(argv[1]);
	if (seq_size > MAX_SEQUENCE) {
		fprintf(stderr, "sequence size must be < %d\n", MAX_SEQUENCE);
		return -1;
	}
	if ((segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), PERM)) == -1) { //(见注1)
		fprintf(stderr, "Unable to create shared memory segment\n");
		return 1;
	}
	printf("Created shared memory segment %d\n", segment_id);
	if ((shared_memory = (shared_data *)shamt(segment_id, 0, 0)) == (shared_data *) - 1){
		fprintf(stderr, "Unable to create shared memory segment\n");
		return 1;
	}
	shared_memory->sequence_size = seq_size;
	if ((pid = fork()) == (pid_t)-1) {
		return 1;
	}
	if (pid == 0) {
		printf("CHILD: shared memory attached at address %p\n", shared_memory);
		shared_memory->fib_sequence[0] = 0;
		shared_memory->fib_sequence[1] = 1;
		for (int i = 2; i < shared_memory->sequence_size; i++) {
			shared_memory->fib_sequence[i] = shared_memory->fib_sequence[i - 2] + 
				shared_memory->fib_sequence[i - 1];
			shmdt((void *)shared_memory);
		}
	}
	else {
		wait(NULL);
		for (i = 0; i < shared_memory->sequence_size; i++) {
			printf("PARENT:  %d:%ld", i, shared_memory->fib_sequence[i]);
		}
		shmctl(segment_id, IPC_RMID, NULL);
	}
	return 0;
}
