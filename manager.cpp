#include "manager.h"

void Manager::run(std::istream& in_stream, std::ostream& output)
{
    ReadRequestFromStream(in_stream, Request::Mode::WRITE);

    RunRequests();
    ReadRequestFromStream(in_stream, Request::Mode::READ_ONLY);
    const auto responses = GetRequests();
    PrintResponses(responses, output);
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

std::vector<BusInfoResponse> Manager::GetRequests()
{
    std::vector<BusInfoResponse> responses;
    while (!queue_requests.empty())
    {
        const auto& read_request = static_cast<const GetBusInfo&>(*queue_requests.front());
        responses.push_back(read_request.Process());
        queue_requests.pop();
    }
    return responses;
}

