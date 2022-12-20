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


User :: GasStationMap :: GasStationMap () {

    cds :: String mapPathString = cds :: filesystem :: Path ( __COMMON_LIBS_PATH ).parent().parent() / "resources/map.json";
    auto mapPathStringLiteral = mapPathString.cStr();
    std :: stringstream iss;
    iss << std :: ifstream ( mapPathStringLiteral ).rdbuf();

    auto jsonData = cds :: json :: parseJson ( iss.str() );

    for ( auto & e : jsonData.getArray ( "gasStations" ) ) {
        auto entry = e.getJson();
        auto newString = new cds :: String ( entry.getString ( "name" ) );
        ( * this->pMap ) [ entry.getInt ( "id" ) ] = std :: make_pair ( newString, entry.getFloat ( "gasPrice" ) );
    }
}
User :: GasStationMap :: ~GasStationMap () {

    for ( auto & [ key, value ] : * this->pMap ) {
        delete value.first;
    }
    this->pMap.reset ();
}
User :: GasStationMap const * User :: pGasStationMap = new GasStationMap;



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


auto User :: sendMessage ( std :: string const & message ) const -> void {

    auto messageSize = message.size();

    write ( this->_client_fd, & messageSize, sizeof ( messageSize ) );
    write ( this->_client_fd, message.c_str(), messageSize );
}


auto User :: receiveInitialData () -> bool {

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

//    printf ( "Client %d sent his position %d and speed %d\n",
//            this->_client_fd,
//            this->_userPosition.getStreetId(),
//            this->_userPosition.getSpeed() );
    return true;
}


auto User :: handleRequest () -> void {

    bool clientConnected = true;
    sint16 request_nr;

    if ( ! this->receiveInitialData() ) {
        pthread_cancel ( pthread_self() );
        return;
    }

    while ( clientConnected ) {

        if ( 0 >= read ( this->_client_fd, & request_nr, sizeof ( request_nr ) ) ) {
            break;
        }

        switch ( request_nr ) {
            case __NO_PARAM_REQUEST : {
                this->sendMessage ( "Command must have parameters\n" );
                break;
            }
            case __SIGNAL : {
                this->handleSignal();
                break;
            }
            case __EXIT : {
                this->sendMessage ( "Bye bye\n" );
                clientConnected = false;
                break;
            }
            case __TIMED_EVENTS_UPDATE : {
                this->handleEventUpdate();
                break;
            }
            case __TIMED_SPEED_LIMIT_UPDATE : {
                this->handleSpeedLimitUpdate();
                break;
            }
            case __TIMED_EVENT_STILL_PRESENT : {
                this->handleEventRemoval();
                break;

            }
            case __TIMED_POSITION_UPDATE : {
                this->handlePositionUpdate();
                break;
            }
            case __TIMED_SPEED_UPDATE : {
                this->handleSpeedUpdate();
                break;
            }
            case __GET_WEATHER : {
                this->handleGetWeather();
                break;
            }
            case __GET_GS : {
                this->handleGetGasStations();
                break;
            }
            case __ENABLE_SPORTS : {
                this->handleSetSportNews();
                break;
            }
            case __DISABLE_SPORTS : {
                this->handleUnsetSportNews();
                break;
            }
            case __TIMED_SPORTS_UPDATE : {
                if ( this->_sport_news_enabled ) {
                    this->handleGetSportNews();
                }
                break;
            }
            case __EVENT_MISSING : {
                if ( this->_event_removal_pair.first > 0 ) {
                    this->handleStreetEvent ( this->_event_removal_pair.first );
                    break;
                }
            }
            default : {
                this->sendMessage ( "No such command available\n" );
                break;
            }
        }

    }

    Server :: getInstance ()->disconnectClient ( pthread_self() );

}


auto User :: handleSignal () -> void {

    char buffer [ __STANDARD_BUFFER_SIZE ];
    if ( 0 >= read ( this->_client_fd, buffer, __STANDARD_BUFFER_SIZE ) ) {
        return;
    }
    auto userStreet = pGraph->getStreet ( this->_userPosition.getStreetId() );

    std :: string stringedBuffer { buffer };

    for ( auto & e : signalNamesArray ) {
        if ( e == stringedBuffer ) {

            this->sendMessage ( "Thank you for your signal\n" );

            stringedBuffer += " on " + userStreet->getName() + "\n";

            pEventQueue->push_back ( std :: move ( stringedBuffer ) );

            if ( e == "roadblock" ) {
//                printf ( "Roadblock on street %s signaled\n", userStreet->getName().c_str() );
                if ( userStreet->isJammed() ) {
                    userStreet->removeTrafficJam ();
                }
                userStreet->signalRoadblock();
            } else {
                if ( e != "police" && ! userStreet->isBlocked() ) {
//                    printf ( "Traffic jam on street %s signaled\n", userStreet->getName().c_str() );
                    pGraph->getStreet ( this->_userPosition.getStreetId() )->signalTrafficJam();
                }
            }

            return;
        }
    }

    this->sendMessage ( "Sorry, no such signal available\n" );

}


