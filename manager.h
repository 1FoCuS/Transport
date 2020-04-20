#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <fstream>
#include "request.h"
#include "parser.h"

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
    Manager();
    void ReadRequestFromStream(std::istream&, Request::Mode);

    std::vector<RequestPtr> queue_requests; //< @todo queue ?
};

#endif // MANAGER_H
