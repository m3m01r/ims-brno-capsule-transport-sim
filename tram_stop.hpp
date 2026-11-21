/**
 * IMS project
 *
 * @file tram_stop.hpp
 * @brief Class declaration for tram stop and its generation data/function
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#ifndef TRAM_STOP_HPP
#define TRAM_STOP_HPP

#include <simlib.h>
#include <vector>

// The probability that a passenger will travel one stop further
#define REMAINING_PERCANTAGE 0.75

extern int capsules_count;
extern int landing_count;

/// Represents tram and is used as linked list to maintain sequence of the stops
class TramStop {
private:
  // Lift, capsules and landing places
  Queue enter_queue;
  Facility enter_lift;
  Store capsules;
  Store landing_places;
  // NULL if is final stop
  TramStop *next;
  // Stop name
  std::string name;
  // Distance to the next stop
  double distance;
  // Estimated daily arrivals
  int estimated_arrivals;
  // Collected statistics for this stop
  std::vector<Stat *> travel_stats;
  Stat passenger_wait_enter;
  Stat passenger_wait_depart;

public:
  int counter;
  TramStop(std::string StopName, double distance, int estimated_arrivals);
  void SetNext(TramStop *NextStop);
  TramStop *GetNext();
  double GetDistanceToTheNextStop();
  std::string GetName();
  int GetEstimatedArrivals();
  Facility *TryEnter(Process *proc);
  void FinishEnter();
  Store *GetCapsules();
  Store *GetLandingPlaces();
  void InitStat();
  void StatTravel(std::string name, double value);
  void StatEnter(double value);
  void StatDepart(double value);
  void StatTravelOutput();
  void StatEnterOutput();
  void StatDepartOutput();
};

typedef struct _stopStats {
  std::string name;
  int distance_until_next;
  int passengers_to_next_flow;
} StopStats;

// Distance source:
// https://idos.cz/vlakyautobusymhdvse/spojeni/
// Passenger flow to the next stop source:
// https://data.brno.cz/datasets/mestobrno::intenzity-mhd-tramvaj-public-transport-intensity-tram/about
const StopStats BRNOSTOPS[] = {
    StopStats{"Reckovice", 439, 3342},   StopStats{"Filkukova", 398, 5374},
    StopStats{"Koriskova", 380, 6114},   StopStats{"Hudcova", 460, 5643},
    StopStats{"Tylova", 465, 5653},      StopStats{"Semilasso", 578, 11095},
    StopStats{"Husitska", 280, 12926},   StopStats{"Jungmannova", 530, 17364},
    StopStats{"Kartouzska", 314, 16817}, StopStats{"Sumavaska", 406, 16706},
    StopStats{"Hrncirska", 324, 17227},  StopStats{"Pionyrska", 560, 16386},
    StopStats{"Antoninska", 460, 17141}, StopStats{"Moravske", 282, 13315},
};

/// Initialize real world stops from Brno as `TramStop` objects
std::vector<TramStop *> generateBrnoStops();
#endif
