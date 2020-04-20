#include "request.h"


std::optional<Request::TypeRequest> CheckTypeRequest(std::string_view str, Request::Mode mode)
{

    if (mode==Request::Mode::WRITE)
    {
        if (str.find("BUS")==0)
        {
            return (str.find('-') == str.npos) ? Request::TypeRequest::ADD_BUS_RING : Request::TypeRequest::ADD_BUS_LINE;
        } else
        {
            if (str.find("Stop") == 0)
            {
                return Request::TypeRequest::ADD_STOP;
            }
        }
    }
    else
    {
        if (mode == Request::Mode::READ_ONLY)
        {
            if (str.find("Bus") == 0)
            {
                return Request::TypeRequest::GET_INFO_BUS;
            }
        }
    }

    return std::nullopt;
}

RequestPtr Request::Create(Request::TypeRequest type)
{
    using RT = Request::TypeRequest;

    switch (type)
    {
        case RT::ADD_STOP:
            return std::make_unique<AddStopRequest>();
        case RT::ADD_BUS_LINE:
            return std::make_unique<AddBusLineRoute>();
        case RT::ADD_BUS_RING:
            return std::make_unique<AddBusRingRoute>();
        case RT::GET_INFO_BUS:
            return std::make_unique<GetBusInfo<BusInfoResponse>>();
        default:
            return nullptr;
    }
}
//************************************* Parse-methods ******************************************************
void AddStopRequest::Parse(std::string_view input)
{
    name = Parser::ReadToken(input, ": ");
    x = Parser::ConvertToDouble(Parser::ReadToken(input, ", "));
    y = Parser::ConvertToDouble(input);
}
void AddStopRequest::Process() const
{
    // add to database stop -> name,x,y
}


void AddBusLineRoute::Parse(std::string_view str_names)
{
    id = Parser::ReadToken(str_names, ": ");
    stops.push_back(std::string(Parser::ReadToken(str_names, " - ")));
    while (!str_names.empty())
    {
        stops.push_back(std::string(Parser::ReadToken(str_names, " - ")));
    }
}

void AddBusLineRoute::Process() const
{
    // add to database route and bus
}

void AddBusRingRoute::Parse(std::string_view str_names)
{
    id = Parser::ReadToken(str_names, ": ");
    stops.push_back(std::string(Parser::ReadToken(str_names, " > ")));
    while (!str_names.empty())
    {
        stops.push_back(std::string(Parser::ReadToken(str_names, " > ")));
    }
}
void AddBusRingRoute::Process() const
{
    // add to database route and bus
}

void GetBusInfo::Parse(std::string_view input)
{
    bus_id = input;
}

BusInfoResponse GetBusInfo::Process() const
{

}

//******************************* function for work with request*********************************************
// @todo optimisation read number
template <typename Number>
Number ReadNumber(std::istream& in_stream)
{
    assert(std::is_arithmetic<Number>::value);

    Number number;
    in_stream >> number;
    std::string value;
    std::getline(in_stream, value);

    return number;
}

RequestPtr ParseRequest(std::string_view str_request, Request::Mode mode)
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