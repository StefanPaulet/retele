//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_VEHICLE_IMPL_HPP
#define CONCURRENT_SV_VEHICLE_IMPL_HPP

constexpr auto Vehicle :: getSpeed () const -> uint8 {

    return this->_speed;
}


constexpr auto Vehicle :: getStreetId () const -> uint16 {

    return this->_street_id;
}


auto Vehicle :: setSpeed ( uint8 newSpeed ) -> uint8 {

    this->_speed = newSpeed;
    return this->_speed;
}


auto Vehicle :: setStreetId ( uint16 newStreetId ) -> uint16 {

    this->_street_id = newStreetId;
    return this->_street_id;
}

#endif //CONCURRENT_SV_VEHICLE_IMPL_HPP
