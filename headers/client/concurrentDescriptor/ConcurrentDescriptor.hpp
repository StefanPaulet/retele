//
// Created by stefan on 15.12.2022.
//

#ifndef CONCURRENT_SV_CONCURRENT_DESCRIPTOR_HPP
#define CONCURRENT_SV_CONCURRENT_DESCRIPTOR_HPP

class ConcurrentDescriptor {

private:
    int _fd { 0 };

private:
    std :: mutex _descriptor_lock;

public:
    explicit ConcurrentDescriptor (
            int const & fd
        ) : _fd ( server_fd ) {

    }

public:
    auto write (
            void * message,
            uint16 length
    ) -> sint64 {

        std :: lock_guard lock ( this->_descriptor_lock );
        return :: write ( this->_fd, message, length );
    }

public:
    auto read (
            void * message,
            uint16 length
    ) -> sint64 {

        return :: read ( this->_fd, message, length );
    }
};

#endif //CONCURRENT_SV_CONCURRENT_DESCRIPTOR_HPP
