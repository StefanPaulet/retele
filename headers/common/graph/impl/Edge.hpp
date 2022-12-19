//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_IMPL_HPP
#define CONCURRENT_SV_EDGE_IMPL_HPP

constexpr auto Edge :: getId () const -> uint16 {

    return this->_id;
}


constexpr auto Edge :: getEndNodes () const -> std :: pair < Node const *, Node const * > {

    return this->_end_nodes;
}


constexpr auto Edge :: getLength () const -> uint8 {

    return this->_length;
}


constexpr auto Edge :: getMaxSpeed () const -> uint8 {

    if ( this->_roadblock )
        return this->_max_speed - 20;
    if ( this->_traffic_jam )
        return this->_max_speed - 10;
    return this->_max_speed;
}


auto Edge :: getName () const -> std::string {

    return this->_name;
}


auto Edge :: isJammed () const -> bool {

    return this->_traffic_jam;
}


auto Edge :: isBlocked () const -> bool {

    return this->_roadblock;
}


auto Edge :: signalTrafficJam () -> void {

    this->_traffic_jam = true;
}


auto Edge :: signalRoadblock () -> void {

    this->_roadblock = true;
}


auto Edge :: removeTrafficJam () -> void {

    this->_traffic_jam = false;
}


auto Edge :: removeRoadblock () -> void {

    this->_roadblock = false;
}

#endif //CONCURRENT_SV_EDGE_IMPL_HPP
