//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_VEHICLE_HPP
#define CONCURRENT_SV_VEHICLE_HPP


class Vehicle {

private:
    uint8 _speed;

private:
    uint8 _streetId;

public:
    Vehicle (
            uint8 const & speed,
            uint8 const & streetId
    ) : _speed    ( speed ),
        _streetId ( streetId ) {

    }

public:
    [[nodiscard]] constexpr auto getSpeed () const -> uint8;

public:
    [[nodiscard]] constexpr auto getStreetId () const -> uint8;

public:
    auto setSpeed ( uint8 newSpeed ) -> uint8;
public:
    auto setStreetId ( uint8 newStreetId ) -> uint8;
};


#endif //CONCURRENT_SV_VEHICLE_HPP
