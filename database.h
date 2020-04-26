#ifndef DATABASE_H
#define DATABASE_H

#include "stop.h"
#include "bus.h"
#include "json.h"
#include <unordered_map>
#include "routeractivity.h"

using TransportGraph = Graph::DirectedWeightedGraph<double>;
using TransportGraphPtr = std::shared_ptr<Graph::DirectedWeightedGraph<double>>;
using TransportRouter = Graph::Router<double>;
using TransportRouterPtr = std::shared_ptr<Graph::Router<double>>;

//*********************************************************************************


//*********************************************************************************

class Database
{
public:
    static Database& Instance()
    {
        static Database db;
        return db;
    }

    // new type ****************
    struct RouteParams
    {
        int bus_wait_time;
        double bus_velocity;
    };
    //***************************

    void AddStop(const std::string&);
    void AddorUpdateStop(const Param_Stop&);
    void AddBusLineRoute(const std::string&, const std::vector<std::string>&);
    void AddBusRingRoute(const std::string&, const std::vector<std::string>&);
    void AddRouteSettings(RouteParams);

    TransportGraphPtr GetGraph() const { return graph; }
    TransportRouterPtr GetRouter() const { return router; }
    RouterActivityPtr GetWaitActivityByEdge(size_t edge_id) const { return wait_activities_by_edge.at(edge_id); }
    RouterActivityPtr GetBusActivityByEdge(size_t edge_id) const { return bus_activities_by_edge.at(edge_id); }

    BusPtr GetBus(const std::string&) const;
    StopPtr GetStop(const std::string&) const;

    void UpdateStats();
    void UpdateGraphAndRouter();
private:
    Database() = default;

    std::unordered_map<std::string, StopPtr> data_stops;
    std::unordered_map<std::string, BusPtr> data_buses;
    RouteParams route_settings;

    TransportGraphPtr graph;
    TransportRouterPtr router;

    std::size_t stop_last_index = 0;

    std::vector<std::string> stops_by_index;
    std::unordered_map<size_t, RouterActivityPtr> wait_activities_by_edge;
    std::unordered_map<size_t, RouterActivityPtr> bus_activities_by_edge;
};

#endif // DATABASE_H
