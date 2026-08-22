#include <pthread.h>

typedef struct chan {
	pthread_mutex_t lock;
	pthread_cond_t send_done, recv_ready;

	void *data;
	int done;
	int listening;
} unbuffered_t;

void send(unbuffered_t *chan, void *data) {
	// lock
	pthread_mutex_lock(&chan->lock);

	chan->data = data;

	while (!chan->listening) {
		pthread_cond_wait(&chan->revc_ready, &chan->lock);
	}

	chan->done = 0;
	pthread_cond_signal(&chan->revc_ready);

	while (!chan->done) {
		pthread_cond_wait(&chan->send_done, &lock);
	}

	chan->data = NULL;
	pthread_mutex_unlock(&chan->lock);
	// unlock
}

// can I use <- for this
void *recv(unbuffered_t *chan) {
	// lock
	pthread_mutex_lock(&chan->lock);
	

	pthread_mutex_unlock(&chan->lock);
	// unlock
	return;
}

int main()
{
	unbuffered_t *xch = {
        	.lock = ;
        	.send_done = ;
		.recv_ready = ;
        	.data = NULL;
        	.done = 0;
        	.listening = 0;
	};
	
	send(xch, (void*)1);		// xch <- 1

	int x = (int)recv(xch);		// x <- xch

	printf("%d\n", x);

	return 0;
}
