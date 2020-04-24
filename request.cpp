#include "request.h"

std::optional<Request::TypeRequest> CheckTypeRequest(std::string_view str, Request::Mode mode)
{

    if (mode==Request::Mode::WRITE)
    {
        if (str.find("Bus")==0)
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
            else
            {
                if (str.find("Stop")==0)
                {
                    return  Request::TypeRequest::GET_INFO_STOP;
                }
            }

        }
    }

    return std::nullopt;
}

std::optional<Request::TypeRequest> CheckTypeRequest(const Json::Node& node, Request::Mode mode)
{

    if (mode==Request::Mode::WRITE)
    {
        if (node.AsMap().at("type").AsString() == "Bus")
        {
            if (node.AsMap().at("is_roundtrip").AsBool())
            {
                return Request::TypeRequest::ADD_BUS_RING;
            }
            else return Request::TypeRequest::ADD_BUS_LINE;
        }
        else if (node.AsMap().at("type").AsString() == "Stop")
        {
            return Request::TypeRequest::ADD_STOP;
        }
    }
    else
    {
        if (mode == Request::Mode::READ_ONLY)
        {
            if (node.AsMap().at("type").AsString() == "Bus")
            {
                return Request::TypeRequest::GET_INFO_BUS;
            }
            else return  Request::TypeRequest::GET_INFO_STOP;
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
            return std::make_unique<GetBusInfo>();
        case RT::GET_INFO_STOP:
            return std::make_unique<GetStopInfo>();
        default:
            return nullptr;
    }
}

//************************************* Parse-methods ******************************************************

void AddStopRequest::Parse(std::string_view input)
{
    param_stop.name = Parser::ReadToken(input, ": ");
    param_stop.point.x = Parser::ConvertToDouble(Parser::ReadToken(input, ", "));
    param_stop.point.y = Parser::ConvertToDouble(Parser::ReadToken(input, ", "));

    while(!input.empty())
    {
        int dist = Parser::ConvertToInt(Parser::ReadToken(input, "m to "));
        std::string new_stop = std::string(Parser::ReadToken(input, ", "));
        param_stop.stop_dist[new_stop] = dist;
    }
}

void AddStopRequest::Parse(const Json::Node& node)
{
    const auto& settings = node.AsMap();
    param_stop.name = settings.at("name").AsString();
    param_stop.point.x = settings.at("latitude").AsDouble();
    param_stop.point.y = settings.at("longitude").AsDouble();

    const auto& distances = settings.at("road_distances").AsMap();
    for (const auto& [other_stop, distanceNode] : distances)
    {
        int distance = distanceNode.AsInt();
        param_stop.stop_dist[other_stop] = distance;
    }
}


void AddStopRequest::Process() const
{
    Database::Instance().AddorUpdateStop(param_stop);
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

void AddBusLineRoute::Parse(const Json::Node& node)
{
    const auto& settings = node.AsMap();
    id = settings.at("name").AsString();
    const auto& stops_nodes = settings.at("stops").AsArray();
    for (const auto& stop_node : stops_nodes)
    {
        stops.push_back(stop_node.AsString());
    }
}

void AddBusLineRoute::Process() const
{
    Database::Instance().AddBusLineRoute(id, stops);
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

void AddBusRingRoute::Parse(const Json::Node& node)
{
    const auto& settings = node.AsMap();
    id = settings.at("name").AsString();
    const auto& stops_nodes = settings.at("stops").AsArray();
    for (const auto& stop_node : stops_nodes)
    {
        stops.push_back(stop_node.AsString());
    }
}

void AddBusRingRoute::Process() const
{
    Database::Instance().AddBusRingRoute(id, stops);
}

void GetBusInfo::Parse(std::string_view input)
{
    bus_id = input;
}

void GetBusInfo::Parse(const Json::Node& node)
{
    const auto& settings = node.AsMap();
    bus_id = settings.at("name").AsString();
    request_id = settings.at("id").AsInt();
}


 ResponsePtr GetBusInfo::Process() const
{
    return std::make_shared<BusInfoResponse>(request_id, bus_id, Database::Instance().GetBus(bus_id));
}

void GetStopInfo::Parse(std::string_view name)
{
    stop_name = name;
}

void GetStopInfo::Parse(const Json::Node& node)
{
    const auto& settings = node.AsMap();
    stop_name = settings.at("name").AsString();
    request_id = settings.at("id").AsInt();
}

 ResponsePtr GetStopInfo::Process() const
{
    return std::make_shared<StopInfoResponse>(request_id, stop_name, Database::Instance().GetStop(stop_name));
}

//******************************* function for work with request*********************************************



