//
// Created by stefan on 14.12.2022.
//

#ifndef CONCURRENT_SV_MOVING_VEHICLE_HPP
#define CONCURRENT_SV_MOVING_VEHICLE_HPP

#include <random>
#include "../../common/graph/Graph.hpp"
#include "../../common/vehicle/Vehicle.hpp"

class MovingVehicle : public Vehicle {

private:    /* NOLINT(readability-redundant-access-specifiers) */
    static std :: random_device * pRandomNrGenerator;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    static Graph * pGraph;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    float _distancePercent { 0.0f };

private:    /* NOLINT(readability-redundant-access-specifiers) */
    Node const * _nextNode { nullptr };

private:    /* NOLINT(readability-redundant-access-specifiers) */
    static auto getRandomInRange ( uint8 maxValue ) -> uint8;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    MovingVehicle ();

private:    /* NOLINT(readability-redundant-access-specifiers) */
    auto chooseNextStreet () -> Edge *;

public:
    auto moveVehicle () -> void;

};


#include "../../common/graph/impl/Graph.hpp"
#include "../../common/vehicle/impl/Vehicle.hpp"

#endif //CONCURRENT_SV_MOVING_VEHICLE_HPP
