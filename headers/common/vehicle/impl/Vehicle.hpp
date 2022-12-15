//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_VEHICLE_IMPL_HPP
#define CONCURRENT_SV_VEHICLE_IMPL_HPP

constexpr auto Vehicle :: getSpeed () const -> uint8 {

    return this->_speed;
}


constexpr auto Vehicle :: getStreetId () const -> uint8 {

    return this->_streetId;
}


auto Vehicle :: setSpeed ( uint8 newSpeed ) -> uint8 {

    this->_speed = newSpeed;
    return this->_speed;
}


auto Vehicle :: setStreetId ( uint8 newStreetId ) -> uint8 {

    this->_streetId = newStreetId;
    return this->_streetId;
}

#endif //CONCURRENT_SV_VEHICLE_IMPL_HPP
