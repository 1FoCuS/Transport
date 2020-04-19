#ifndef STOP_H
#define STOP_H

#include <string>
#include <memory>

struct Point
{
    Point(double x, double y) : x(x), y(y) {}
    double x = 0.0;
    double y = 0.0;
};

class Stop
{
public:
    Stop(const std::string&, double x = 0.0, double y = 0.0);

private:
    const std::string name;
    const Point point;
};

using StopPtr = std::shared_ptr<Stop>;

#endif // STOP_H
