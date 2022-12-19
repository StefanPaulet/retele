//
// Created by stefan on 15.12.2022.
//

#ifndef CONCURRENT_SV_ATOMIC_SOCKET_HPP
#define CONCURRENT_SV_ATOMIC_SOCKET_HPP

class AtomicSocket {

private:
    int _fd { 0 };

    std :: mutex _descriptor_lock;

public:
    explicit AtomicSocket (
            int const & fd
        ) : _fd ( fd ) {

    }

    auto operator= ( int const & fd ) -> int & {

        this->_fd = fd;
        return this->_fd;
    }


    explicit operator int () const {

        return this->_fd;
    }


    auto write (
            void const * message,
            uint16 length,
            void const * param = nullptr,
            uint16 paramLength = 0
    ) -> sint64 {

        std :: lock_guard lock ( this->_descriptor_lock );
        auto result = :: write ( this->_fd, message, length );
        if ( result < 0 ) {
            return result;
        }

        if ( param != nullptr ) {
            result = :: write ( this->_fd, param, paramLength );
        }

        return result;
    }


    auto read (
            void * message,
            uint16 length
    ) const -> sint64 {

        return :: read ( this->_fd, message, length );
    }
};

#endif //CONCURRENT_SV_ATOMIC_SOCKET_HPP
