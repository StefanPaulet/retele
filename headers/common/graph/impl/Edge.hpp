//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_IMPL_HPP
#define CONCURRENT_SV_EDGE_IMPL_HPP

constexpr auto Edge :: getId () const -> uint16 {

    return this->_id;
}


constexpr auto Edge :: getEndNodes () const -> std :: pair < Node const *, Node const * > {

    return this->_endNodes;
}


constexpr auto Edge :: getLength () const -> uint8 {

    return this->_length;
}


constexpr auto Edge :: getMaxSpeed () const -> uint8 {

    if ( this->_roadblock )
        return this->_maxSpeed - 20;
    if ( this->_traffic_jam )
        return this->_maxSpeed - 10;
    return this->_maxSpeed;
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


auto Edge :: signal_traffic_jam () -> void {

    this->_traffic_jam = true;
}


auto Edge :: signal_roadblock () -> void {

    this->_roadblock = true;
}


auto Edge :: remove_traffic_jam () -> void {

    this->_traffic_jam = false;
}


auto Edge :: remove_roadblock () -> void {

    this->_roadblock = false;
}

#endif //CONCURRENT_SV_EDGE_IMPL_HPP
