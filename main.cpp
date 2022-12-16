
#include <CommonLibs>
#include <CDS/util/JSON>
#include <CDS/filesystem/Path>
#include "headers/client/concurrentDescriptor/AtomicSocket.hpp"


int main() {
    cds :: String in1 = cds :: filesystem :: Path ( __FILE__ ).parent() / "resources/map.json";
    auto in1Path = in1.cStr();
    std :: stringstream iss;
    iss << std :: ifstream ( in1Path ).rdbuf ();
    auto json = cds :: json :: parseJson ( iss.str() );
    std :: cout << json.toString() << '\n';
    for ( auto & e : json.getArray ( "streets" ) ) {
        std::cout << e.getJson().getInt ( "firstEndId" ) << ' ';
        std::cout << e.getJson().getInt ( "secondEndId" ) << ' ';
        std::cout << e.getJson().getInt ( "speedLimit" ) << ' ';
        std::cout << e.getJson().getInt ( "length" ) << ' ';
        std::cout << e.getJson().getString ( "name" ) << '\n';
    }
    for ( auto & e : json.getArray ( "gasStations" ) ) {
        std :: cout << e.getJson().getInt ( "id" ) << '\n';
    }
}