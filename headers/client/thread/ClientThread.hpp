//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

#include "../../common/thread/ThreadCommons.hpp"

#include "../movingVehicle/MovingVehicle.hpp"
#include "../movingVehicle/impl/MovingVehicle.hpp"

auto _console_output_main ( void * param ) -> void * {      /* NOLINT(bugprone-reserved-identifier) */

    auto serverFd = reinterpret_cast < ConcurrentDescriptor * > ( param );

    uint64 bufferSize;
    char buffer [ __STANDARD_BUFFER_SIZE ];

    while ( true ) {

        if ( 0 >= serverFd->read ( & bufferSize, sizeof ( bufferSize ) ) ) {
            printf ( "Server connection dropped\n" );
            pthread_cancel ( pthread_self() );
            return nullptr;
        }
        if ( 0 >= serverFd->read ( buffer, bufferSize )) {
            printf ( "Server connection dropped\n" );
            pthread_cancel ( pthread_self() );
            return nullptr;
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

    auto clientFd = reinterpret_cast < ConcurrentDescriptor * > ( param );

    int cycleCount = 0;

    while ( true ) {

        for ( auto e : requests ) {
            if ( cycleCount % e.second == 0 ) {
                if ( clientFd->write ( & e.first, sizeof ( int ) ) < 0 && errno == EPIPE ) {
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

    auto serverFd = reinterpret_cast < ConcurrentDescriptor * > ( param );
    auto movingVehicle = new MovingVehicle;

    {
        std :: unique_lock lk ( Client :: conditionMutex );
        Client :: conditionVariable.wait ( lk, [] { return ! Client :: serverInformed; } );

        auto request = __INITIALIZING_POSITION_REQUEST;
        auto requestParam = movingVehicle->getStreetId();
        serverFd->write ( &request, sizeof ( request ) );
        serverFd->write ( & requestParam, sizeof ( requestParam ) );

        request = __INITIALIZING_SPEED_REQUEST;
        requestParam = movingVehicle->getSpeed();
        serverFd->write ( &request, sizeof ( request ) );
        serverFd->write ( & requestParam, sizeof ( requestParam ) );

        printf ( "I sent my position %d and speed %d to the server\n",
                 movingVehicle->getStreetId(),
                 movingVehicle->getSpeed() );

        Client :: serverInformed = true;

        Client :: conditionVariable.notify_one();

    }

    sleep ( 1 );

    auto speedRequest    = __TIMED_SPEED_UPDATE;
    auto positionRequest = __TIMED_POSITION_UPDATE;
    auto speedParameter  = movingVehicle->getSpeed();
    auto positionParameter = movingVehicle->getStreetId();

    while ( true ) {

        movingVehicle->changeSpeed();
        movingVehicle->moveVehicle();

        speedParameter  = movingVehicle->getSpeed();
        positionParameter = movingVehicle->getStreetId();

        if ( -1 == serverFd->write ( & speedRequest, sizeof ( speedRequest ) ) && errno == EPIPE ) {
            pthread_cancel ( pthread_self() );
            return nullptr;
        }

        if ( -1 == serverFd->write ( & speedParameter, sizeof ( speedParameter ) ) ) {
            pthread_cancel ( pthread_self() );
            return nullptr;
        }

        if ( -1 == serverFd->write ( & positionRequest, sizeof ( positionRequest ) ) ) {
            pthread_cancel ( pthread_self() );
            return nullptr;
        }
        if ( -1 == serverFd->write ( & positionParameter, sizeof ( positionParameter ) ) ) {
            pthread_cancel ( pthread_self() );
            return nullptr;
        }

        sleep ( 1 );

    }

    return nullptr;
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
