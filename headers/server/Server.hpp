//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_SERVER_HPP
#define CONCURRENT_SV_SERVER_HPP

#include <list>
#include <map>
#include <utility>
#include "thread/Thread.hpp"

#define __PORT 7000  /* NOLINT(bugprone-reserved-identifier) */

#define __BACKLOG_SIZE 16    /* NOLINT(bugprone-reserved-identifier) */

#define __MAX_USER_COUNT 1024   /* NOLINT(bugprone-reserved-identifier) */
class Server {

private:
    static Server * _instance;

private:
    sockaddr_in _server_info { 0 };

private:
    int _socked_fd { 0 };

private:
    std :: list < Thread * > * _p_thread_list { nullptr };

private:
    pthread_t _threadId [ __MAX_USER_COUNT ]{};

private:
    uint8 _threadCount { 0 };

private:
    std :: map < int, pthread_t > * _p_user_map { nullptr };

private:
    Server();

public:
    static auto getInstance () -> Server *;

public:
    auto initialize_server () -> bool;

public:
    [[nodiscard]] auto get_client () const -> int;

public:
    auto create_thread ( int * clientFd ) -> Thread *;
};

#include "impl/Server.hpp"

#endif //CONCURRENT_SV_SERVER_HPP
