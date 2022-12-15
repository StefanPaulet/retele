//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_MOVING_VEHICLE_HPP
#define CONCURRENT_SV_MOVING_VEHICLE_HPP

#include <random>
#include "../../common/graph/Graph.hpp"
#include "../../common/vehicle/Vehicle.hpp"

class MovingVehicle : public Vehicle {

private:
    Graph * _pGraph;

private:
    float _distancePercent { 0.0f };

private:
    std :: random_device * _pRandomNrGenerator;

private:
    Node * _nextNode { nullptr };

private:
    auto getRandomInRange ( uint8 maxValue ) -> uint8;

public:
    MovingVehicle (
            uint8 const & speed,
            uint8 const & streetId
    );

public:


};


#include "../../common/graph/impl/Graph.hpp"
#include "../../common/vehicle/impl/Vehicle.hpp"

#endif //CONCURRENT_SV_MOVING_VEHICLE_HPP
