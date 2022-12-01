//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_IMPL_HPP
#define CONCURRENT_SV_USER_IMPL_HPP

std :: shared_ptr < AtomicQueue > User :: pEventQueue = std :: make_shared < AtomicQueue > ();

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

        printf ( "Client request %d\n", request_nr );

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
//            case __TIMED_EVENTS_UPDATE : {
//                this->send_msg ( "No event for you\n" );
//                break;
//            }
//            case __TIMED_SPEED_LIMIT_UPDATE : {
//                this->send_msg ( "You will get your speed limit soon\n" );
//                break;
//            }
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

auto User :: handle_signal () -> int {

    char buffer [ __STANDARD_BUFFER_SIZE ];

    read ( this->_client_fd, buffer, __STANDARD_BUFFER_SIZE );
    this->send_msg ( "Thank you for your signal" );
    printf ("Thread %d signaled %s\n", this->_client_fd, buffer );
}

#endif //CONCURRENT_SV_USER_IMPL_HPP