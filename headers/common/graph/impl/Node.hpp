//
// Created by stefan on 03.12.2022.
//

#ifndef CONCURRENT_SV_NODE_IMPL_HPP
#define CONCURRENT_SV_NODE_IMPL_HPP

auto Node :: setGasStation () -> void {

    this->_gas_station = true;
}

constexpr auto Node :: isGasStation () const -> bool {

    return this->_gas_station;
}

#endif //CONCURRENT_SV_NODE_IMPL_HPP
