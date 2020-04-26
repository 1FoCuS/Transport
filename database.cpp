#include "database.h"
#include <iostream>



void Database::AddStop(const std::string& name)
{
    if (data_stops.find(name) == data_stops.end())
    {
        data_stops[name] = std::make_shared<Stop>(name);
        data_stops[name]->SetIndex(stop_last_index++);
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
    data_stops[param_stop.name]->SetIndex(stop_last_index);
    ++stop_last_index;
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

void Database::UpdateGraphAndRouter()
{
    using namespace std;
    const size_t vertex_count = data_stops.size();
    graph = make_shared<TransportGraph>(vertex_count);
    const double wait_time = route_settings.bus_wait_time;
    for (auto& [bus_id, bus] : data_buses)
    {
        const auto& stops = bus->GetStops();
        const int n = stops.size();
        for (int i = 0; i < n - 1; ++i)
        {
            double distance = 0;
            for (int j = i + 1; j < n; ++j)
            {
                distance += Bus::GetDistStops(stops[j - 1], stops[j]);
                const double bus_time = distance / route_settings.bus_velocity;
                const size_t span_count = j - i;

                const auto edge_id = graph->AddEdge({ stops[i]->GetIndex(), stops[j]->GetIndex(), wait_time + bus_time });
                wait_activities_by_edge[edge_id] = make_shared<WaitActivity>(stops[i]->GetName(), wait_time);
                bus_activities_by_edge[edge_id] = make_shared<BusActivity>(bus_id, bus_time, span_count);
            }
        }
    }
    router = make_shared<TransportRouter>(*graph);
}
