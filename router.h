#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "transport_types.h"


class Router
{
public:
    static Router& Instance()
    {
        static Router router;
        return router;
    }

    void add_route(int number_route, Data_route data_route);

    const Data_route& get_route(int number_route) const;
private:
    Router();
};



#endif // ROUTER_H
