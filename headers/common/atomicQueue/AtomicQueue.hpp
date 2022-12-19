//
// Created by stefan on 01.12.2022.
//

#ifndef CONCURRENT_SV_ATOMIC_QUEUE_HPP
#define CONCURRENT_SV_ATOMIC_QUEUE_HPP

#include <string>
#include <memory>

class AtomicQueue {

private:    /* NOLINT(readability-redundant-access-specifiers) */
    friend class User;

    struct SharedNode {

        std :: string                    _message;
        std :: shared_ptr < SharedNode > _pNext { nullptr };

        SharedNode() = default;

        explicit SharedNode ( std :: string const & message ) :
                _message ( message ) {

        }
        explicit SharedNode ( std :: string && message ) :
                _message ( std :: move ( message ) ) {

        }
    };

    using QueueNode = std :: shared_ptr < SharedNode >;

    QueueNode _pFront;

    QueueNode _pBack;

    std :: mutex _back_lock;

    std :: mutex _front_lock;


public:
    constexpr AtomicQueue () {
        this->_pFront = this->_pBack = std :: make_shared < SharedNode > ();
    }

    auto back () -> QueueNode const & {

        std :: lock_guard lock ( this->_back_lock );
        return this->_pBack;
    }


    auto front () -> QueueNode const & {

        std :: lock_guard lock ( this->_front_lock );
        return this->_pFront;
    }


    auto push_back ( std :: string && message ) -> void {

        std :: lock_guard lock ( this->_back_lock );

        auto new_QueueNode = std::make_shared < SharedNode > ();

        this->_pBack->_pNext = new_QueueNode;
        this->_pBack->_message = std :: move ( message );
        this->_pBack = new_QueueNode;
    }


    auto pop_front () -> void {

        std :: lock_guard lock ( this->_front_lock );

        auto oldFront = this->_pFront;
        this->_pFront = this->_pFront->_pNext;
        oldFront.reset();
    }
};

#endif //CONCURRENT_SV_ATOMIC_QUEUE_HPP
