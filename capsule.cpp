/**
 * IMS project
 *
 * @file capsule.cpp
 * @brief Implementation of capsule return process
 *
 * @author Albert Tikaiev (xtikaia00)
 * @author Danil Tasmassys (xtasmad00)
 * @date 2025-12-07
 */

#include "capsule.hpp"

CapsuleReturn::CapsuleReturn(Store *initial_place, Store *landing_place,
                             double return_time) {
  this->initial_place = initial_place;
  this->landing_place = landing_place;
  this->return_time = return_time;
}

void CapsuleReturn::Behavior() {
  // Make landing place available when leaving
  this->landing_place->Leave(1);

  // Return to the initial stop
  Wait(this->return_time);

  // Make the capsule available at the initial stop
  this->initial_place->Leave(1);
}
