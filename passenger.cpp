/**
 * IMS project
 *
 * @file passenger.cpp
 * @brief Implementation of passenger process and its generator
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#include "passenger.hpp"
#include "capsule.hpp"
#include <cmath>
#include <cstdio>
#include <simlib.h>

// Default value if is not specified in the program arguments (m/s^2)
double acceleration = 2.;

// Default value (m/s)
double max_speed = 333.;

// the bigger it gets, the worse it gets in terms of comfort
// small numbers mean more curvy and gradually change
// j = a/t
double jerk = 1;

Passenger::Passenger(TramStop *InitialStop) { this->Initial = InitialStop; }

void Passenger::Behavior() {
  auto started = this->Initial->GetName();
  Arrival = Time;

  // Find all subsequent stops
  std::vector<TramStop *> possible_destinations;
  TramStop *scanner = this->Initial->GetNext();

  while (scanner != NULL) {
    possible_destinations.push_back(scanner);
    scanner = scanner->GetNext();
  }

  if (possible_destinations.empty()) {
    return;
  }

  // Randomly selects a destination with a given probability
  // REMAINING_PERCANTAGE
  int random_index = 0;
  while (random_index < static_cast<int>(possible_destinations.size()) - 1) {
    if (Random() < 1 - REMAINING_PERCANTAGE)
      break;
    random_index++;
  }

  // Calculate total distance from the initial to destination stop
  TramStop *Destination = possible_destinations[random_index];

  double total_distance = 0;
  TramStop *calc_cursor = this->Initial;

  while (calc_cursor != Destination) {
    calc_cursor->counter++;
    total_distance += calc_cursor->GetDistanceToTheNextStop();
    calc_cursor = calc_cursor->GetNext();
  }

  // Calculate travel time from the initial to destination stop
  double total_time;

  double minimal_distance_for_max_speed =
      (max_speed * max_speed) / acceleration;
  if (total_distance < minimal_distance_for_max_speed)
    total_time = std::sqrt(total_distance / acceleration) * 2;
  else // if it will surpass maximum speed
    total_time = total_distance / max_speed + max_speed / acceleration;

  total_time += acceleration / jerk;

  // Trying to occupy lift to enter into capsule
try_enter_lift:
  auto enter_lift = this->Initial->TryEnter(this);
  if (enter_lift == NULL) {
    Passivate();
    goto try_enter_lift;
  } else {
    this->Seize(*enter_lift);
  }

  // Enter the capsule
  auto occupied_capsule = this->Initial->GetCapsules();
  occupied_capsule->Enter(this, 1);
  this->Initial->StatEnter(Time - Arrival);
  Wait(Uniform(8, 10));

  // Start the journey if there is available landing place at the destination
  // stop
  auto time_before_depart = Time;
  auto landing_place = Destination->GetLandingPlaces();
  landing_place->Enter(this, 1);
  this->Initial->StatDepart(Time - time_before_depart);

  // Lift is available for next passenger
  this->Release(*enter_lift);
  this->Initial->FinishEnter();

  Wait(total_time);

  // Leaving capsule
  Wait(Uniform(8, 10));

  if (Destination) {
    this->Initial->StatTravel(Destination->GetName(), Time - Arrival);
  }

  // Passenger process has ended
  // Start process of capsule return
  (new CapsuleReturn(occupied_capsule, landing_place, total_time))->Activate();
}

PassengerGenerator::PassengerGenerator(TramStop *corresponding) {
  this->stop = corresponding;
}

void PassengerGenerator::Behavior() {
  (new Passenger(this->stop))->Activate();
  Activate(Time +
           Exponential(double(64800) / this->stop->GetEstimatedArrivals()));
}
