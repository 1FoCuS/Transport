#ifndef MANAGER_H
#define MANAGER_H

#include "router.h"
#include "transport_types.h"

#include <iostream>
#include <fstream>

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
};

#endif // MANAGER_H
