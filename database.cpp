#include "database.h"
#include <iostream>

void Database::AddStop(const std::string& name)
{
    if (data_stops.find(name) == data_stops.end())
    {
        data_stops[name] = std::make_shared<Stop>(name);
    }
}

void Database::AddorUpdateStop(const Param_Stop& param_stop)
{
    auto it = data_stops.find(param_stop.name);

    if (it == data_stops.end())
    {
        data_stops[param_stop.name] = std::make_shared<Stop>(param_stop);
    }
    else
    {
        it->second->SetNewPoint(param_stop.point.x,param_stop.point.y);
        it->second->AddDistances(param_stop.stop_dist);
    }
}

void Database::AddBusLineRoute(const std::string& name_bus, const std::vector<std::string>& stops)
{
    auto it = data_buses.find(name_bus);
    if (it == data_buses.end())
    {
        std::vector<StopPtr> bus_stops;
        for(const auto& stop: stops)
        {
            AddStop(stop);
            data_stops[stop]->AddBus(name_bus);
            bus_stops.push_back(data_stops[stop]);
        }

        const std::size_t half_size_route = bus_stops.size()-2;
        for(int i = half_size_route; i>=0; --i)
        {
            StopPtr stop = bus_stops[i];
            bus_stops.push_back(stop);
        }

        data_buses[name_bus] = std::make_shared<Bus>(name_bus, std::move(bus_stops));
    }
}
void Database::AddBusRingRoute(const std::string& name_bus, const std::vector<std::string>& stops)
{
    auto it = data_buses.find(name_bus);
    if (it == data_buses.end())
    {
        std::vector<StopPtr> bus_stops;
        for(const auto& stop: stops)
        {
            AddStop(stop);
            data_stops[stop]->AddBus(name_bus);
            bus_stops.push_back(data_stops[stop]);
        }

        data_buses[name_bus] = std::make_shared<Bus>(name_bus, std::move(bus_stops));
    }
}

void Database::AddRouteSettings(RouteParams params)
{
    route_settings = std::move(params);
}

BusPtr Database::GetBus(const std::string& id) const
{
    const auto it = data_buses.find(id);
    return (it!=data_buses.end()) ? it->second : nullptr;
}

StopPtr Database::GetStop(const std::string& stop_name) const
{
    const auto it = data_stops.find(stop_name);
    return (it!=data_stops.end()) ? it->second : nullptr;
}


void Database::UpdateStats()
{
    for (auto route: data_buses)
    {
        route.second->UpdateStats();
    }

}
