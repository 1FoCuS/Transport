#include "response.h"

void BusInfoResponse::AddToStream(std::ostream& os) const
{
    os << "Bus " << bus_number << ": ";
    if (bus)
    {
        os << bus->GetStopsCount() << " stops on route, "
           << bus->GetStats().unique_stops << " unique stops, "
           << bus->GetStats().route_length << " route length";
    } else
    {
        os << "not found";
    }
}

std::ostream& operator<<(std::ostream& os, const Response& response)
{
    response.AddToStream(os);
    return os;
}

void PrintResponses(const std::vector<BusInfoResponse>& responses, std::ostream& stream)
{
    for (auto response : responses)
    {
        stream << response << '\n';
    }
}
