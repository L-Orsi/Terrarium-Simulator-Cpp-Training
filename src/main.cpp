#include <iostream>
#include <memory>

#include <simulator/simulation_engine.hpp>

int main()
{
    std::unique_ptr<simulation::SimulationEngine> test_ptr = std::make_unique<simulation::SimulationEngine>();
    test_ptr->populate_map_with_species();
    test_ptr->start_simulation(/*n_rounds=*/10);
    return 0;
}
