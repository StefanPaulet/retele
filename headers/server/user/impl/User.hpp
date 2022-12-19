//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_IMPL_HPP
#define CONCURRENT_SV_USER_IMPL_HPP

auto User :: pEventQueue = new AtomicQueue ();

auto User :: pGraph = new Graph ();

RandomGenerator User :: randomGenerator { };

std :: array < std :: string, 6 > const User :: signalNamesArray {
    "roadblock",
    "police",
    "accident",
    "wet-ground",
    "frozen-ground",
    "crowded"
};

User :: CommonWeather :: CommonWeather() {

    std :: string currentSeason;

    {
        time_t rawTime;
        struct tm * pInfo;
        char buffer [ 4 ];

        time ( & rawTime );
        pInfo = localtime ( & rawTime );

        strftime ( buffer, 128, "%m", pInfo );

        auto seasonNr = strtol ( buffer, nullptr, 10 );

        switch ( seasonNr ) {
            case 12:
            case 1:
            case 2: {
                currentSeason = "winter";
                break;
            }
            case 6:
            case 7:
            case 8: {
                currentSeason = "summer";
                break;
            }

            default: {
                currentSeason = "normal";
            }
        }
    }

    cds :: json :: JsonObject jsonData;

    {
        cds :: String mapPathString = cds :: filesystem :: Path ( __COMMON_LIBS_PATH ).parent().parent() / "resources/weather.json";
        auto mapPathStringLiteral = mapPathString.cStr();
        std :: stringstream iss;
        iss << std :: ifstream ( mapPathStringLiteral ).rdbuf();

        jsonData = cds :: json :: parseJson ( iss.str() );
    }

    for ( auto & e : jsonData.getArray ( currentSeason ) ) {

        auto aux = e.getJson();

        std :: string weatherNews ( "Temperature: " + std :: to_string ( aux.getInt ( "temperature" ) ) );
        weatherNews +=
                "\nRain chance: " + std :: to_string ( aux.getInt ( "rainChance" ) ) +
                "\nThe sky will be: " + aux.getString ( "sky" ) + "\n";
        if ( !aux.getString ( "other" ).empty() ) {
            weatherNews += "Careful! There might be " + aux.getString ( "other" ) + "\n";
        }

        this->pWeatherString->push_back ( new std :: string ( weatherNews ) );

    }

    this->commonChoice = randomGenerator.getRandomInRange ( ( uint8 ) this->pWeatherString->size() );

}

User :: CommonWeather :: ~CommonWeather () {

    for ( auto & e : * this->pWeatherString ) {
        delete e;
    }
    this->pWeatherString.reset();

}

auto User :: pCommonWeather = new CommonWeather;




auto User :: send_msg ( std :: string const & message ) const -> void {

    auto messageSize = message.size();

    write ( this->_client_fd, & messageSize, sizeof ( messageSize ) );
    write ( this->_client_fd, message.c_str(), messageSize );
}


auto User :: receive_initial_data () -> bool {

    sint16 request_nr;
    uint16 recv_position;
    uint8 recv_speed;

    if ( 0 >= read ( this->_client_fd, & request_nr, sizeof ( request_nr ) ) ) {
        return false;
    }
    if ( request_nr != __INITIALIZING_POSITION_REQUEST ) {
        fprintf ( stderr, "Client %d failed to send his initial position\n", this->_client_fd );
        return false;
    }

    if ( 0 >= read ( this->_client_fd, & recv_position, sizeof ( recv_position ) ) ) {
        return false;
    }
    this->_userPosition.setStreetId ( recv_position );

    if ( 0 >= read ( this->_client_fd, & request_nr, sizeof ( request_nr ) ) ) {
        return false;
    }
    if ( request_nr != __INITIALIZING_SPEED_REQUEST ) {
        fprintf ( stderr, "Client %d failed to send his initial speed\n", this->_client_fd );
        return false;
    }

    if ( 0 >= read ( this->_client_fd, & recv_speed, sizeof ( recv_speed ) ) ) {
        return false;
    }
    this->_userPosition.setSpeed( recv_speed );

    printf ( "Client %d sent his position %d and speed %d\n",
            this->_client_fd,
            this->_userPosition.getStreetId(),
            this->_userPosition.getSpeed() );
    return true;
}


auto User :: handle_request () -> void {

    bool clientConnected = true;
    sint16 request_nr;

    if ( ! this->receive_initial_data() ) {
        pthread_cancel ( pthread_self() );
        return;
    }

    while ( clientConnected ) {

        if ( 0 >= read ( this->_client_fd, & request_nr, sizeof ( request_nr ) ) ) {
            break;
        }

        switch ( request_nr ) {
            case __NO_PARAM_REQUEST : {
                this->send_msg ( "Command must have parameters\n" );
                break;
            }
            case __SIGNAL : {
                this->handle_signal();
                break;
            }
            case __EXIT : {
                this->send_msg ( "Bye bye\n" );
                clientConnected = false;
                break;
            }
            case __TIMED_EVENTS_UPDATE : {
                this->handle_event_update();
                break;
            }
            case __TIMED_SPEED_LIMIT_UPDATE : {
                this->handle_speed_limit_update();
                break;
            }
            case __TIMED_EVENT_STILL_PRESENT : {
                this->handle_event_removal();
                break;

            }
            case __TIMED_POSITION_UPDATE : {
                this->handle_position_update();
                break;
            }
            case __TIMED_SPEED_UPDATE : {
                this->handle_speed_update();
                break;
            }
            case __GET_WEATHER : {
                this->handle_get_weather();
                break;
            }
            case __GET_GS : {
                this->handle_get_gas_stations();
                break;
            }
            case __TIMED_SPORTS_UPDATE : {
                break;
            }
            case __EVENT_MISSING : {
                if ( this->_waiting_response > 0 ) {
                    this->remove_street_event ( this->_waiting_response );
                    break;
                }
            }
            default : {
                this->send_msg ( "No such command available\n" );
                break;
            }
        }

    }

    Server :: getInstance ()->disconnect_client ( pthread_self() );

}


