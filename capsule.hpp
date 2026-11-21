/**
 * IMS project
 *
 * @file capsule.hpp
 * @brief Class declaration for capsule return process
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#ifndef CAPSULE_HPP
#define CAPSULE_HPP

#include <simlib.h>

/// Process of capsule return
/// Activates when the passenger leaves it at the destination stop
class CapsuleReturn : public Process {
public:
  CapsuleReturn(Store *initial_place, Store *landing_place, double return_time);
  void Behavior();

private:
  // Initial stop
  Store *initial_place;
  // Destination stop
  Store *landing_place;
  double return_time;
};

#endif
