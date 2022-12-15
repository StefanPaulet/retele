//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

#include "../../common/thread/ThreadCommons.hpp"

#include "../movingVehicle/MovingVehicle.hpp"
#include "../movingVehicle/impl/MovingVehicle.hpp"

auto _console_output_main ( void * param ) -> void * {      /* NOLINT(bugprone-reserved-identifier) */

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

auto _pinging_main ( void * param ) -> void * {       /* NOLINT(bugprone-reserved-identifier) */

    std :: pair < int, int > requests[] = {
            { __TIMED_EVENTS_UPDATE, 1 },
            { __TIMED_SPEED_LIMIT_UPDATE, 10 },
            { __TIMED_EVENT_STILL_PRESENT, 10 },
            { __TIMED_SPEED_UPDATE, 60 },
            { __TIMED_SPORTS_UPDATE, 60 },
            { __TIMED_WEATHER_UPDATE, 60 }
    };

    auto client = reinterpret_cast < Client * > ( param );

    int cycleCount = 0;

    while ( true ) {

        for ( auto e : requests ) {
            if ( cycleCount % e.first == 0 ) {
                if ( client->server_write ( & e.first, sizeof ( int ) ) < 0 && errno == EPIPE ) {
                    pthread_cancel ( pthread_self() );
                    return nullptr;
                }
            }
        }

        ++ cycleCount;
        sleep ( 1 );
    }
}


auto _movement_main ( void * param ) -> void * {        /* NOLINT(bugprone-reserved-identifier) */

    auto client = ( Client * ) param;
    auto movingVehicle = new MovingVehicle;

}

typedef void * ( * ThreadFunctionType ) ( void * );

static auto launch_new_thread (
                 pthread_t * newThread,
                 ThreadFunctionType threadMain,
                 void * threadParam
     ) -> bool {

    if ( 0 > pthread_create ( newThread, nullptr, threadMain, threadParam ) ) {
        perror ( "Thread creation error" );
        return false;
    }
    return true;
}



#endif //CONCURRENT_SV_THREAD_HPP
