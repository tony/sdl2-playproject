#ifndef HAVE_STATS_H_INCLUDED
#define HAVE_STATS_H_INCLUDED

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
#endif  // HAVE_STATS_H_INCLUDED
