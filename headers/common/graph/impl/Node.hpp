//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_IMPL_HPP
#define CONCURRENT_SV_NODE_IMPL_HPP

constexpr auto Node :: isGasStation () const -> bool {

    return this->_gas_station;
}


auto Node :: getStreetList () const -> std :: list < Edge * > * {

    return this->_pIncident_street_list.get();
}


auto Node :: getId () const -> uint16 {

    return this->_id;
}


auto Node :: setGasStation () -> void {

    this->_gas_station = true;
}


inline auto Node :: add_incident_street (
        Edge * neighbour
    ) -> void {

    this->_pIncident_street_list->push_back ( neighbour );
}

#endif //CONCURRENT_SV_NODE_IMPL_HPP
