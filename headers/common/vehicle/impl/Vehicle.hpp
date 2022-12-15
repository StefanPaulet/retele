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

#endif //CONCURRENT_SV_VEHICLE_IMPL_HPP
