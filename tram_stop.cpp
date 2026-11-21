/**
 * IMS project
 *
 * @file tram_stop.cpp
 * @brief Implementation of tram stop class and its generation function
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#include "tram_stop.hpp"
#include <simlib.h>
#include <vector>

int capsules_count = 2;
int landing_count = 5;

TramStop::TramStop(std::string StopName, double distance,
                   int estimated_arrivals)
    : capsules((StopName + " capsules").c_str(), capsules_count),
      landing_places((StopName + " landing").c_str(), landing_count),
      passenger_wait_enter(("Waiting for enter on " + StopName).c_str()),
      passenger_wait_depart(("Waiting for depart from " + StopName).c_str()) {
  this->distance = distance;
  this->estimated_arrivals = estimated_arrivals;
  this->next = NULL;
  this->name = StopName;
  this->counter = 0;
}

void TramStop::SetNext(TramStop *NextStop) { this->next = NextStop; }

TramStop *TramStop::GetNext() { return this->next; }

double TramStop::GetDistanceToTheNextStop() { return this->distance; };

std::string TramStop::GetName() { return this->name; }

int TramStop::GetEstimatedArrivals() { return this->estimated_arrivals; };

Facility *TramStop::TryEnter(Process *proc) {
  if (!this->enter_lift.Busy())
    return &this->enter_lift;
  proc->Into(this->enter_queue);
  return NULL;
}

void TramStop::FinishEnter() {
  if (!this->enter_queue.empty()) {
    (this->enter_queue.GetFirst())->Activate();
  }
}

Store *TramStop::GetCapsules() { return this->capsules; }

Store *TramStop::GetLandingPlaces() { return this->landing_places; }

void TramStop::InitStat() {
  auto cursor = this->GetNext();
  while (cursor != NULL) {
    this->travel_stats.push_back(new Stat(cursor->GetName().c_str()));
    cursor = cursor->GetNext();
  }
}

void TramStop::StatTravel(std::string name, double value) {
  for (auto stat : this->travel_stats) {
    if (stat->Name() == name) {
      (*stat)(value);
      return;
    }
  }
}

void TramStop::StatEnter(double value) { this->passenger_wait_enter(value); }

void TramStop::StatDepart(double value) { this->passenger_wait_depart(value); }

void TramStop::StatTravelOutput() {
  for (auto &stat : this->travel_stats) {
    Print("%s %s %lf %lf %lf\n", this->name.c_str(), stat->Name().c_str(),
          stat->MeanValue(), stat->Max(), stat->Min());
  }
}

void TramStop::StatEnterOutput() {
  if (this->passenger_wait_enter.Number() > 0) {
    Print("%s %lf %lf %lf %lu\n", this->name.c_str(),
          this->passenger_wait_enter.MeanValue(),
          this->passenger_wait_enter.Max(), this->passenger_wait_enter.Min(),
          this->passenger_wait_enter.Number());
  }
}

void TramStop::StatDepartOutput() {
  if (this->passenger_wait_depart.Number() > 0) {
    Print("%s %lf %lf %lf %lu\n", this->name.c_str(),
          this->passenger_wait_depart.MeanValue(),
          this->passenger_wait_depart.Max(), this->passenger_wait_depart.Min(),
          this->passenger_wait_depart.Number());
  }
}

std::vector<TramStop *> generateBrnoStops() {
  std::vector<TramStop *> stops;
  TramStop *previous_stop = NULL;
  int previous_estimated = 0;
  for (auto stopstat : BRNOSTOPS) {
    // Estimated number of passengers arriving at this stop per day based on
    // probability of REMAINING_PERCANTAGE and previous estimated number
    int current_estimated_arrivals = stopstat.passengers_to_next_flow -
                                     previous_estimated * REMAINING_PERCANTAGE;

    previous_estimated = stopstat.passengers_to_next_flow;

    // This heuristic allowed us to bring the values in the simulation closer to
    // the values from the real world.
    if (current_estimated_arrivals > 6000) {
      current_estimated_arrivals *= 0.9;
    }

    TramStop *current_stop =
        new TramStop(stopstat.name, double(stopstat.distance_until_next),
                     current_estimated_arrivals);
    if (previous_stop)
      previous_stop->SetNext(current_stop);

    // Additional stop from line 6
    if (stopstat.name == "Semilasso") {
      TramStop *kralovo = new TramStop("KralovoPole", double(330), 2071);
      kralovo->SetNext(current_stop);
      stops.push_back(kralovo);
    }
    previous_stop = current_stop;

    stops.push_back(current_stop);
  }
  return stops;
}
