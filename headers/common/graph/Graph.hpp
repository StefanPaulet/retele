//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_GRAPH_HPP
#define CONCURRENT_SV_GRAPH_HPP

#include "Edge.hpp"

class Graph {

private:
    uint16 _nodeCount;

private:
    uint16 _edgeCount;

private:
    Node * * _pNodeList;

private:
    Edge * * _pEdgeList;

public:
    Graph();

public:
    auto constexpr getStreet (
            int const & streetId
    ) -> Edge * ;

public:
    [[nodiscard]] auto constexpr getEdgeCount () const -> uint16;
};

#include "impl/Edge.hpp"


#endif //CONCURRENT_SV_GRAPH_HPP
