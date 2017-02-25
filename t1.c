#include <omp.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sys/stat.h>
#include <pthread.h>

#define NTHREADS 4

_Atomic int _glbl_id=0;
int fixt()
{
    int id=-1;
    do {
        id = atomic_load(&_glbl_id);
    } while(!atomic_compare_exchange_weak(&_glbl_id, &id, id+1));
    return id;

}

int not_fixt()
{
    int ret=-1,id;
    //what possible could go wrong?
    id = atomic_load(&_glbl_id);
    ret = id;
    id += 1;
    atomic_store(&_glbl_id, id);
    return ret;
}

void *run_fixt(void* p_iters)
{
    int* iters = (int*) p_iters;
    volatile int uv = 0;
    for (int i=0; i<*iters; ++i)
    {
        uv = fixt();
    }
    printf("End result is %d\n", uv);

    return NULL;
}

void *run_not_fixt(void* p_iters)
{
    int* iters = (int*) p_iters;
    volatile int uv = 0;
    for (int i=0; i<*iters; ++i)
    {
        uv = not_fixt();
    }
    printf("End result is %d\n", uv);

    return NULL;
}

int get_uniq() {
    int retval;
    retval = atomic_load(&_glbl_id);
    return retval;
}

void reset_uniq() {
    atomic_store(&_glbl_id, 0);
}

int main()
{
    pthread_t fixt_threads[NTHREADS];
    pthread_t unfixg_threads[NTHREADS];

    int n_iters = 1000000;
    int current_value;

    for (int i=0; i<NTHREADS; i++)
    {
        pthread_create(&fixt_threads[i], NULL, run_fixt, &n_iters);
    }

    for (int i=0; i<NTHREADS; i++)
    {
        pthread_join(fixt_threads[i], NULL);
    }

    current_value = get_uniq();
    printf("Fixt value: %d\n", current_value);
    reset_uniq();

    for (int i=0; i<NTHREADS; i++)
    {
        pthread_create(&fixt_threads[i], NULL, run_not_fixt, &n_iters);
    }

    for (int i=0; i<NTHREADS; i++)
    {
        pthread_join(fixt_threads[i], NULL);
    }

    current_value = get_uniq();
    printf("Not Fixt value: %d\n", current_value);
}
