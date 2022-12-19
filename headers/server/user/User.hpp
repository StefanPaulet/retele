//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_HPP
#define CONCURRENT_SV_USER_HPP

#include <AtomicQueue>
#include <CDS/util/JSON>
#include <random>
#include <array>
#include <chrono>
#include "../../common/graph/Graph.hpp"
#include "../../common/vehicle/Vehicle.hpp"
#include "../../common/randomGenerator/RandomGenerator.hpp"

class User {

private:
    static AtomicQueue * pEventQueue;

    static Graph * pGraph;

    static RandomGenerator randomGenerator;

    static std :: array < std :: string, 6 > const signalNamesArray;

    struct CommonWeather {
        uint8 commonChoice = 0;
        std :: unique_ptr < std :: vector < std :: string const * > > pWeatherString = std :: make_unique < std :: vector < std :: string const * > > ();

        CommonWeather();
        ~CommonWeather();
    };
    static CommonWeather * pCommonWeather;

    friend auto _queue_supervisor_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

     AtomicQueue :: QueueNode _pLocalEventNode { pEventQueue->front() };

    int _client_fd;

    Vehicle _userPosition { 30, 0 };

    uint8 _waiting_response { 0 };

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
    auto send_msg ( std :: string const & message ) const -> void;

    auto receive_initial_data () -> bool;

public:
    auto handle_request () -> void;

private:
    auto handle_signal () -> void;

    auto handle_event_update () -> void;

    auto handle_speed_limit_update () -> void;

    auto handle_event_removal () -> void;

    auto remove_street_event ( uint8 eventType ) const -> void;

    auto handle_position_update () -> void;

    auto handle_speed_update () -> void;

    auto handle_get_weather () -> void;

    auto handle_get_gas_stations () -> void;
};

#include "../../common/vehicle/impl/Vehicle.hpp"        /* NOLINT(llvm-include-order) */
#include "../../common/graph/impl/Graph.hpp"

#endif //CONCURRENT_SV_USER_HPP
