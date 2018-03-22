#include <iostream>
#include "GasAutomaton.hpp"

int main(int argc, char* argv[]) {
  unsigned iterations = 3000;
  if (argc > 1)
    iterations = std::stoi(argv[1]);
  std::cout << "Starting" << std::endl;
  Automata::GasAutomaton gasAutomaton(iterations, "./test.avi");
  gasAutomaton.execute();
  return 0;
}