#ifndef WAYPOINTS
#define WAYPOINTS

#include <map>
#include <string>

namespace butcher {

static const std::map<int, std::string> waypoints = {
  { 1, "Caves" },
  { 5, "Farms" },
  { 10, "Villages" },
  { 15, "Barracks" }
};

}

#endif // WAYPOINTS

