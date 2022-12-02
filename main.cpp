#include <iostream>
#include <atomic>
#include <memory>
class AtomicQueue {

public:
    struct QueueNode {

        std :: string                 _message;
        std :: atomic < QueueNode * > _pNext;

        explicit QueueNode ( std :: string const & message ) :
                _message ( message ) {

        }
        explicit QueueNode ( std :: string && message ) :
                _message ( std :: move ( message ) ) {

        }
    };

private:
    std :: atomic < QueueNode * > _pFront;

private:
    std :: atomic < QueueNode * > _pBack;


public:
    auto back () -> QueueNode * {
        return this->_pBack.load();
    }


public:
    auto push_back ( std :: string && message ) {

        auto new_QueueNode = new QueueNode ( message );
        auto oldBack = this->_pBack.load();
        oldBack->_pNext.store ( new_QueueNode );
        while ( this->_pBack.compare_exchange_strong (
                oldBack,
                new_QueueNode
        )
                );
    }

};


int main() {
    AtomicQueue a;
    a.push_back ( "abc" );
    std :: cout << a.back() << '\n';
    a.push_back ( "def" );
    std :: cout << a.back() << '\n';
    a.push_back ( "ghi" );
    std :: cout << a.back() << '\n';
    a.push_back ( "jkl" );
    std :: cout << a.back() << '\n';
}