#include "bus.h"

void Bus::UpdateStats()
{
    stats.unique_stops = GetUniqueStopsCount();
    stats.route_length = GetRouteLength();

    stats.geo_route_length = ComputeGeographicalRouteLength();
    stats.k = ComputeCurvature(stats.route_length, stats.geo_route_length);
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
        new_route_length += GetGeoDistanceBetweenStops(route[i - 1], route[i]);
    }
    return new_route_length;
}


double Bus::GetGeoDistanceBetweenStops(StopPtr lhs, StopPtr rhs) const
{
    const double EARTH_RADIUS = 6371000.0;

    const auto lc = lhs->GetPointInRadians();
    const auto rc = rhs->GetPointInRadians();

    return acos(sin(lc.x) * sin(rc.x) + cos(lc.x) * cos(rc.x) * cos(std::abs(lc.y - rc.y))) * EARTH_RADIUS;
}

double Bus::GetRealDistanceBetweenStops(StopPtr lhs, StopPtr rhs) const
{
    if (auto distance_from_lhs_to_rhs = lhs->GetDistanceTo(rhs))
    {
        return *distance_from_lhs_to_rhs;
    } else
        if (auto distance_from_rhs_to_lhs = rhs->GetDistanceTo(lhs))
    {
        return *distance_from_rhs_to_lhs;
    } else
    {
        throw std::runtime_error("no distance between " + lhs->GetName() + " and " + rhs->GetName());
    }
}

double Bus::ComputeRouteLength() const
{
    double new_route_length = 0;
    const int n = route.size();
    for (int i = 1; i < n; ++i)
    {
        new_route_length += GetRealDistanceBetweenStops(route[i - 1], route[i]);
    }
    return new_route_length;
}

double Bus::ComputeGeographicalRouteLength() const
{
    double new_route_length = 0;
    const int n = route.size();
    for (int i = 1; i < n; ++i)
    {
        new_route_length += GetGeoDistanceBetweenStops(route[i - 1], route[i]);
    }
    return new_route_length;
}

double Bus::ComputeCurvature(double real_length, double geographical_length) const
{
    return real_length / geographical_length;
}
