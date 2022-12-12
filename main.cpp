#include <iostream>
#include <cstring>
#include <functional>
int f ( int a, int b, int c ) { return a + b + c; }
int f ( int a, int b ) { return a * b; }

int main() {
    std::function<int(int, int)> fn = static_cast < int ( * ) ( int, int ) > ( f );
    std :: cout << fn ( 4, 5 ) << '\n';
    std :: cout << f (2, 3, 4 ) << '\n';
    std :: cout << f (2, 3 ) << '\n';
}