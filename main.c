#include <pthread.h>

typedef struct chan {
	pthread_mutex_t mu;
	pthread_cond_t sent, ready;

	void *data;
	int received
	int has_receiver;
} unbuffered_t;

void send(unbuffered_t *chan, void *data) {
	pthread_mutex_lock(&chan->mu);

	chan->data = data;

	// blocks
	while (chan->has_receiver != 1) {
		pthread_cond_wait(&chan->ready, &chan->mu);
	}

	chan->received = 0;
	pthread_cond_signal(&chan->ready);

	while (chan->received != 1) {
		pthread_cond_wait(&chan->sent, &mu);
	}

	chan->data = NULL;
	pthread_mutex_unlock(&chan->mu);
}

void *recv(unbuffered_t *chan) {
	// lock
	pthread_mutex_lock(&chan->mu);

	chan->has_receiver = true;

	// unblocks
	pthread_cond_signal(&chan->ready);
	
	while (chan->data == NULL) {
		pthread_cond_wait(&chan->ready, &chan->mu);
	}
	
	int ret = *(chan->data);

	chan->has_receiver = 0;
	chan->received = 1;
	pthread_cond_signal(&chan->sent);

	pthread_mutex_unlock(&chan->mu);
	// unlock
	return ret;
}

int main()
{
	unbuffered_t *xch = {
        	.mu = PTHREAD_MUTEX_INITIALIZER;
        	.sent = PTHREAD_COND_INITIALIZER;
		.ready = PTHREAD_COND_INITIALIZER;

        	.data = NULL;
        	.has_receiver = 0;
        	.received = 0;
	};
	
	send(xch, (void*)1);		// xch <- 1

	int x = (int)recv(xch);		// x <- xch

	printf("%d\n", x);

	return 0;
}
