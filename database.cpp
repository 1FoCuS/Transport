#include "database.h"
#include <iostream>

Database::Database()
{

}

void Database::AddStop(const std::string& name_stop, double x, double y)
{
    auto it = data_stops.find(name_stop);

    if (it == data_stops.end())
    {
        data_stops[name_stop] = std::make_shared<Stop>(name_stop, x, y);
    }
    else
    {
        //it->second->SetNewPoint(x,y);
    }
}

void Database::AddorUpdateStop(const std::string& name_stop, double x, double y)
{
    auto it = data_stops.find(name_stop);

    if (it == data_stops.end())
    {
//        std::cerr << "add new stop " << name_stop << std::endl ;
        data_stops[name_stop] = std::make_shared<Stop>(name_stop, x, y);
    }
    else
    {
//        std::cerr << "new pos for stop " << name_stop << std::endl ;
        it->second->SetNewPoint(x,y);
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

        data_buses[name_bus] = std::make_shared<Bus>(name_bus, bus_stops);
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

        data_buses[name_bus] = std::make_shared<Bus>(name_bus, bus_stops);
    }
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
