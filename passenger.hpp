/**
 * IMS project
 *
 * @file passenget.hpp
 * @brief Class declaration for passenger process and its generator
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include "tram_stop.hpp"
#include <simlib.h>

extern double acceleration;

/// Passenger process includes waiting for lift, available capsule, available
/// landing place, capsule entering and leaving, and the trip itself
class Passenger : public Process {
public:
  Passenger(TramStop *InitialStop);
  void Behavior();

private:
  double Arrival;
  TramStop *Initial;
};

/// Generates Passenger processes based on estimetad daily arrivals of specific
/// stop
class PassengerGenerator : public Event {
public:
  PassengerGenerator(TramStop *corresponding);
  void Behavior();

private:
  TramStop *stop;
};

#endif
