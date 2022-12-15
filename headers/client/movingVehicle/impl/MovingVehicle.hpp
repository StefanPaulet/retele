//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP
#define CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP

auto MovingVehicle :: getRandomInRange ( uint8 maxValue ) -> uint8 {

    return ( * this->_pRandomNrGenerator )() % maxValue;
}

MovingVehicle :: MovingVehicle (
        uint8 const & speed,
        uint8 const & streetId
    ) : Vehicle (
        speed,
        streetId
) {

    this->_pGraph = new Graph();
    this->_pRandomNrGenerator = new std :: random_device;


}

#endif //CONCURRENT_SV_MOVING_VEHICLE_IMPL_HPP
