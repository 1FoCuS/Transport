#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include "response.h"

struct Request;
using RequestPtr = std::unique_ptr<Request>;


//***************************** 1-level class **********************************
struct Request
{
    enum class TypeRequest
    {
        ADD_STOP,
        ADD_BUS_LINE,
        ADD_BUS_RING,
        GET_INFO_BUS
    };

    enum class Mode
    {
        READ_ONLY,
        WRITE
    };

    Request(TypeRequest type) : type(type) {}
    static RequestPtr Create(TypeRequest);
    virtual void Parse(std::string_view) = 0;
    virtual ~Request() = default;

    const TypeRequest type;
};

//***************************** 2-level class **********************************

struct ModifyRequest : Request
{
    using Request::Request;
    virtual void Process() const = 0;
};


template <typename ResultType>
struct ReadRequest: Request
{
    using Request::Request;
    virtual ResultType Process() = 0;
};

//***************************** 3-level class **********************************


// ************************* 3-level write-request ******************************
struct AddStopRequest : ModifyRequest
{
    AddStopRequest() : ModifyRequest(TypeRequest::ADD_STOP) {}
    void Parse(std::string_view input) override final;
    void Process() const override final;

private:
    std::string name;
    double x;
    double y;
};

struct AddBusLineRoute : ModifyRequest
{
    AddBusLineRoute() : ModifyRequest(TypeRequest::ADD_BUS_LINE) {}
    void Parse(std::string_view input) override final;
    void Process() const override final;
};

struct AddBusRingRoute : ModifyRequest
{
    AddBusRingRoute() : ModifyRequest(TypeRequest::ADD_BUS_RING) {}
    void Parse(std::string_view input) override final;
    void Process() const override final;
};


// ************************* 3-level read-request ****************************

template <typename ResultType>
struct GetBusInfo : ReadRequest<BusInfoResponse>
{
    GetBusInfo() : ReadRequest(TypeRequest::GET_INFO_BUS) {}
    virtual void Parse(std::string_view) override final;
    virtual BusInfoResponse Process() override final;
};

// ************************* function for work with request ******************

std::optional<Request::TypeRequest> CheckTypeRequest(std::string_view, Request::Mode);


#endif // REQUEST_H
