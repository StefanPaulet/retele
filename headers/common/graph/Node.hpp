//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_HPP
#define CONCURRENT_SV_NODE_HPP

class Edge;

class Node {

private:        /* NOLINT(readability-redundant-access-specifiers) */
    uint16 _id;

private:        /* NOLINT(readability-redundant-access-specifiers) */
    bool _gas_station;

private:        /* NOLINT(readability-redundant-access-specifiers) */
    std :: unique_ptr < std :: list < Edge * > > _pIncidentStreetList = std :: make_unique < std :: list < Edge * > > ();

public:         /* NOLINT(readability-redundant-access-specifiers) */
    explicit Node (
            uint16 id,
            bool gas_station = false
    ) :
        _id          ( id ),
        _gas_station ( gas_station ) {

    }

public:     /* NOLINT(readability-redundant-access-specifiers) */
    [[nodiscard]] constexpr auto isGasStation () const -> bool;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    [[nodiscard]] auto getStreetList () const -> std :: list < Edge * > *;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    [[nodiscard]] auto getId () const -> uint16;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto setGasStation () -> void;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto add_incident_street (
            Edge * neighbour
    ) -> void;
};

#endif //CONCURRENT_SV_NODE_HPP
