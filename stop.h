#ifndef STOP_H
#define STOP_H

#include <string>
#include <memory>
#include <cmath>
#include <set>
#include <unordered_map>

class Stop;
using StopPtr = std::shared_ptr<Stop>;

struct Point
{
    Point() = default;
    Point(double x, double y) : x(x), y(y) {}
    double x = 0.0;
    double y = 0.0;
};

struct Param_Stop
{
    Param_Stop() = default;
    Param_Stop(const std::string& name) : name(name) {}

    std::string name;
    Point point;
    std::unordered_map<std::string, int> stop_dist;
};

class Stop
{
public:
    Stop(const std::string& name, double x = 0.0, double y = 0.0) : name(name), point(x,y) {}
    Stop(Param_Stop param) : name(std::move(param.name)), point(param.point),
                            stops_distance(
                                std::make_move_iterator(param.stop_dist.begin()),
                                std::make_move_iterator(param.stop_dist.end())
                                ) {}

    void SetNewPoint(double x, double y) { point.x = x; point.y = y; }
    void AddDistances(std::unordered_map<std::string, int> dist)
    {
        stops_distance = {
            std::make_move_iterator(dist.begin()),
            std::make_move_iterator(dist.end())
        };
    }

    void AddBus(const std::string& bus_name) { buses.insert(bus_name); }
    const auto& GetBuses() const { return buses; }

    void AddDistStops(std::unordered_map<std::string, int> stop_dist) { stops_distance = std::move(stop_dist); }
    Point GetPointInRadians() const
    {
        return Point(DegreesToRadians(point.x), DegreesToRadians(point.y));
    }

    std::optional<double> GetDistanceTo(StopPtr rhs) const ;
    const std::string& GetName() const { return name; }
private:
    const std::string name;
    Point point;
    std::set<std::string> buses;
    std::unordered_map<std::string, int> stops_distance;

    double DegreesToRadians(double number) const
    {
        return number * M_PI / 180.0;
    }
};



#endif // STOP_H
