//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_HPP
#define CONCURRENT_SV_THREAD_HPP

class Thread {

private:
    class Server;

private:
    int _client_fd { 0 };

public:
    Thread ( pthread_t * newThread,
             int * threadParam );

private:
    static void * _server_main ( void * param );
};

#include "impl/Thread.hpp"

#endif //CONCURRENT_SV_THREAD_HPP
