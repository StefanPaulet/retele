//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_HPP
#define CONCURRENT_SV_EDGE_HPP

#include <utility>
#include "Node.hpp"

class Edge {

private:
    std :: pair < Node, Node > _endNodes;

private:
    uint8 _maxSpeed { 30 };

private:
    uint8 _length;

private:
    std :: string _name;

private:
    uint8 _event_flag { 0 };

public:
    Edge (
            Node const & firstEnd,
            Node const & secondEnd,
            uint8 maxSpeed,
            uint8 length,
            std :: string && name
    ) :
        _endNodes ( firstEnd, secondEnd ),
        _maxSpeed ( maxSpeed ),
        _length   ( length ),
        _name     ( std :: move ( name ) ) {

    }

public:
    [[nodiscard]] constexpr auto getEndNodes () const -> std :: pair < Node, Node >;

public:
    [[nodiscard]] constexpr auto getLength () const -> uint8;

public:
    [[nodiscard]] constexpr auto getMaxSpeed () const -> uint8;

public:
    [[nodiscard]] auto getName () const -> std :: string;
};

#include "impl/Node.hpp"

#endif //CONCURRENT_SV_EDGE_HPP
