#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <queue>
#include "request.h"
#include "json.h"

class Manager
{
public:
    static Manager& Instance()
    {
        static Manager manager;
        return manager;
    }

    void run(std::istream& input = std::cin, std::ostream& output = std::cout);

    void PrintClearResponses(std::ostream& stream = std::cout);
    void PrintClearResponsesJson(std::ostream& stream = std::cout);
private:
    Manager() = default;

    [[deprecated("use read from json")]]
    void ReadRequestFromStream(std::istream&, Request::Mode);
    void ReadRequestFromJson(const Json::Document&, const std::string&);

    void RunRequests();
    void RunResponse();

    std::queue<RequestPtr> queue_requests;
    std::queue<ResponsePtr> queue_responses;
    const std::unordered_map<std::string, Request::Mode> mode_settings = {
        std::make_pair("base_requests", Request::Mode::WRITE),
        std::make_pair("routing_settings", Request::Mode::WRITE),
        std::make_pair("stat_requests", Request::Mode::READ_ONLY),
    };
};

#endif // MANAGER_H
