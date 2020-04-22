#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include "bus.h"

class Response;
using ResponsePtr = std::shared_ptr<Response>;

class Response
{
public:
    virtual void AddToStream(std::ostream&) const = 0;
};


class BusInfoResponse : public Response
{
public:
    BusInfoResponse(const std::string& bus_number, BusPtr bus) : bus_number(bus_number), bus(bus) {}
    void AddToStream(std::ostream& = std::cout) const final;

private:
    std::string bus_number;
    BusPtr bus;
};


// *************** new
class StopInfoResponse : public Response
{
public:
    StopInfoResponse(const std::string& stop_name, StopPtr stop_ptr) : stop_name(stop_name), stop_ptr(stop_ptr) {}
    void AddToStream(std::ostream& = std::cout) const final;

private:
    std::string stop_name;
    StopPtr stop_ptr;
};

inline std::ostream& operator<<(std::ostream& os, const ResponsePtr& response)
{
    response->AddToStream(os);
    return os;
}

#endif // RESPONSE_H
