#include "bus.h"

void Bus::UpdateStats()
{
    stats.unique_stops = GetUniqueStopsCount();
    stats.route_length = GetRouteLength();
}

std::size_t Bus::GetUniqueStopsCount() const
{
    const auto unique_stops = std::unordered_set<StopPtr>(route.begin(), route.end());
    return unique_stops.size();
}

double Bus::GetRouteLength() const
{
    double new_route_length = 0;
    const int route_len = route.size();
    for (int i = 1; i < route_len; ++i)
    {
        new_route_length += GetDistanceBetweenStops(*route[i - 1], *route[i]);
    }
    return new_route_length;
}


double Bus::GetDistanceBetweenStops(const Stop& lhs, const Stop& rhs) const
{
    const double EARTH_RADIUS = 6371000.0;

    const auto lc = lhs.GetPointInRadians();
    const auto rc = rhs.GetPointInRadians();

    return acos(sin(lc.x) * sin(rc.x) + cos(lc.x) * cos(rc.x) * cos(std::abs(lc.y - rc.y))) * EARTH_RADIUS;
}
