//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_HPP
#define CONCURRENT_SV_NODE_HPP

#include <memory>

class Edge;

class Node {

private:
    uint16 _id;

    bool _gas_station;

    std :: unique_ptr < std :: list < Edge * > > _pIncident_street_list = std :: make_unique < std :: list < Edge * > > ();

public:
    explicit Node (
            uint16 id,
            bool gas_station = false
    ) :
        _id          ( id ),
        _gas_station ( gas_station ) {

    }

    [[nodiscard]] constexpr auto isGasStation () const -> bool;

    [[nodiscard]] auto getStreetList () const -> std :: list < Edge * > *;

    [[nodiscard]] auto getId () const -> uint16;

    auto setGasStation () -> void;

    auto add_incident_street (
            Edge * neighbour
    ) -> void;
};

#endif //CONCURRENT_SV_NODE_HPP
