//
// Created by stefan on 01.12.2022.
//

#ifndef CONCURRENT_SV_ATOMIC_QUEUE_HPP
#define CONCURRENT_SV_ATOMIC_QUEUE_HPP

#include <string>
#include <atomic>

class AtomicQueue {

public:
    struct QueueNode {

        std :: string _message;
        QueueNode *        _pNext;

        explicit QueueNode ( std :: string const & message ) :
                _message ( message ),
                _pNext   ( nullptr ) {

        }
        explicit QueueNode ( std :: string && message ) :
                _message ( std :: move ( message ) ),
                _pNext   ( nullptr ) {

        }
    };

private:
    std :: atomic < QueueNode * > _pFront;

private:
    std :: atomic < QueueNode * > _pBack;


public:
    AtomicQueue () {
        this->_pFront.store ( nullptr );
        this->_pBack.store ( nullptr );
    }


public:
    auto back () -> QueueNode * {
        return this->_pBack.load();
    }


public:
    auto push_back ( std :: string const & message ) {

        auto new_QueueNode = new QueueNode ( message );

        new_QueueNode->_pNext = this->_pBack.load();

        while ( ! this->_pBack.compare_exchange_strong ( new_QueueNode->_pNext, new_QueueNode ) );
    }

};

#endif //CONCURRENT_SV_ATOMIC_QUEUE_HPP
