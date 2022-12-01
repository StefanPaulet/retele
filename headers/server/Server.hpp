//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_SERVER_HPP
#define CONCURRENT_SV_SERVER_HPP

#include <list>


#define __BACKLOG_SIZE 16    /* NOLINT(bugprone-reserved-identifier) */

class Server {

private:
    static Server * _instance;

private:
    sockaddr_in _server_info { };

private:
    int _socked_fd { 0 };

private:
    std :: list < pthread_t > _threadList { };

private:
    pthread_mutex_t _threadListLock { };

private:
    Server();

public:
    static auto getInstance () -> Server *;

public:
    auto initialize_server () -> bool;

public:
    [[nodiscard]] auto get_client () const -> int;

public:
    [[nodiscard]] auto create_thread ( int * clientFd ) -> bool;

private:
    static auto server_error ( int clientFd, std :: string const & error ) -> void;

public:
    auto disconnect_client ( pthread_t threadId ) -> void;
};

#include "thread/ServerThread.hpp"
#include "impl/Server.hpp"

#endif //CONCURRENT_SV_SERVER_HPP
