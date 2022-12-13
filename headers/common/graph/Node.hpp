//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_HPP
#define CONCURRENT_SV_NODE_HPP

#include <list>

class Edge;

class Node {

private:
    uint8 _id;

private:
    bool _gas_station;

private:
    std :: list < Edge * > * _pIncidentStreetList { nullptr };

public:
    explicit constexpr Node (
            uint8 id,
            bool gas_station = false
    ) :
        _id          ( id ),
        _gas_station ( gas_station ) {

        this->_pIncidentStreetList = new std :: list < Edge * >;
    }

public:
    auto setGasStation () -> void;

public:
    [[nodiscard]] constexpr auto isGasStation () const -> bool;

public:
    auto add_incident_street (
            Edge * neighbour
    ) -> void;
};

#endif //CONCURRENT_SV_NODE_HPP
