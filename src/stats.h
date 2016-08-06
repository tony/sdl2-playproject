#pragma once

#include <memory>
#include "ship.h"

class StatService {
 public:
  StatService(const std::shared_ptr<ShipStats>& ship_stats)
      : ship(ship_stats) {}
  const std::shared_ptr<ShipStats>& ship;
};
