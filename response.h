#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include "bus.h"

struct Response
{
    virtual void AddToStream(std::ostream&) = 0;
};


// ответ на запрос
class BusInfoResponse : public Response
{
public:
    BusInfoResponse(const std::string& bus_number): bus_number(bus_number) {}
    void AddToStream(std::ostream& = std::cout) final;
private:
    std::string bus_number;
};

#endif // RESPONSE_H