auto User :: handleEventUpdate () -> void {

    if ( this->_pLocalEventNode != pEventQueue->back() ) {
        std :: string event = "Attention: " + this->_pLocalEventNode->_message;
        this->sendMessage ( event );
        this->_pLocalEventNode = this->_pLocalEventNode->_pNext;
    }
}


auto User :: handleSpeedLimitUpdate () -> void {

    auto speedLimit = pGraph->getStreet ( this->_userPosition.getStreetId() )->getMaxSpeed();
    if ( speedLimit < this->_userPosition.getSpeed() ) {
        this->sendMessage (
                "Careful! Speed limit is " + std :: to_string ( speedLimit ) + "\n"
            );
    }
}


auto User :: handleEventRemoval () -> void {

    this->_event_removal_pair.first = 0;

    auto userStreet = pGraph->getStreet ( this->_userPosition.getStreetId() );
    this->_event_removal_pair.second = this->_userPosition.getStreetId();

    if ( userStreet->isBlocked() ) {
        this->sendMessage ( "Roadblock signaled on your street. Is it still there?[enter \"n\" to remove it]\n" );
        this->_event_removal_pair.first = 1;
    } else {
        if ( userStreet->isJammed() ) {
            this->sendMessage ( "Traffic jam signaled on your street. Is it still there?[enter \"n\" to remove it]\n" );
            this->_event_removal_pair.first = 2;
        }
    }
}


auto User :: handleStreetEvent ( uint8 eventType ) const -> void {

    if ( eventType == 1 ) {
//        printf ( "Roadblock on street %s removed\n", pGraph->getStreet ( this->_event_removal_pair.second )->getName().c_str() );
        pGraph->getStreet ( this->_event_removal_pair.second )->removeRoadblock();
    } else if ( eventType == 2 ) {
//        printf ( "Traffic jam on street %s removed\n", pGraph->getStreet ( this->_event_removal_pair.second )->getName().c_str() );
        pGraph->getStreet ( this->_event_removal_pair.second )->removeTrafficJam();
    }
    this->sendMessage ( "Thank you! Problem removed\n" );
}


auto User :: handlePositionUpdate () -> void {

    uint16 newPosition;
    if ( 0 >= read ( this->_client_fd, & newPosition, sizeof ( newPosition ) ) ) {
        return;
    }
    if ( this->_userPosition.getStreetId() != newPosition ) {
        this->_userPosition.setStreetId ( newPosition );
    }
}


auto User :: handleSpeedUpdate () -> void {

    uint8 newSpeed;
    if ( 0 >= read ( this->_client_fd, & newSpeed, sizeof ( newSpeed ) ) ) {
        return;
    }
    this->_userPosition.setSpeed ( newSpeed );
}


auto User :: handleGetWeather () -> void {

    this->sendMessage ( * ( * pCommonWeather->pWeatherString ) [ pCommonWeather->commonChoice ] );
}


auto User :: handleGetGasStations () -> void {

    auto pResults = pGraph->bfsTraversal (
            this->_userPosition.getStreetId(),
            & Node :: isGasStation
    );

    if ( pResults->empty() ) {
        this->sendMessage ( "Sorry, we couldn't find what you needed in your vicinity\n" );
    } else {
        std :: string resultString { "We found what you need at:\n"};
        for ( auto & e : * pResults ) {
            auto firstStreet = e->getStreetList()->begin();
            auto secondStreet = firstStreet ++;
            if ( ( * firstStreet )->getName() == ( * secondStreet )->getName() ) {
                ++ firstStreet;
            }
            resultString +=
                    "Gas station " + ( * pGasStationMap->pMap ) [ e->getId() ].first->toString() +
                    " with gas price " + roundf ( ( * pGasStationMap->pMap ) [ e->getId() ].second * 100 ) / 100 +
                    " at the intersection of streets " + ( * firstStreet )->getName() +
                    " and " + ( * secondStreet )->getName() + "\n";
        }
        this->sendMessage ( resultString );
    }

    delete pResults;
}


auto User :: handleSetSportNews () -> void {

    if ( this->_sport_news_enabled ) {
        this->sendMessage ( "Sport news already enabled\n" );
    } else {
        this->_sport_news_enabled = true;
        this->sendMessage ( "Sport news enabled\n" );
    }
}


auto User :: handleUnsetSportNews () -> void {

    if ( ! this->_sport_news_enabled ) {
        this->sendMessage ( "Sport news already disabled\n" );
    } else {
        this->_sport_news_enabled = false;
        this->sendMessage ( "Sport news disabled\n" );
    }
}


auto User :: handleGetSportNews () -> void {

    std :: ifstream fin;

    std :: string filePath = __COMMON_LIBS_HPP + "../resources/sport.txt";
    fin.open ( filePath.c_str() );

    auto selectedNews = randomGenerator.getRandomInRange (( uint8 )  16 );
    char s [ 3 ][ 256 ];

    for ( int i = 0; i <= selectedNews; ++ i ) {
        for ( auto & j : s) {
            fin.getline ( j, 256 );
        }
    }

    for ( auto & j : s) {
        std :: string stringedBuffer ( j );
        stringedBuffer += "\n";
        this->sendMessage ( stringedBuffer );
    }
}
#endif //CONCURRENT_SV_USER_IMPL_HPP
