//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_HPP
#define CONCURRENT_SV_EDGE_HPP

#include <utility>
#include "Node.hpp"

class Edge {

private:
    uint8 _id;

private:
    std :: pair < Node, Node > _endNodes;

private:
    uint8 _maxSpeed { 30 };

private:
    uint8 _length;

private:
    std :: string _name;

private:
    bool _traffic_jam { false };

private:
    bool _roadblock { false };

public:
    Edge (
            uint8 id,
            Node const & firstEnd,
            Node const & secondEnd,
            uint8 maxSpeed,
            uint8 length,
            std :: string && name
    ) :
        _id       ( id ),
        _endNodes ( firstEnd, secondEnd ),
        _maxSpeed ( maxSpeed ),
        _length   ( length ),
        _name     ( std :: move ( name ) ) {

    }


public:
    [[nodiscard]] constexpr auto getId () const -> uint8;

public:
    [[nodiscard]] constexpr auto getEndNodes () const -> std :: pair < Node, Node >;

public:
    [[nodiscard]] constexpr auto getLength () const -> uint8;

public:
    [[nodiscard]] constexpr auto getMaxSpeed () const -> uint8;

public:
    [[nodiscard]] auto getName () const -> std :: string;

public:
    auto signalTrafficJam () -> void;

public:
    auto signalRoadBlock () -> void;

public:
    auto removeTrafficJam () -> void;

public:
    auto removeRoadBlock () -> void;
};

#include "impl/Node.hpp"

#endif //CONCURRENT_SV_EDGE_HPP
