#include <iostream>
#include <AtomicQueue>

int main() {

    AtomicQueue a;
    a.push_back ( "abc" );
    std :: cout << a.front()->_pNext.use_count() << '\n';
    std :: cout << a.back().use_count() << '\n';
    a.push_back ( "abc" );
    std :: cout << a.front().use_count() << '\n';
    std :: cout << a.back() << '\n';
    a.push_back ( "def" );
    std :: cout << a.back() << '\n';
    a.push_back ( "ghi" );
    std :: cout << a.back() << '\n';
    a.push_back ( "jkl" );
    std :: cout << a.back() << '\n';
}