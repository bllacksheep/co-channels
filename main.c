#include <pthread.h>

typedef struct chan {
	pthread_mutex_t lock;
	pthread_cond_t send_done, recv_ready;

	void *data;
	char has_data;
} unbuffered_t;


void send(unbuffered_t *chan, void *data) {

}

// can I use <- for this
void *receive(unbuffered_t *chan) {

	return;
}

int main()
{
	unbuffered_t *xch;
	
	send(xch, (void*)1);		// xch <- 1

	int x = (int)receive(xch);	// x <- xch

	printf("%d\n", x);

	return 0;
}
