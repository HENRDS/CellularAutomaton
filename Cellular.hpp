//
// Created by henry on 3/20/18.
//

#ifndef AUTOMATA_CELLULAR_HPP
#define AUTOMATA_CELLULAR_HPP

#include <random>
#include "Lattice.hpp"

namespace Automata {
  template<class T, ::std::size_t _size, bool auto_swap = false>
  class CellularAutomaton {
  public:
      static constexpr std::size_t size = _size;
      void execute() {
          while ((this->_currentStep <= this->_totalSteps) && !_hasErrorOcurred) {
              this->applyRule();
              if constexpr (auto_swap)
                  this->_currentLattice.swap(this->_nextLattice);
          }
      }

      void step() {
          this->applyRule();
          if constexpr (auto_swap)
              this->_currentLattice.swap(this->_nextLattice);
      }

      void stop() {
          this->_currentStep = this->_totalSteps + 1;
      }

      void reset() {
          this->_currentStep = 0;
          init();
      }

      void reset(std::size_t totalSteps) {
          this->_totalSteps = totalSteps;
          this->reset();
      }

      std::size_t currentStep() const {
          return this->_currentStep;
      }

      std::size_t totalSteps() const {
          return this->_totalSteps;
      }

  protected:
      std::size_t _totalSteps;
      std::size_t _currentStep;
      Lattice<T, _size> _currentLattice;
      Lattice<T, _size> _nextLattice;
      bool _hasErrorOcurred;
      explicit CellularAutomaton(std::size_t totalSteps)
          : _totalSteps(totalSteps), _currentStep(0), _hasErrorOcurred(false) {}

      virtual void applyRule() = 0;
      virtual void init() noexcept = 0;
      virtual void draw() = 0;

  };
}

#endif //AUTOMATA_CELLULAR_HPP
