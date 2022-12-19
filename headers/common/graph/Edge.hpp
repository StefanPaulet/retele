//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_HPP
#define CONCURRENT_SV_EDGE_HPP

#include "Node.hpp"

class Edge {

private:
    uint16 _id;

    std :: pair < Node const *, Node const * > _end_nodes;

    uint8 _max_speed { 30 };

    uint8 _length;

    std :: string _name;

    bool _traffic_jam { false };

    bool _roadblock { false };

public:
    Edge (
            uint16 id,
            Node const * firstEnd,
            Node const * secondEnd,
            uint8 maxSpeed,
            uint8 length,
            std :: string && name
    ) :
        _id       ( id ),
        _end_nodes ( firstEnd, secondEnd ),
        _max_speed ( maxSpeed ),
        _length   ( length ),
        _name     ( std :: move ( name ) ) {

    }

    [[nodiscard]] constexpr auto getId () const -> uint16;

    [[nodiscard]] constexpr auto getEndNodes () const -> std :: pair < Node const *, Node const * >;

    [[nodiscard]] constexpr auto getLength () const -> uint8;

    [[nodiscard]] constexpr auto getMaxSpeed () const -> uint8;

    [[nodiscard]] auto getName () const -> std :: string;

    [[nodiscard]] auto isJammed () const -> bool;

    [[nodiscard]] auto isBlocked () const -> bool;

    auto signalTrafficJam () -> void;

    auto signalRoadblock () -> void;

    auto removeTrafficJam () -> void;

    auto removeRoadblock () -> void;
};

#include "impl/Node.hpp"

#endif //CONCURRENT_SV_EDGE_HPP
