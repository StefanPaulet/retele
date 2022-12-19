//
// Created by stefan on 19.12.2022.
//

#ifndef CONCURRENT_SV_RANDOM_GENERATOR_HPP
#define CONCURRENT_SV_RANDOM_GENERATOR_HPP

#include <random>
#include <memory>

class RandomGenerator {

private:
    std :: unique_ptr < std :: random_device > pGenerator = std ::make_unique < std :: random_device > ();
public:

    [[nodiscard]] auto getRandomInRange ( uint8 maxValue ) const -> uint8 {
        return ( * this->pGenerator )() % maxValue;
    }

    [[nodiscard]] auto getRandomInRange ( uint16 maxValue ) const -> uint16 {
        return ( * this->pGenerator )() % maxValue;
    }
};

#endif //CONCURRENT_SV_RANDOM_GENERATOR_HPP