auto User :: handle_signal () -> void {

    char buffer [ __STANDARD_BUFFER_SIZE ];
    if ( 0 >= read ( this->_client_fd, buffer, __STANDARD_BUFFER_SIZE ) ) {
        return;
    }
    auto userStreet = pGraph->getStreet ( this->_userPosition.getStreetId() );

    std :: string stringedBuffer { buffer };

    for ( auto & e : signalNamesArray ) {
        if ( e == stringedBuffer ) {

            this->send_msg ( "Thank you for your signal\n" );

            stringedBuffer += " on" + userStreet->getName() + "\n";

            pEventQueue->push_back ( std :: move ( stringedBuffer ) );

            if ( e == "roadblock" ) {
                if ( userStreet->isJammed() ) {
                    userStreet->remove_traffic_jam ();
                }
                userStreet->signal_roadblock();
            } else {
                if ( e != "police" && ! userStreet->isBlocked() ) {
                    pGraph->getStreet ( this->_userPosition.getStreetId() )->signal_traffic_jam();
                }
            }

            return;
        }
    }

    this->send_msg ( "Sorry, no such signal available\n" );

}


auto User :: handle_event_update () -> void {

    if ( this->_pLocalEventNode != pEventQueue->back() ) {
        std :: string event = "Attention: " + this->_pLocalEventNode->_message;
        this->send_msg ( event );
        this->_pLocalEventNode = this->_pLocalEventNode->_pNext;
    }
}


auto User :: handle_speed_limit_update () -> void {

    auto speedLimit = pGraph->getStreet ( this->_userPosition.getStreetId() )->getMaxSpeed();
    if ( speedLimit < this->_userPosition.getSpeed() ) {
        this->send_msg (
                "Careful! Speed limit is " + std :: to_string ( speedLimit ) + "\n"
            );
    }
}


auto User :: handle_event_removal () -> void {

    this->_waiting_response = 0;

    auto userStreet = pGraph->getStreet ( this->_userPosition.getStreetId() );
    if ( userStreet->isBlocked() ) {
        this->send_msg ( "Roadblock signaled on your street. Is it still there?[enter \"n\" to remove it]\n" );
        this->_waiting_response = 1;
    } else {
        if ( userStreet->isJammed() ) {
            this->send_msg ( "Traffic jam signaled on your street. Is it still there?[enter \"n\" to remove it]\n" );
            this->_waiting_response = 2;
        }
    }
}


auto User :: remove_street_event ( uint8 eventType ) const -> void {

    if ( eventType == 1 ) {
        pGraph->getStreet ( this->_userPosition.getStreetId() )->remove_roadblock();
    } else if ( eventType == 2 ) {
        pGraph->getStreet ( this->_userPosition.getStreetId() )->remove_traffic_jam();
    }
    this->send_msg ( "Thank you! Problem removed\n" );
}


auto User :: handle_position_update () -> void {

    uint16 newPosition;
    if ( 0 >= read ( this->_client_fd, & newPosition, sizeof ( newPosition ) ) ) {
        return;
    }
    if ( this->_userPosition.getStreetId() != newPosition ) {
        this->_userPosition.setStreetId ( newPosition );
    }
}


auto User :: handle_speed_update () -> void {

    uint8 newSpeed;
    if ( 0 >= read ( this->_client_fd, & newSpeed, sizeof ( newSpeed ) ) ) {
        return;
    }
    this->_userPosition.setSpeed ( newSpeed );
}


auto User :: handle_get_weather () -> void {

    this->send_msg ( * ( * pCommonWeather->pWeatherString ) [ pCommonWeather->commonChoice ] );
}


auto User :: handle_get_gas_stations () -> void {

    auto pResults = pGraph->bfsTraversal (
            this->_userPosition.getStreetId(),
            & Node :: isGasStation
    );

    if ( pResults->empty() ) {
        this->send_msg ( "Sorry, we couldn't find what you needed in your vicinity\n" );
    } else {
        std :: string resultString { "We found what you need at:\n"};
        for ( auto & e : * pResults ) {
            auto firstStreet = e->getStreetList()->begin();
            auto secondStreet = firstStreet ++;
            if ( ( * firstStreet )->getName() == ( * secondStreet )->getName() ) {
                ++ firstStreet;
            }
            resultString +=
                    "The intersection of streets " + ( * firstStreet )->getName() +
                    " and " + ( * secondStreet )->getName() + "\n";
        }
        this->send_msg ( resultString );
    }

    delete pResults;
}

#endif //CONCURRENT_SV_USER_IMPL_HPP
