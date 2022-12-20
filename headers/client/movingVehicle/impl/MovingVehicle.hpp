//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP
#define CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP


auto MovingVehicle :: pRandomNrGenerator = new std :: random_device;
auto MovingVehicle :: pGraph = new Graph();

auto MovingVehicle :: getRandomInRange ( uint16 maxValue ) -> uint16 {

    return ( ( * MovingVehicle :: pRandomNrGenerator )() % maxValue );
}

MovingVehicle :: MovingVehicle () :
    Vehicle (
        MovingVehicle :: getRandomInRange ( 21 ) + 30,
        MovingVehicle :: getRandomInRange ( pGraph->getEdgeCount() )
        ) {

    this->_pNext_node = MovingVehicle :: pGraph->getStreet ( this->getStreetId() )->getEndNodes().first;
}


auto MovingVehicle :: chooseNextStreet () -> Edge * {

    auto possibleStreets = this->_pNext_node->getStreetList();

    auto choice = MovingVehicle :: getRandomInRange ( possibleStreets->size() );

    auto nextStreet = possibleStreets->begin();
    for ( auto i = 0; i < choice; ++ i, ++ nextStreet );

    auto changingStreetTryCount = 0;
    while ( ( * nextStreet )->getId() == this->getStreetId() && changingStreetTryCount < 2 ) {
        choice = MovingVehicle :: getRandomInRange ( possibleStreets->size() );
        nextStreet = possibleStreets->begin();
        for ( auto i = 0; i < choice; ++ i, ++ nextStreet );
        ++ changingStreetTryCount;
    }

    return * nextStreet;
}


auto MovingVehicle :: moveVehicle () -> void {

    if ( this->_distance_percent >= 1.0f ) {

        auto nextStreet = this->chooseNextStreet();

        auto DEBUG_oldNode = this->_pNext_node;

        this->setStreetId ( nextStreet->getId() );

        if ( this->_pNext_node == nextStreet->getEndNodes().first ) {
            this->_pNext_node = nextStreet->getEndNodes().second;
        } else {
            this->_pNext_node = nextStreet->getEndNodes().first;
        }

        if ( DEBUG_oldNode != pGraph->getStreet ( this->getStreetId() )->getEndNodes().first &&
                DEBUG_oldNode != pGraph->getStreet ( this->getStreetId() )->getEndNodes().second ) {
            perror ( "Wrong turn made\n" );

            exit ( EXIT_FAILURE );
        }
        this->_distance_percent = 0.0f;
    } else {

        this->_distance_percent = this->_distance_percent + ( float ) (
                    ( float ) ( this->getSpeed() ) * 0.1f /
                    ( float ) pGraph->getStreet ( this->getStreetId() )->getLength()
                );
    }
}


auto MovingVehicle :: changeSpeed () -> uint16 {

    if ( this->getSpeed() < 10 ) {
        this->setSpeed ( 20 );
        return this->getSpeed();
    }

    this->setSpeed ( this->getSpeed() + MovingVehicle :: getRandomInRange ( 5 ) - 2 );

    return this->getSpeed();
}

#endif //CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP
