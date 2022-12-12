//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_IMPL_HPP
#define CONCURRENT_SV_USER_IMPL_HPP

AtomicQueue * User :: pEventQueue = new AtomicQueue ();

Graph * User :: pGraph = new Graph ();

std :: array < std :: string, 5 > const User :: signalNamesArray {
    "roadblock",
    "police",
    "wet-ground",
    "frozen-ground",
    "traffic-jam"
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

            case __BAD_REQUEST : {
                this->send_msg ( "No such command available\n" );
                break;
            }
            case __NO_PARAM_REQUEST : {
                this->send_msg ( "Command must have parameters\n" );
                break;
            }
            case __SIGNAL : {
                this->handle_signal();
                break;
            }
            case __GET_GP : {
                this->send_msg ( "Here are the gas prices\n" );
                break;
            }
            case __ENABLE_S : {
                this->send_msg ( "You will receive sports news\n" );
                break;
            }
            case __ENABLE_W : {
                this->send_msg ( "You will receive weather news\n" );
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
//            case __TIMED_SPEED_UPDATE : {
//                this->send_msg ( "Nice speed bro\n" );
//                break;
//            }
//            case __TIMED_SPORTS_UPDATE : {
//                this->send_msg ( "WC going on right now\n" );
//                break;
//            }
//            case __TIMED_WEATHER_UPDATE : {
//                this->send_msg ( "It's a beautiful day\n" );
//                break;
//            }
            default : {
                break;
            }
        }

    }

    Server :: getInstance ()->disconnect_client ( pthread_self() );

}


auto User :: handle_signal () -> void {

    char buffer [ __STANDARD_BUFFER_SIZE ];
    read ( this->_client_fd, buffer, __STANDARD_BUFFER_SIZE );

    std :: string stringedBuffer { buffer };

    for ( auto & e : signalNamesArray ) {
        if ( e == stringedBuffer ) {

            this->send_msg ( "Thank you for your signal\n" );

            stringedBuffer += " on" + pGraph->getStreet ( this->_street_id )->getName() + "\n";

            pEventQueue->push_back ( std :: move ( stringedBuffer ) );

            if ( e == "roadblock" ) {
                pGraph->getStreet ( this->_street_id )->signalRoadBlock();
            } else {
                if ( e == "traffic-jam" ) {
                    pGraph->getStreet ( this->_street_id )->signalTrafficJam();
                }
            }

            return;
        }
    }

    this->send_msg ( "Sorry, no such signal available\n" );

}


auto User :: handle_event_update () -> void {

    if ( this->_pLocalEventNode != pEventQueue->back() ) {
        std :: string event = "Attention " + this->_pLocalEventNode->_message;
        this->send_msg ( event );
        this->_pLocalEventNode = this->_pLocalEventNode->_pNext;
    }
}


auto User :: handle_speed_limit_update () -> void {

    auto speedLimit = pGraph->getStreet ( this->_street_id )->getMaxSpeed();
    this->send_msg ( "Speed limit is " + std :: to_string ( speedLimit ) + "\n" );
}

#endif //CONCURRENT_SV_USER_IMPL_HPP
