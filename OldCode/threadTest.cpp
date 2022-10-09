#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <algorithm>
//#include <wiringPi.h>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

void *PrintHello(void *threadid) {
   long tid;
   tid = (long)threadid;
   printf("Hello World! Thread ID, %d\n", tid);
   pthread_exit(NULL);
}

int main () {
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    for (i=0; i < NUM_THREADS; i++) {
        printf("Creating Thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);

        if (rc) {
            printf("Unable to create thread\n");
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
