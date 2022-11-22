//
// Created by stefan on 21.11.2022.
//

#ifndef CONCURRENT_SV_THREAD_IMPL_HPP
#define CONCURRENT_SV_THREAD_IMPL_HPP

Thread :: Thread (
        pthread_t * newThread,
       int * threadParam
) {

    pthread_create ( newThread, NULL, Thread :: _server_main, ( void * ) threadParam );
}



#endif //CONCURRENT_SV_THREAD_IMPL_HPP
