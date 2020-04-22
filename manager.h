#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include <queue>
#include "request.h"

class Manager
{
public:
    static Manager& Instance()
    {
        static Manager manager;
        return manager;
    }

    void run(std::istream& input = std::cin, std::ostream& output = std::cout);

private:
    Manager() = default;

    void ReadRequestFromStream(std::istream&, Request::Mode);
    void RunRequests();
    std::vector<BusInfoResponse> GetRequests();

    std::queue<RequestPtr> queue_requests;
};

#endif // MANAGER_H
