#ifndef STOP_H
#define STOP_H

#include <string>
#include <memory>
#include <cmath>
#include <set>

class Stop;
using StopPtr = std::shared_ptr<Stop>;

struct Point
{
    Point(double x, double y) : x(x), y(y) {}
    double x = 0.0;
    double y = 0.0;
};

class Stop
{
public:
    Stop(const std::string& name, double x = 0.0, double y = 0.0) : name(name), point(x,y) {}
    void SetNewPoint(double x, double y) { point.x = x; point.y = y; }

    void AddBus(const std::string& bus_name) { buses.insert(bus_name); }
    const auto& GetBuses() const { return buses; }

    Point GetPointInRadians() const
    {
        return Point(DegreesToRadians(point.x), DegreesToRadians(point.y));
    }

private:
    const std::string name;
    Point point;
    std::set<std::string> buses;

    double DegreesToRadians(double number) const
    {
        return number * M_PI / 180.0;
    }
};



#endif // STOP_H
