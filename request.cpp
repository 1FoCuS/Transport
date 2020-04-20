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
