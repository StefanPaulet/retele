//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_GRAPH_HPP
#define CONCURRENT_SV_GRAPH_HPP

#include <list>
#include <queue>
#include <functional>
#include <CDS/util/JSON>
#include <CDS/filesystem/Path>

#include "Edge.hpp"

class Graph {

private:
    uint16 _node_count;

    uint16 _edge_count;

    Node * * _pNode_list;

    Edge * * _pEdge_list;

public:
    Graph ();

    ~Graph ();

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
