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

//**************************************************************************************


