#include <stdio.h>
#include <stdlib.h>
#include "lfq.h"
#include "threadpool.h"
#include <pthread.h>

void *print_without_num(){
    printf("%s\n","hello");
}
void print(int *a){
    printf("%s %d\n","hello",*a);
}

int main(){
	struct threadpool *thpool;
        thpool = make_new_threadpool(4);
        init(thpool);
        struct task *t1;
        t1=malloc(sizeof(struct task));
        int *a=malloc(sizeof(int));
        *a=5;

        t1->arg=NULL; //chk with variable 'a' also
        t1->function=print_without_num;
        int i;
        for (i = 0; i < 4; i++) {
            submit_to_worker_thread(thpool,t1);
        }
        sleep(6);
	
}
