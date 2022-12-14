//
// Created by stefan on 01.12.2022.
//

#ifndef CONCURRENT_SV_ATOMIC_QUEUE_HPP
#define CONCURRENT_SV_ATOMIC_QUEUE_HPP

#include <string>
#include <memory>
#include <mutex>

class AtomicQueue {

private:
    friend class User;

private:
    struct SharedNode {

        std :: string                    _message;
        std :: shared_ptr < SharedNode > _pNext;

        SharedNode() :
                _pNext ( nullptr ) {

        }

        explicit SharedNode ( std :: string const & message ) :
                _message ( message ),
                _pNext   ( nullptr ) {

        }
        explicit SharedNode ( std :: string && message ) :
                _message ( std :: move ( message ) ),
                _pNext   ( nullptr ) {

        }
    };

private:
    using QueueNode = std :: shared_ptr < SharedNode >;

public:
    QueueNode _pFront;

public:
    QueueNode _pBack;

private:
    std :: mutex _back_lock;

private:
    std :: mutex _front_lock;


public:
    constexpr AtomicQueue () {
        this->_pFront = this->_pBack = std :: make_shared < SharedNode > ();
    }

public:
    auto back () -> QueueNode const & {

        std :: lock_guard lock ( this->_back_lock );
        return this->_pBack;
    }

public:
    auto front () -> QueueNode const & {

        std :: lock_guard lock ( this->_front_lock );
        return this->_pFront;
    }


public:
    auto push_back ( std :: string && message ) {

        std :: lock_guard lock ( this->_back_lock );

        auto new_QueueNode = std::make_shared < SharedNode > ();

        this->_pBack->_pNext = new_QueueNode;
        this->_pBack->_message = message;
        this->_pBack = new_QueueNode;
    }


public:
    auto pop_front () {

        std :: lock_guard lock ( this->_front_lock );

        auto oldFront = this->_pFront;
        this->_pFront = this->_pFront->_pNext;
        oldFront.reset();
    }
};

#endif //CONCURRENT_SV_ATOMIC_QUEUE_HPP
