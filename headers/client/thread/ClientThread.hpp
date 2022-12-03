//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

#include "../../common/thread/ThreadCommons.hpp"

auto _client_reading_endpoint ( void * param ) -> void * {      /* NOLINT(bugprone-reserved-identifier) */

    int serverFd = * ( int * ) param;

    uint64 bufferSize;
    char buffer [ __STANDARD_BUFFER_SIZE ];

    while ( true ) {

        if ( 0 >= read ( serverFd, & bufferSize, sizeof ( bufferSize ) ) ) {
            printf ( "Server connection dropped\n" );
            break;
        }
        if ( 0 >= read ( serverFd, buffer, bufferSize )) {
            printf ( "Server connection dropped\n" );
            break;
        }
        write ( 1, buffer, bufferSize );
    }

    exit ( EXIT_FAILURE );

}

auto _client_pinging ( void * param ) -> void * {       /* NOLINT(bugprone-reserved-identifier) */

    auto parameters = ( int * ) param;
    int serverFd = parameters[ 0 ];
    int requestType = parameters [ 1 ];
    int frequency = parameters [ 2 ];

    while ( true ) {

        sleep ( frequency );
        if ( 0 > write ( serverFd, & requestType, 4 ) ) {
            break;
        }
    }

    pthread_cancel ( pthread_self() );

    return nullptr;
}

typedef void * ( * ThreadFunctionType ) ( void * );

static auto launch_new_thread (
                 pthread_t * newThread,
                 ThreadFunctionType threadMain,
                 int * threadParam
     ) -> bool {

    if ( 0 > pthread_create ( newThread, nullptr, threadMain, ( void * ) threadParam ) ) {
        perror ( "Thread creation error" );
        return false;
    }
    return true;
}



#endif //CONCURRENT_SV_THREAD_HPP
