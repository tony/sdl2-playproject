#pragma once

#include <memory>
#include "ship.h"

class StatService {
 public:
  StatService() {}
  void set_ship_stats(const std::shared_ptr<ShipStats>& ship_stats) {
    ship = ship_stats;
  }
  std::shared_ptr<ShipStats> ship;
};
