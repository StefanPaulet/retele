//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_SERVER_HPP
#define CONCURRENT_SV_SERVER_HPP

#include <netinet/in.h>
#include <list>
#include <mutex>
#include "../common/graph/Graph.hpp"

constexpr auto __BACKLOG_SIZE = 16;    /* NOLINT(bugprone-reserved-identifier) */

class Server {

private:
    static Server * _instance;

    sockaddr_in _server_info { };

    int _socked_fd { 0 };

    pthread_t _queue_supervisor_id { };

    std :: list < pthread_t > _thread_list { };

    std :: mutex _thread_list_lock { };

    Server();

public:
    static auto getInstance () -> Server *;

    auto initializeServer () -> bool;

    [[nodiscard]] auto getClient () const -> int;

    [[nodiscard]] auto createThread ( int * clientFd ) -> bool;

private:
    static auto serverError ( int clientFd, std :: string const & error ) -> void;

public:
    auto disconnectClient ( pthread_t threadId ) -> void;
};


#include "../common/graph/impl/Graph.hpp"
#include "thread/ServerThread.hpp"
#include "impl/Server.hpp"

#endif //CONCURRENT_SV_SERVER_HPP
