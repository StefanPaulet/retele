//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

#include "../../common/thread/ThreadCommons.hpp"
#include "../user/User.hpp"

auto _server_main ( void * param ) -> void * {      /* NOLINT(bugprone-reserved-identifier) */

    int clientFd = * ( int * ) param;

    auto user = new User ( clientFd );
    user->handleRequest();

    pthread_cancel ( pthread_self() );

    delete user;

    return nullptr;
}

auto _queue_supervisor_main ( void * param ) -> void * {

    while ( true ) {

        if ( User :: pEventQueue->front().use_count() == 1 ) {
            User :: pEventQueue->pop_front();
        }

        sleep ( 5 );
    }
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

#include "../user/impl/User.hpp"

#endif //CONCURRENT_SV_THREAD_HPP
