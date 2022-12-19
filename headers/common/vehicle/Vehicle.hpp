//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_VEHICLE_HPP
#define CONCURRENT_SV_VEHICLE_HPP

class Vehicle {

private:
    uint8 _speed;

    uint16 _street_id;

public:
    Vehicle (
            uint8 const & speed,
            uint16 const & streetId
    ) : _speed    ( speed ),
        _street_id ( streetId ) {

    }

    [[nodiscard]] constexpr auto getSpeed () const -> uint8;

    [[nodiscard]] constexpr auto getStreetId () const -> uint16;

    auto setSpeed ( uint8 newSpeed ) -> uint8;

    auto setStreetId ( uint16 newStreetId ) -> uint16;
};


#endif //CONCURRENT_SV_VEHICLE_HPP
