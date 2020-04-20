#include "manager.h"

Manager::Manager()
{

}

void Manager::run(std::istream& in_stream, std::ostream& output)
{
    ReadRequestFromStream(in_stream, Request::Mode::WRITE);
}

void Manager::ReadRequestFromStream(std::istream& in_stream, Request::Mode mode)
{
    const std::size_t count_request = Parser::ReadNumber<std::size_t>(in_stream);

    for(std::size_t i = 0; i< count_request; ++i)
    {
        std::string str_request;
        std::getline(in_stream, str_request);
        const auto type_request = Parser::ParseRequest(str_request, mode);
        if (!type_request)
        {
            return;
        }


    }

}
