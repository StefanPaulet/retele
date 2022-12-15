//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_IMPL_HPP
#define CONCURRENT_SV_USER_IMPL_HPP

AtomicQueue * User :: pEventQueue = new AtomicQueue ();

Graph * User :: pGraph = new Graph ();

std :: array < std :: string, 6 > const User :: signalNamesArray {
    "roadblock",
    "police",
    "accident",
    "wet-ground",
    "frozen-ground",
    "crowded"
};


auto User :: send_msg ( std :: string const & message ) const -> void {

    auto messageSize = message.size();

    write ( this->_client_fd, & messageSize, sizeof ( messageSize ) );
    write ( this->_client_fd, message.c_str(), messageSize );
}

auto User :: handle_request () -> void {

    bool clientConnected = true;
    sint32 request_nr;

    while ( clientConnected ) {

        if ( 0 >= read ( this->_client_fd, & request_nr, sizeof ( int ) ) ) {
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
            case __EVENT_MISSING : {
                if ( this->_waiting_response > 0 ) {
                    this->remove_street_event ( this->_waiting_response );
                    break;
                }
            }
            case __TIMED_SPORTS_UPDATE :
            case __TIMED_WEATHER_UPDATE :
            case __TIMED_SPEED_UPDATE : {
                break;
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
    read ( this->_client_fd, buffer, __STANDARD_BUFFER_SIZE );
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
        std :: string event = "Attention! " + this->_pLocalEventNode->_message;
        this->send_msg ( event );
        this->_pLocalEventNode = this->_pLocalEventNode->_pNext;
    }
}


auto User :: handle_speed_limit_update () -> void {

    auto speedLimit = pGraph->getStreet ( this->_userPosition.getStreetId() )->getMaxSpeed();
    if ( speedLimit > this->_userPosition.getSpeed() ) {
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

#endif //CONCURRENT_SV_USER_IMPL_HPP
