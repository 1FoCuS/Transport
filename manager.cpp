#include "manager.h"

void Manager::run(std::istream& in_stream, std::ostream& output)
{
    Json::Document document = Json::Load(in_stream);



    ReadRequestFromStream(in_stream, Request::Mode::WRITE);
    RunRequests();

    ReadRequestFromStream(in_stream, Request::Mode::READ_ONLY);
    RunResponse();

    PrintClearResponses(output);
}

void Manager::PrintClearResponses(std::ostream& stream)
{
    while(!queue_responses.empty())
    {
        stream << queue_responses.front() << '\n';
        queue_responses.pop();
    }
}

void Manager::ReadRequestFromStream(std::istream& in_stream, Request::Mode mode)
{
    const std::size_t count_request = ReadNumber<std::size_t>(in_stream);
    for(std::size_t i = 0; i< count_request; ++i)
    {
        std::string str_request;
        std::getline(in_stream, str_request);
        auto request = ParseRequest(str_request, mode);
        if (!request)
        {
            return;
        }
        queue_requests.push(std::move(request));
    }
}

void ReadRequestFromJson(const Json::Document& doc, Request::Mode mode)
{
    const Json::Node& root = doc.GetRoot();
    const std::string request_type = (mode == Request::Mode::WRITE) ? "base_requests" : "stat_requests";
    const auto& json_requests = root.AsMap().at(request_type).AsArray();
    std::size_t count_request = json_requests.size();

    for(const auto& request : json_requests)
    {
        // Parser?
    }
}

void Manager::RunRequests()
{
    while (!queue_requests.empty())
    {
        const auto& write_request = static_cast<const WriteRequest&>(*queue_requests.front());
        write_request.Process();
        queue_requests.pop();
    }
    Database::Instance().UpdateStats();
}

void Manager::RunResponse()
{
    while (!queue_requests.empty())
    {
        const auto& read_request = static_cast<const ReadRequest&>(*queue_requests.front());
        queue_responses.push(read_request.Process());
        queue_requests.pop();
    }
}
