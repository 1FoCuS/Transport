#ifndef BUS_H
#define BUS_H

#include <string>
#include <memory>
#include <vector>
#include "stop.h"


struct Stats
{
    std::size_t unique_stops = 0;
    double route_length = 0.0;
};

class Bus
{
public:
    Bus(const std::string&);

private:
    const std::string bus_number;
    std::vector<StopPtr> route;
    Stats stats;
};

using BusPtr = std::shared_ptr<Bus>;

#endif // BUS_H
