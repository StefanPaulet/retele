//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_HPP
#define CONCURRENT_SV_NODE_HPP

class Node {

private:
    uint8 _id;

private:
    bool _gas_station;

public:
    constexpr Node (
            uint8 id,
            bool gas_station = false
    ) :
        _id          ( id ),
        _gas_station ( gas_station ) {

    }

public:
    auto setGasStation () -> void;

public:
    [[nodiscard]] constexpr auto isGasStation () const -> bool;
};

#endif //CONCURRENT_SV_NODE_HPP
