#ifndef DATABASE_H
#define DATABASE_H

#include "stop.h"
#include "bus.h"
#include <unordered_map>

class Database
{
public:

    static Database& Instance()
    {
        static Database db;
        return db;
    }

    void AddStop(const std::string&);
    void AddorUpdateStop(const Param_Stop&);
    void AddBusLineRoute(const std::string&, const std::vector<std::string>&);
    void AddBusRingRoute(const std::string&, const std::vector<std::string>&);

    BusPtr GetBus(const std::string&) const;
    StopPtr GetStop(const std::string&) const;

    void UpdateStats();

private:
    Database() = default;

    std::unordered_map<std::string, StopPtr> data_stops;
    std::unordered_map<std::string, BusPtr> data_buses;
};

#endif // DATABASE_H
