#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include "json.h"
#include "bus.h"

class Response;
using ResponsePtr = std::shared_ptr<Response>;

class Response
{
public:
    Response(std::size_t rid) : request_id(rid) {}
    virtual void AddToStream(std::ostream&) const = 0;
    virtual Json::Node ToJson() const = 0;

    std::size_t request_id = 0;
};


class BusInfoResponse : public Response
{
public:
    BusInfoResponse(std::size_t rid, const std::string& bus_number, BusPtr bus) : Response(rid), bus_number(bus_number), bus(bus) {}
    void AddToStream(std::ostream& = std::cout) const final;
    virtual Json::Node ToJson() const final;

private:
    std::string bus_number;
    BusPtr bus;
};


// *************** new
class StopInfoResponse : public Response
{
public:
    StopInfoResponse(std::size_t rid, const std::string& stop_name, StopPtr stop_ptr) : Response(rid), stop_name(stop_name), stop_ptr(stop_ptr) {}
    void AddToStream(std::ostream& = std::cout) const final;
    virtual Json::Node ToJson() const final;

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
