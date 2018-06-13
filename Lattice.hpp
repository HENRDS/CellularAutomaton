//
// Created by henry on 3/19/18.
//

#ifndef AUTOMATA_LATTICE_HPP
#define AUTOMATA_LATTICE_HPP

#include <cstddef>
#include <algorithm>

namespace Automata {
    template<class T, std::size_t _width, std::size_t _height = _width, std::size_t _align = 0>
    struct Lattice {
    public:
        using value_type = T;
        static constexpr auto height = _height;
        static constexpr auto width =  _width;

        value_type& operator()(std::size_t x, std::size_t  y) {
            auto index = (y * width) + x;
            if ((x >= width) || (y >= height))
                throw std::out_of_range("out of range");
            return data[index];
        }

        value_type operator()(std::size_t x, std::size_t  y) const {
            auto index = (y * width) + x;
            if ((x >= width) || (y >= height))
                throw std::out_of_range("out of range");
            return data[index];
        }

        void swap(Lattice& other) {
            std::swap(data, other.data);
        }
    private:
        alignas(_align) value_type data[height * width];
    };
}

#endif //AUTOMATA_LATTICE_HPP
