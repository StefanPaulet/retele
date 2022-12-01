//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

#include "../../common/thread/ThreadCommons.hpp"
#include "../user/User.hpp"

namespace {
    auto _server_main ( void * param ) -> void * {

        int clientFd = * ( int * ) param;

        auto user = new User ( clientFd );
        user->handle_request();

        pthread_cancel ( pthread_self() );

        delete user;

        return nullptr;
    }
}

typedef void * ( * ThreadFunctionType ) ( void * );

static auto launch_new_thread (
                 pthread_t * newThread,
                 int * threadParam
     ) -> bool {


    if ( 0 > pthread_create ( newThread, nullptr, & _server_main, ( void * ) threadParam ) ) {
        perror ( "Thread creation error" );
        return false;
    }
    return true;
}

#include "../user/impl/User.hpp"

#endif //CONCURRENT_SV_THREAD_HPP
