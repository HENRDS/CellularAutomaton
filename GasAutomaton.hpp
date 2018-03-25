//
// Created by henry on 3/20/18.
//

#ifndef AUTOMATA_GASAUTOMATA_HPP
#define AUTOMATA_GASAUTOMATA_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include "Cellular.hpp"

namespace Automata {
  using ubyte = unsigned char;
  class GasAutomaton : public CellularAutomata<ubyte, 400> {
  public:
      GasAutomaton(size_t totalSteps, std::string outputPath);
      virtual ~GasAutomaton() noexcept;

  private:
      enum Direction : unsigned {
          North = 0, South = 1, East = 2, West = 3, Solid = 4
      };

      cv::Mat frame;
      cv::VideoWriter writer;
      std::string outputPath;
      void init() noexcept override;
      void applyRule() override;
      void draw() override;

      template<unsigned n>
      inline constexpr void set(unsigned x, unsigned y, bool value = true) {
          static_assert(n < 8, "Invalid offset for byte");
          if (value)
              _nextLattice(x, y) |= (1U << n);
          else
              _nextLattice(x, y) &= ~(1U << n);
      }

      template<unsigned n>
      inline constexpr bool get(unsigned x, unsigned y) {
          static_assert(n < 8, "Invalid offset for byte");
          return (_currentLattice(x, y) & (1U << n)) > 0;
      }
  };
}

#endif //AUTOMATA_GASAUTOMATA_HPP
