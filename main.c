#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct chan {
	pthread_mutex_t mu;
	pthread_cond_t sent, ready;

	void *data;
	int received;
	int has_receiver;
} unbuffered_t;

void send(unbuffered_t *chan, void *data) {
	pthread_mutex_lock(&chan->mu);

	chan->data = data;
	printf("ch <- %d\n", *((int*)chan->data));

	// blocks
	while (chan->has_receiver != 1) {
		pthread_cond_wait(&chan->ready, &chan->mu);
	}

	chan->received = 0;
	pthread_cond_signal(&chan->ready);

	while (chan->received != 1) {
		pthread_cond_wait(&chan->sent, &chan->mu);
	}

	chan->data = NULL;
	pthread_mutex_unlock(&chan->mu);
}

void *recv(unbuffered_t *chan) {
	pthread_mutex_lock(&chan->mu);

	chan->has_receiver = true;

	pthread_cond_signal(&chan->ready);
	
	while (chan->data == NULL) {
		pthread_cond_wait(&chan->ready, &chan->mu);
	}
	
	void *ret = chan->data;
	printf("%d <- ch\n", *((int*)ret));

	chan->has_receiver = 0;
	chan->received = 1;
	pthread_cond_signal(&chan->sent);

	pthread_mutex_unlock(&chan->mu);
	return ret;
}

void *sender(void *arg)
{
	unbuffered_t *chan = (unbuffered_t *)arg;
	int *data = (int*)malloc(sizeof(int));
	*data = 42;
	send(chan, (void*)data);			// xch <- 1
	return NULL;
}

void *receiver(void *chan)
{
	void *data = (void *)recv(chan);		// x <- xch
	return data;
}

// (void*)(intptr_t)42; // or why computers are cool

int main()
{
	unbuffered_t ch = {
        	.mu = PTHREAD_MUTEX_INITIALIZER,
        	.sent = PTHREAD_COND_INITIALIZER,
		.ready = PTHREAD_COND_INITIALIZER,

        	.data = NULL,
        	.has_receiver = 0,
        	.received = 0,
	};
	
	pthread_t send_tid, recv_tid;

	pthread_create(&send_tid, NULL, sender, (void *)&ch);
	pthread_create(&recv_tid, NULL, receiver, (void *)&ch);
	
	int *data = NULL;
	pthread_join(send_tid, NULL);
	pthread_join(recv_tid, (void **)&data);

	printf("got: %d\n", *data);

	free(data);
	return 0;
}
