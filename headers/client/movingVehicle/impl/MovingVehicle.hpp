//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP
#define CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP


auto MovingVehicle :: pRandomNrGenerator = new std :: random_device;
auto MovingVehicle :: pGraph = new Graph();

auto MovingVehicle :: getRandomInRange ( uint8 maxValue ) -> uint8 {

    return ( ( * MovingVehicle :: pRandomNrGenerator )() % maxValue );
}

MovingVehicle :: MovingVehicle () :
    Vehicle (
        MovingVehicle :: getRandomInRange ( 21 ) + 30,
        MovingVehicle :: getRandomInRange ( pGraph->getEdgeCount() )
        ) {

    this->_nextNode = MovingVehicle :: pGraph->getStreet ( this->getStreetId() )->getEndNodes().first;
}


auto MovingVehicle :: chooseNextStreet () -> Edge * {

    auto possibleStreets = this->_nextNode->getStreetList();

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

    if ( this->_distancePercent >= 1.0f ) {

        auto nextStreet = this->chooseNextStreet();

        auto DEBUG_oldNode = this->_nextNode;

        this->setStreetId ( nextStreet->getId() );

        if ( this->_nextNode == nextStreet->getEndNodes().first ) {
            this->_nextNode = nextStreet->getEndNodes().second;
        } else {
            this->_nextNode = nextStreet->getEndNodes().first;
        }

        if ( DEBUG_oldNode != pGraph->getStreet ( this->getStreetId() )->getEndNodes().first &&
                DEBUG_oldNode != pGraph->getStreet ( this->getStreetId() )->getEndNodes().second ) {
            perror ("Wrong turn made\n");

            exit ( EXIT_FAILURE );
        }
        this->_distancePercent = 0.0f;
    } else {

        this->_distancePercent = this->_distancePercent + ( float ) (
                    ( float ) ( this->getSpeed() ) * 0.1f /
                    ( float ) pGraph->getStreet ( this->getStreetId() )->getLength()
                    ///TODO: set to 0.01f for immersion
                );
    }
}

#endif //CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP
