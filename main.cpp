
#include <CommonLibs>
#include <CDS/experimental/TreeMap>
#include "headers/client/concurrentDescriptor/ConcurrentDescriptor.hpp"

int main() {
    cds :: experimental :: TreeMap <int, int> tm { {1,2}, {3,4}, {5,6} };
    for ( auto e : tm.keys() ) {
        std :: cout << e << ' ' << '\n';
    }
}