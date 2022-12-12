//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_HPP
#define CONCURRENT_SV_NODE_HPP

#include <list>

class Node {

private:
    uint8 _id;

private:
    bool _gas_station;

private:
    std :: list < Node * > * _pNeighbourList { nullptr };

public:
    explicit constexpr Node (
            uint8 id,
            bool gas_station = false
    ) :
        _id          ( id ),
        _gas_station ( gas_station ) {

        this->_pNeighbourList = new std :: list < Node * >;
    }

public:
    auto setGasStation () -> void;

public:
    [[nodiscard]] constexpr auto isGasStation () const -> bool;

public:
    auto addNeighbour (
            Node * neighbour
    ) -> void;
};

#endif //CONCURRENT_SV_NODE_HPP
