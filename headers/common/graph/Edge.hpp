//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_HPP
#define CONCURRENT_SV_EDGE_HPP

#include "Node.hpp"

class Edge {

private:
    uint16 _id;

private:
    std :: pair < Node const *, Node const * > _endNodes;

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
            uint16 id,
            Node const * firstEnd,
            Node const * secondEnd,
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
    [[nodiscard]] constexpr auto getId () const -> uint16;

public:
    [[nodiscard]] constexpr auto getEndNodes () const -> std :: pair < Node const *, Node const * >;

public:
    [[nodiscard]] constexpr auto getLength () const -> uint8;

public:
    [[nodiscard]] constexpr auto getMaxSpeed () const -> uint8;

public:
    [[nodiscard]] auto getName () const -> std :: string;

public:
    [[nodiscard]] auto isJammed () const -> bool;

public:
    [[nodiscard]] auto isBlocked () const -> bool;

public:
    auto signal_traffic_jam () -> void;

public:
    auto signal_roadblock () -> void;

public:
    auto remove_traffic_jam () -> void;

public:
    auto remove_roadblock () -> void;
};

#include "impl/Node.hpp"

#endif //CONCURRENT_SV_EDGE_HPP
