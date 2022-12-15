//
// Created by stefan on 01.12.2022.
//

#ifndef CONCURRENT_SV_ATOMIC_QUEUE_HPP
#define CONCURRENT_SV_ATOMIC_QUEUE_HPP

#include <string>
#include <memory>
#include <mutex>

class AtomicQueue {

private:    /* NOLINT(readability-redundant-access-specifiers) */
    friend class User;

private:    /* NOLINT(readability-redundant-access-specifiers) */
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

private:    /* NOLINT(readability-redundant-access-specifiers) */
    using QueueNode = std :: shared_ptr < SharedNode >;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    QueueNode _pFront;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    QueueNode _pBack;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    std :: mutex _back_lock;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    std :: mutex _front_lock;


public:     /* NOLINT(readability-redundant-access-specifiers) */
    constexpr AtomicQueue () {
        this->_pFront = this->_pBack = std :: make_shared < SharedNode > ();
    }

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto back () -> QueueNode const & {

        std :: lock_guard lock ( this->_back_lock );
        return this->_pBack;
    }

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto front () -> QueueNode const & {

        std :: lock_guard lock ( this->_front_lock );
        return this->_pFront;
    }


public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto push_back ( std :: string && message ) {

        std :: lock_guard lock ( this->_back_lock );

        auto new_QueueNode = std::make_shared < SharedNode > ();

        this->_pBack->_pNext = new_QueueNode;
        this->_pBack->_message = std :: move ( message );
        this->_pBack = new_QueueNode;
    }


public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto pop_front () {

        std :: lock_guard lock ( this->_front_lock );

        auto oldFront = this->_pFront;
        this->_pFront = this->_pFront->_pNext;
        oldFront.reset();
    }
};

#endif //CONCURRENT_SV_ATOMIC_QUEUE_HPP
