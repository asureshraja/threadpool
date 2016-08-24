struct threadpool{
        int num_of_threads;
        int worker_epfd;
        struct lfq *q;
        pthread_mutex_t th_mutex;
	pthread_cond_t th_cond;
};

struct task{
    void (*function)();
    void * arg;
};
void* worker_thread_work(void *thpool);
void init(struct threadpool *thpool);
void submit_to_worker_thread(struct threadpool *thpool,struct task *arg);
struct threadpool* make_new_threadpool(int no_of_threads){
        struct threadpool *thpool;
        thpool=malloc(sizeof(struct threadpool));
        thpool->num_of_threads=no_of_threads;
        thpool->q=make_new_lfq(100);
	pthread_mutex_init ( &thpool->th_mutex, NULL);
	pthread_cond_init(&thpool->th_cond,NULL);
	return thpool;
}
void* worker_thread_work(void *arg) {
	struct threadpool *thpool = (struct threadpool *)arg;
        int i,k=-1;
        struct task *t1;
        while (1) {
		pthread_mutex_lock(&(thpool->th_mutex));
		pthread_cond_wait(&thpool->th_cond, &thpool->th_mutex);
		pthread_mutex_unlock(&(thpool->th_mutex));
		while ((k = dequeue(thpool->q,&t1)) != -1) {
			if (t1->arg==NULL) {
				t1->function();
			}else{
				t1->function(t1->arg);
			}
		}
                
        }
}

void init(struct threadpool *thpool){
        pthread_t threads[thpool->num_of_threads];
        int i;
        for (i = 0; i < thpool->num_of_threads; i++) {
                pthread_create(&threads[i], NULL, worker_thread_work, thpool);
        }
}

void submit_to_worker_thread(struct threadpool *thpool,struct task *arg) {
        enqueue(thpool->q, arg);
	pthread_mutex_lock(&(thpool->th_mutex));
	pthread_cond_signal(&(thpool->th_cond));
	pthread_mutex_unlock(&(thpool->th_mutex));
}

