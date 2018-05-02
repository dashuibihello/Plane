#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#define NUM_THREADS 10

#define M 10
#define N 10
#define K 10

int A[M][K];
int B[K][N];
int **C;

struct matrix
{
    int i;
    int j;
};

void *runner(void *d)
{
    struct matrix *data = (struct matrix*)d;
    for (int a = 0; a < K; a++)
    {
        C[data->i][data->j] += A[data->i][a] * B[a][data->j];
    }
    pthread_exit(0);
}

int main()
{
    int m,n,k;
    printf("Please input m,n,k.(Matrix A has m rows and k columns.Matrix B has k rows and n columns(m,n,k <= 10)\n");
    scanf("%d%d%d", &m, &n, &k);
    if(m > M || n > N || k > K) {
        fprintf(stderr, "m,n,k need be smaller than 10\n");
        return -1;
    }

    C = (int**)malloc(sizeof(int*)*m);
    for(int i = 0; i < m; i++) {
        C[i] = (int*)malloc(sizeof(int)*n);
    }

    printf("Please input Matrix A\n");
    for(int i = 0; i < m; i++) {
        printf("Please input the %d row of Matrix A: ", i);
        for(int j = 0; j < k; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Please input Matrix B\n");
    for(int i = 0; i < k; i++) {
        printf("Please input the %d row of Matrix B: ", i);
        for(int j = 0; j < n; j++) {
            scanf("%d", &B[i][j]);
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            printf("%3d ", A[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", B[i][j]);
        }
        printf("\n");
    }

    pthread_t workers[NUM_THREADS][NUM_THREADS];
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            struct matrix data;
            data.i = i;
            data.j = j;
            pthread_create(&workers[i][j], NULL, (void *)runner, (void *)&data);
            pthread_join(workers[i][j], NULL);
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", C[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < m; i++) {
        free(C[i]);
    }
    free(C);
    return 0;
}
