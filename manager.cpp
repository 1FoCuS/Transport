#include "manager.h"

Manager::Manager()
{

}

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
        queue_requests.push_back(request);
    }
}

void Manager::RunRequests()
{
    for(const auto& request : queue_requests)
    {
        const auto& write_request = static_cast<const WriteRequest&>(*request);
        write_request.Process();
    }

    Database::Instance().UpdateStats();
}

std::vector<BusInfoResponse> Manager::GetRequests()
{
    std::vector<BusInfoResponse> responses;
    for(const auto& request : queue_requests)
    {
        const auto& read_request = static_cast<const GetBusInfo&>(*request);
        responses.push_back(read_request.Process());
    }
    return responses;
}
