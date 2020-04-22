#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include <cassert>

#include "response.h"
#include "parser.h"
#include "database.h"

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
        GET_INFO_BUS,
        GET_INFO_STOP
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

struct WriteRequest : public Request
{
    WriteRequest(TypeRequest type) : Request(type) {}
    virtual void Process() const = 0;
};


struct ReadRequest: Request
{
    ReadRequest(TypeRequest type) : Request(type) {}
    virtual ResponsePtr Process() const = 0;
};

//***************************** 3-level class **********************************


// ************************* 3-level write-request ******************************
struct AddStopRequest : WriteRequest
{
    AddStopRequest() : WriteRequest(TypeRequest::ADD_STOP) {}

    void Parse(std::string_view input) override final;
    void Process() const override final;

private:
    std::string name;
    double x;
    double y;
};

struct AddBusLineRoute : WriteRequest
{
    AddBusLineRoute() : WriteRequest(TypeRequest::ADD_BUS_LINE) {}
    void Parse(std::string_view input) override final;
    void Process() const override final;

private:
    std::string id;
    std::vector<std::string> stops;
};

struct AddBusRingRoute : WriteRequest
{
    AddBusRingRoute() : WriteRequest(TypeRequest::ADD_BUS_RING) {}

    void Parse(std::string_view input) override final;
    void Process() const override final;

private:
    std::string id;
    std::vector<std::string> stops;
};


// ************************* 3-level read-request ****************************

struct GetBusInfo : ReadRequest
{
    GetBusInfo() : ReadRequest(TypeRequest::GET_INFO_BUS) {}

    virtual void Parse(std::string_view) override final;
    virtual ResponsePtr Process() const override final;

private:
    std::string bus_id;
};

struct GetStopInfo : ReadRequest
{
    GetStopInfo() : ReadRequest(TypeRequest::GET_INFO_STOP) {}

    virtual void Parse(std::string_view) override final;
    virtual ResponsePtr Process() const override final;

private:
    std::string stop_name;
};

// ************************* function for work with request ******************

std::optional<Request::TypeRequest> CheckTypeRequest(std::string_view, Request::Mode);

// @todo optimisation read number
template <typename Number>
inline  Number ReadNumber(std::istream& in_stream)
{
    assert(std::is_arithmetic<Number>::value);

    Number number;
    in_stream >> number;
    std::string value;
    std::getline(in_stream, value);

    return number;
}

inline RequestPtr ParseRequest(std::string_view str_request, Request::Mode mode)
{
    const auto type_request = CheckTypeRequest(str_request, mode);
    if (!type_request)
    {
        return nullptr;
    }

    auto request_ptr = Request::Create(type_request.value());
    if (request_ptr)
    {
        Parser::ReadToken(str_request);
        request_ptr->Parse(str_request);
    }

    return request_ptr;
}


#endif // REQUEST_H
