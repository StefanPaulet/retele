
#include <CommonLibs>
#include <CDS/util/JSON>
#include <CDS/filesystem/Path>
#include "headers/client/concurrentDescriptor/AtomicSocket.hpp"



template < auto func >
void f () {
    std :: cout << "Int getter\n";
}


int main() {
    f < & cds :: json :: JsonObject :: getInt > ();
    f < & cds :: json :: JsonObject :: getFloat > ();
}