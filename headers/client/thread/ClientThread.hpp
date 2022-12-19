//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

#include "../../common/thread/ThreadCommons.hpp"

#include "../movingVehicle/MovingVehicle.hpp"

auto _console_output_main ( void * param ) -> void * {      /* NOLINT(bugprone-reserved-identifier) */

    auto serverFd = reinterpret_cast < AtomicSocket * > ( param );

    uint64 bufferSize;
    char buffer [ __STANDARD_BUFFER_SIZE ];

    while ( true ) {

        if ( 0 >= serverFd->read ( & bufferSize, sizeof ( bufferSize ) ) ) {
            printf ( "Server connection dropped\n" );
            pthread_cancel ( pthread_self() );
            break;
        }
        if ( 0 >= serverFd->read ( buffer, bufferSize )) {
            printf ( "Server connection dropped\n" );
            pthread_cancel ( pthread_self() );
            break;
        }
        write ( 1, buffer, bufferSize );
    }

    exit ( EXIT_FAILURE );

}

auto _pinging_main ( void * param ) -> void * {       /* NOLINT(bugprone-reserved-identifier) */

    std :: pair < sint16 , uint8 > requests[] = {
            { __TIMED_EVENTS_UPDATE, 1 },
            { __TIMED_SPEED_LIMIT_UPDATE, 5 },
            { __TIMED_EVENT_STILL_PRESENT, 3 },
            { __TIMED_SPORTS_UPDATE, 60 }
    };

    auto clientFd = reinterpret_cast < AtomicSocket * > ( param );

    int cycleCount = 0;

    while ( true ) {

        for ( auto e : requests ) {
            if ( cycleCount % e.second == 0 ) {
                if ( clientFd->write ( & e.first, sizeof ( e.first ) ) < 0 && errno == EPIPE ) {
                    pthread_cancel ( pthread_self() );
                    return nullptr;
                }
            }
        }

        ++ cycleCount;
        sleep ( 1 );
    }
    return nullptr;
}


auto _movement_main ( void * param ) -> void * {        /* NOLINT(bugprone-reserved-identifier) */

    auto serverFd = reinterpret_cast < AtomicSocket * > ( param );
    auto movingVehicle = std :: make_unique < MovingVehicle >();

    {
        std :: unique_lock lk ( Client :: conditionMutex );
        Client :: conditionVariable.wait ( lk, [] { return ! Client :: serverInformed; } );

        auto request = __INITIALIZING_POSITION_REQUEST;
        auto positionParam = movingVehicle->getStreetId();
        serverFd->write ( & request, sizeof ( request ), & positionParam, sizeof ( positionParam ) );

        request = __INITIALIZING_SPEED_REQUEST;
        auto speedParam = movingVehicle->getSpeed();
        serverFd->write ( &request, sizeof ( request ), & speedParam, sizeof ( speedParam ) );

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

        speedParameter    = movingVehicle->getSpeed();
        positionParameter = movingVehicle->getStreetId();

        if ( -1 == serverFd->write ( & speedRequest, sizeof ( speedRequest ), & speedParameter, sizeof ( speedParameter ) ) && errno == EPIPE ) {
            pthread_cancel ( pthread_self() );
            return nullptr;
        }

        if ( -1 == serverFd->write ( & positionRequest, sizeof ( positionRequest ), & positionParameter, sizeof ( positionParameter ) ) ) {
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


#include "../movingVehicle/impl/MovingVehicle.hpp"

#endif //CONCURRENT_SV_THREAD_HPP
