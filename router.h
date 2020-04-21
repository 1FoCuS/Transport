#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

class Router
{
public:
    static Router& Instance()
    {
        static Router router;
        return router;
    }
private:
    Router();
};



#endif // ROUTER_H
