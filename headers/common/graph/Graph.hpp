//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_GRAPH_HPP
#define CONCURRENT_SV_GRAPH_HPP

#include "Edge.hpp"
#include <queue>
#include <functional>
#include <CDS/util/JSON>
#include <CDS/filesystem/Path>

class Graph {

private:
    uint16 _nodeCount;

    uint16 _edgeCount;

    Node * * _pNodeList;

    Edge * * _pEdgeList;

public:
    Graph();

    auto constexpr getStreet ( int const & streetId ) -> Edge * ;

    [[nodiscard]] auto constexpr getEdgeCount () const -> uint16;

    [[nodiscard]] auto constexpr getNodeCount () const -> uint16;

    auto bfsTraversal (
            uint16              streetId,
            const std :: function < bool ( Node const * ) > & predicate
    ) -> std :: list < Node const * > *;
};

#include "impl/Edge.hpp"


#endif //CONCURRENT_SV_GRAPH_HPP
