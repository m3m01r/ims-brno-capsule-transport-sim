/**
 * IMS project
 *
 * @file main.cpp
 * @brief Starts simulation, collects statistics
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#include "passenger.hpp"
#include "tram_stop.hpp"
#include <ctime>
#include <iostream>
#include <simlib.h>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[]) {
  RandomSeed(std::time(NULL));
  bool save_files = false;

  int opt;
  while ((opt = getopt(argc, argv, "a:c:l:f")) != -1) {
    switch (opt) {
    case 'a':
      acceleration = std::stof(optarg);
      break;
    case 'c':
      capsules_count = std::stoi(optarg);
      break;
    case 'l':
      landing_count = std::stoi(optarg);
      break;
    case 'f':
      save_files = true;
      break;
    default:
      std::cout << "Unknown argument: " << char(opt) << std::endl;
      return -1;
      break;
    }
  }

  // 18 hours
  Init(0, 64800);

  auto stops = generateBrnoStops();
  for (auto stop : stops) {
    stop->InitStat();
  }

  for (auto &stop : stops) {
    (new PassengerGenerator(stop))->Activate();
  }

  Run();

  if (save_files) {
    SetOutput("count.out");
  } else {
    Print("Number of people who traveled between specified stops\n");
    Print("stops simnumber refnumber diff\n");
  }
  TramStop *next = NULL;
  for (auto stop : stops) {
    if ((next = stop->GetNext()) != NULL) {
      int ref = 0;
      if (stop->GetName() == "KralovoPole") {
        ref = 2071;
      } else {
        for (auto stopstat : BRNOSTOPS) {
          if (stopstat.name == stop->GetName()) {
            ref = stopstat.passengers_to_next_flow;
            break;
          }
        }
      }
      Print("%s->%s %i %i %i\n", stop->GetName().c_str(),
            next->GetName().c_str(), stop->counter, ref, stop->counter - ref);
    }
  }

  if (save_files) {
    SetOutput("enter_waiting.out");
  } else {
    Print("\nThe time passengers spent at stops before they could enter the "
          "capsule\n");
    Print("stop mean max min\n");
  }
  for (auto stop : stops) {
    stop->StatEnterOutput();
  }

  if (save_files) {
    SetOutput("depart_waiting.out");
  } else {
    Print("\nThe time passengers spent at stops before they were able to "
          "depart\n");
    Print("stop mean max min passenger_count\n");
  }
  for (auto stop : stops) {
    stop->StatDepartOutput();
  }

  if (save_files) {
    SetOutput("journeys.out");
  } else {
    Print("\nJourney time between two specific stops\n");
    Print("from to mean max min passenger_count\n");
  }
  for (auto stop : stops) {
    stop->StatTravelOutput();
    delete stop;
  }

  return 0;
}
