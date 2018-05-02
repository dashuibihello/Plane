#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 20

int Fibo_arr[MAX_SIZE];
void* Fibo_fun(void *param);

int main(int argc, char *argv[]) {
    pthread_t tid;
    pthread_attr_t attr;
    if(argc != 2) {
        fprintf(stderr, "usage:main<integer value>\n");
        return -1;
    }
    if(atoi(argv[1]) < 0 || atoi(argv[1]) > 20) {
        fprintf(stderr,"%d must be >= 0 and <= 20\n",atoi(argv[1]));
        return -1;
    }
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, Fibo_fun, argv[1]);
    pthread_join(tid, NULL);
    for(int i = 0; i < atoi(argv[1]); i++) {
        printf("Fiboncci(%d): %d\n", i, Fibo_arr[i]);
    }
}

void* Fibo_fun(void *param) {
    int upper = atoi(param);
    Fibo_arr[0] = 0;
    Fibo_arr[1] = 1;
    for(int i = 2; i < upper; i++) {
        Fibo_arr[i] = Fibo_arr[i - 1] + Fibo_arr[i - 2];
    }
    pthread_exit(0);
} 

