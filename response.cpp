#include "response.h"

void BusInfoResponse::AddToStream(std::ostream& os) const
{
    os << "Bus " << bus_number << ": ";
    if (bus)
    {
        os << bus->GetStopsCount() << " stops on route, "
           << bus->GetStats().unique_stops << " unique stops, "
           << bus->GetStats().route_length << " route length, "
           << bus->GetStats().k << " curvature";
    } else
    {
        os << "not found";
    }
}

void StopInfoResponse::AddToStream(std::ostream& os) const
{
    os << "Stop " << stop_name << ": ";
    if (stop_ptr)
    {
        const auto& buses = stop_ptr->GetBuses();
        if (!buses.empty()) {
            os << "buses ";
            for (const auto& bus : buses)
            {
                os << bus << ' ';
            }
        } else
        {
            os << "no buses";
        }
    } else
    {
        os << "not found";
    }
}

Json::Node RouteStops::ToJson() const
{
    using namespace std;
    using namespace Json;
    map<string, Node> nodes_map;
    nodes_map["request_id"] = Node((int)request_id);
    if (found)
    {
        vector<Node> nodes;
        for (const auto& activity : items)
        {
            nodes.push_back(activity->ToJson());
        }
        nodes_map["items"] = Node(nodes);
        nodes_map["total_time"] = total_time;
    }
    else
    {
        nodes_map["error_message"] = Node(string("not found"));
    }
    return Node(nodes_map);
}

//**************************************************************************************

Json::Node BusInfoResponse::ToJson() const
{
    using namespace Json;
    std::map<std::string, Node> nodes_map;
    nodes_map["request_id"] = Node((int)request_id);
    if (bus)
    {
        const auto& bus_stats = bus->GetStats();
        nodes_map["route_length"] = Node(bus_stats.route_length);
        nodes_map["curvature"] = Node(bus_stats.k);
        nodes_map["stop_count"] = Node((int)bus->GetStopsCount());
        nodes_map["unique_stop_count"] = Node((int)bus_stats.unique_stops);
    }
    else
    {
        nodes_map["error_message"] = Node(std::string("not found"));
    }
    return Node(nodes_map);
}

Json::Node StopInfoResponse::ToJson() const {
    using namespace Json;
    std::map<std::string, Node> nodes_map;
    nodes_map["request_id"] = Node((int)request_id);
    if (stop_ptr)
    {
        const auto& buses = stop_ptr->GetBuses();
        std::vector<Node> nodes;
        for (const auto& bus : buses) {
            nodes.push_back(Node(bus));
        }
        nodes_map["buses"] = Node(nodes);
    } else
    {
        nodes_map["error_message"] = Node(std::string("not found"));
    }
    return Node(nodes_map);
}
