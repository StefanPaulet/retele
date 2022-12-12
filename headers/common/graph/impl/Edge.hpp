//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_EDGE_IMPL_HPP
#define CONCURRENT_SV_EDGE_IMPL_HPP

constexpr auto Edge :: getEndNodes () const -> std::pair < Node, Node > {

    return this->_endNodes;
}

constexpr auto Edge :: getLength () const -> uint8 {

    return this->_length;
}


constexpr auto Edge :: getMaxSpeed () const -> uint8 {

    return this->_maxSpeed;
}


auto Edge :: getName () const -> std::string {

    return this->_name;
}

#endif //CONCURRENT_SV_EDGE_IMPL_HPP
