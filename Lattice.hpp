//
// Created by henry on 3/19/18.
//

#ifndef AUTOMATA_LATTICE_HPP
#define AUTOMATA_LATTICE_HPP

#include <cstddef>
#include <algorithm>

namespace Automata {
    template<class T, std::size_t _size, std::size_t _align = 0>
    struct Lattice {
    public:
        using type = T;
        static constexpr std::size_t size = _size;

        constexpr T& operator()(unsigned x, unsigned  y) {
            auto index = (y * size) + x;
            if (x >= size || y >= size)
                index = 0;
            return data[index];
        }

        constexpr T operator()(unsigned x, unsigned  y) const {
            auto index = (y * size) + x;
            if (x >= size || y >= size)
                index = 0;
            return data[index];
        }

        constexpr void swap(Lattice& other) {
            std::swap(data, other.data);
        }
//        template<class U>
//        constexpr U& as(unsigned x, unsigned  y) {
//            auto index = y * size + x;
//            if (index >= length)
//                index = 0;
//            return reinterpret_cast<U*>(data+index);
//        }
//        template<class U>
//        constexpr U as(unsigned x, unsigned  y) const {
//            auto index = y * size + x;
//            if (index >= length)
//                index = 0;
//            return *reinterpret_cast<U*>(data+index);
//        }
    private:
        static constexpr std::size_t length = _size * _size;
        alignas(_align) T data[length];
    };
}

#endif //AUTOMATA_LATTICE_HPP
