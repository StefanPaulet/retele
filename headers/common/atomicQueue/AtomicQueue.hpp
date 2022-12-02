//
// Created by stefan on 01.12.2022.
//

#ifndef CONCURRENT_SV_ATOMIC_QUEUE_HPP
#define CONCURRENT_SV_ATOMIC_QUEUE_HPP

#include <string>
#include <memory>

class AtomicQueue {

public:
    struct SharedNode {

        std :: string   _message;
        std :: shared_ptr < SharedNode >     _pNext;

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

public:
    using QueueNode = std :: shared_ptr < SharedNode >;

private:
    QueueNode _pFront;

private:
    QueueNode _pBack;

private:
    std :: mutex _queueLock;


public:
    AtomicQueue () {
        this->_pFront = this->_pBack = std :: make_shared < SharedNode > ();
    }

public:
    auto back () -> QueueNode {

        std :: lock_guard lock ( this->_queueLock );
        return this->_pBack;
    }

public:
    auto front () -> QueueNode {

        return this->_pFront;
    }


public:
    auto push_back ( std :: string && message ) {

        std :: lock_guard lock ( this->_queueLock );

        auto new_QueueNode = std::make_shared < SharedNode > ();

        this->_pBack->_pNext = new_QueueNode;
        this->_pBack->_message = message;
        this->_pBack = new_QueueNode;
    }


public:
    auto pop_front () {

        auto oldFront = this->_pFront;
        this->_pFront = this->_pFront->_pNext;
        oldFront.reset();
    }
};

#endif //CONCURRENT_SV_ATOMIC_QUEUE_HPP
