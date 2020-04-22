#ifndef BUS_H
#define BUS_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include "stop.h"

class Bus;
using BusPtr = std::shared_ptr<Bus>;

struct Stats
{
    std::size_t unique_stops = 0;
    double route_length = 0.0;
    double geo_route_length = 0.0;
    double k = 1.0;
};

class Bus
{
public:
    Bus(const std::string& bus_name, const std::vector<StopPtr>& stops) : bus_number(bus_name), route(stops) {};
    void UpdateStats();
    Stats GetStats() const { return stats; }
    std::size_t GetStopsCount() const { return route.size(); }

    double ComputeRouteLength() const;
    double ComputeGeographicalRouteLength() const;
    double ComputeCurvature(double real_length, double geographical_length) const;

private:
    const std::string bus_number;
    std::vector<StopPtr> route;
    Stats stats;

    std::size_t GetUniqueStopsCount() const;
    double GetRouteLength() const;
    double GetGeoDistanceBetweenStops(StopPtr lhs, StopPtr rhs) const;
    double GetRealDistanceBetweenStops(StopPtr lhs, StopPtr rhs) const;
};

#endif // BUS_H
