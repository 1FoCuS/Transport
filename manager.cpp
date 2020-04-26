#include "manager.h"

void Manager::run(std::istream& in_stream, std::ostream& output)
{
    Json::Document document = Json::Load(in_stream);

    ReadRequestFromJson(document, "base_requests");
    RunRequests();
    ReadRequestFromJson(document, "routing_settings");
    /*ReadRequestFromJson(document, "stat_requests");
    RunResponse();
*/
    PrintClearResponsesJson(output);
}

void Manager::PrintClearResponses(std::ostream& stream)
{
    while(!queue_responses.empty())
    {
        stream << queue_responses.front() << '\n';
        queue_responses.pop();
    }
}

void Manager::PrintClearResponsesJson(std::ostream& stream)
{
    std::vector<Json::Node> json_response;
    while(!queue_responses.empty())
    {
        json_response.push_back(queue_responses.front()->ToJson());
        queue_responses.pop();
    }
    stream << Json::Node(json_response);
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

void Manager::ReadRequestFromJson(const Json::Document& doc, const std::string& section)
{
    const Json::Node& root = doc.GetRoot();
    if (mode_settings.find(section) == mode_settings.end())
    {
        std::cerr << "unknown section" <<std::endl;
        return;
    }
    const auto request_mode = mode_settings.at(section);
    if (section == "routing_settings")
    {
        auto request_ptr = Request::Create(Request::TypeRequest::ADD_ROUTER_SETTINGS);
        const auto& it = root.AsMap();
        if (request_ptr && it.find(section)!=it.end())
        {
            request_ptr->Parse(it.at(section));
            //queue_requests.push(std::move(request_ptr));
            const auto& request = static_cast<const ReadRequest&>(*request_ptr);
            request.Process();
        }

        Database::Instance().UpdateGraphAndRouter();
    }
    else
    {
        const auto& json_requests = root.AsMap().at(section).AsArray();
        for(const auto& json_request : json_requests)
        {
            if (auto request = ParseRequestJson(json_request, request_mode) )
            {
                queue_requests.push(std::move(request));
            }
        }
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
