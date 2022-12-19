//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_HPP
#define CONCURRENT_SV_USER_HPP

#include <AtomicQueue>
#include <map>

#include "../../common/graph/Graph.hpp"

#include "../../common/vehicle/Vehicle.hpp"

#include "../../common/randomGenerator/RandomGenerator.hpp"

class User {

private:
    static AtomicQueue * pEventQueue;

    static Graph * pGraph;

    static RandomGenerator randomGenerator;

    static std :: array < std :: string, 6 > const signalNamesArray;


    struct GasStationMap {
        std :: unique_ptr < std :: map < uint16, std :: pair < cds :: String const *, float > > > pMap = std :: make_unique < std :: map < uint16, std :: pair < cds :: String const *, float > > > ();

        GasStationMap ();
        ~GasStationMap ();
    };
    static GasStationMap const * pGasStationMap;

    struct CommonWeather {
        uint8 commonChoice = 0;
        std :: unique_ptr < std :: vector < std :: string const * > > pWeatherString = std :: make_unique < std :: vector < std :: string const * > > ();

        CommonWeather ();
        ~CommonWeather ();
    };
    static CommonWeather * pCommonWeather;

    AtomicQueue :: QueueNode _pLocalEventNode { pEventQueue->front() };

    int _client_fd;

    Vehicle _userPosition { 30, 0 };

    std :: pair < uint8, uint16 > _event_removal_pair { 0, 0 };

    friend auto _queue_supervisor_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

public:
    explicit User ( int clientFd ) :
        _client_fd ( clientFd ) {

        auto newWeather = randomGenerator.getRandomInRange ( ( uint8 ) 15 );
        if ( newWeather == 0 ) {
            auto oldWeather = pCommonWeather->commonChoice;
            while ( pCommonWeather->commonChoice == oldWeather ) {
                pCommonWeather->commonChoice = randomGenerator.getRandomInRange ( ( uint8 ) pCommonWeather->pWeatherString->size() );
            }
        }
    };

private:
    auto sendMessage ( std :: string const & message ) const -> void;

    auto receiveInitialData () -> bool;

public:
    auto handleRequest () -> void;

private:
    auto handleSignal () -> void;

    auto handleEventUpdate () -> void;

    auto handleSpeedLimitUpdate () -> void;

    auto handleEventRemoval () -> void;

    auto handleStreetEvent ( uint8 eventType ) const -> void;

    auto handlePositionUpdate () -> void;

    auto handleSpeedUpdate () -> void;

    auto handleGetWeather () -> void;

    auto handleGetGasStations () -> void;
};

#include "../../common/vehicle/impl/Vehicle.hpp"        /* NOLINT(llvm-include-order) */

#include "../../common/graph/impl/Graph.hpp"

#endif //CONCURRENT_SV_USER_HPP
