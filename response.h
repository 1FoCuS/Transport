#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include "bus.h"

class Response
{
public:
    virtual void AddToStream(std::ostream&) const = 0;
};


// ответ на запрос
class BusInfoResponse : public Response
{
public:
    BusInfoResponse(const std::string& bus_number, BusPtr bus): bus_number(bus_number), bus(bus) {}
    void AddToStream(std::ostream& = std::cout) const final;

private:
    std::string bus_number;
    BusPtr bus;
};

void PrintResponses(const std::vector<BusInfoResponse>& responses, std::ostream& stream = std::cout);

#endif // RESPONSE_H
