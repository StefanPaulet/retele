//
// Created by stefan on 18.11.2022.

#include <CommonLibs>

pthread_mutex_t myLock;
int counter;
void * thread_main ( void * threadNr ) {
    pthread_mutex_lock ( & myLock );

    counter ++;
    printf( "%d\n", counter );

    pthread_mutex_unlock ( & myLock );

    pthread_cancel ( pthread_self() );
}

int main () {
    pthread_mutex_init ( &myLock, NULL );
    pthread_t tid [ 5 ];
    for ( int i = 0; i < 5; ++ i ) {
        pthread_create ( & tid[i], NULL, & thread_main, ( void * ) ( & i ) );
//        pthread_join ( newThread, NULL );
    }
    for ( int i = 0; i < 5; ++ i )
        pthread_join ( tid[i], NULL );
    pthread_mutex_destroy ( & myLock );
}
