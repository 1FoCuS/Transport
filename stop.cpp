#include "stop.h"

std::optional<double> Stop::GetDistanceTo(StopPtr second_stop) const
{
    const std::string& name = second_stop->GetName();
    if (stops_distance.find(name) != stops_distance.end())
    {
        return stops_distance.at(name);
    }

    return std::nullopt;
}
