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
    static std :: random_device * pRandomNrGenerator;

    static Graph * pGraph;

    float _distance_percent { 0.0f };

    Node const * _pNext_node { nullptr };

    static auto getRandomInRange ( uint16 maxValue ) -> uint16;

public:
    MovingVehicle ();

private:
    auto chooseNextStreet () -> Edge *;

public:
    auto moveVehicle () -> void;

    auto changeSpeed () -> uint16;

};


#include "../../common/graph/impl/Graph.hpp"
#include "../../common/vehicle/impl/Vehicle.hpp"

#endif //CONCURRENT_SV_MOVING_VEHICLE_HPP
