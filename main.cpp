#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <array>
#include <mutex>

void * threadMain ( void * param ) {

    std :: array < std :: string, 3 > a {
        "abfasdfasdfasdfasdfasdfasdfasdfasdc",
        "def",
        "ghi"
    };
    for ( int i = 0; i < 10; ++ i ) {
        printf ( "\0337" );
        switch ( i % 3 ) {
            case 0: {
                printf ( "\033[4F%s\033[4E\0338", a [ i % 3].c_str());
                fflush ( stdout );
                fflush ( stdout );
                break;
            }
            case 1 : {
                printf ( "\033[3F%s\033[3E\0338", a [ i % 3].c_str());
                fflush ( stdout );
                break;
            }
            case 2 : {
                printf ( "\033[2F%s\033[2E\0338", a [ i % 3].c_str());
                fflush ( stdout );
                break;
            }
        }
        sleep ( 1 );
    }
    pthread_cancel ( pthread_self() );
    return nullptr;
}

int main() {

    printf ( "\n\n\n\n\n\n\n\n\n" );
    pthread_t th1;
    pthread_create ( &th1, nullptr, & threadMain, nullptr );
    char buffer [ 128 ];
    while ( true ) {
        fgets ( buffer, 128, stdin );
        buffer [ strlen ( buffer ) - 1 ] = 0;
        printf ( "\033[2F\033[2K%s", buffer );
        fflush ( stdout );
        printf ( "\033[1E\033[2K" );
    }
    pthread_join ( th1, nullptr );
}