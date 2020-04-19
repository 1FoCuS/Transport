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

    void AddStop();
    void AddUpdateStop();
    void AddBusLineRoute();
    void AddBusRingRoute();
    void GetBus();
    void UpdateStats();

private:
    Database();

    std::unordered_map<std::string, StopPtr> data_stops;
    std::unordered_map<std::string, BusPtr> data_buses;
};

#endif // DATABASE_H
