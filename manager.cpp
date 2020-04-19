#include "manager.h"

Manager::Manager()
{

}

void run(std::istream& input = std::cin, std::ostream& output = std::cout)
{
    int Q;
    input >> Q;

    std::string command;
    for(int i=0; i<Q; ++i)
    {
        std::cerr << "--> start" << std::endl;
        input >> command;
        std::cerr << "read " << command << std::endl;
        if (command == "Bus")
        {
            int number_route;
            input >> number_route;

            Data_route data_route;
            input.ignore(1);


            std::string not_pars_str;
            std::getline(input, not_pars_str);
            std::size_t pos = 0;
            std::string stop;
            while(pos < not_pars_str.size())
            {
                //std::find_first_of(not_pars_str.begin(), not_pars_str.end(), '>');
                data_route.stops.push_back(stop);
            }
            input.ignore(1);

            if (data_route.stops.front() != data_route.stops.back())
            {
                for(int i=data_route.stops.size()-2; i>=0; --i)
                {
                    data_route.stops.push_back(data_route.stops[i]);
                }
            }
            Router::Instance().add_route(number_route, std::move(data_route));
            std::cerr << "Bus: OK" << std::endl;
        }
        if (command == "Get")
        {
            std::cerr << "Get: start" << std::endl;
            int number_route;
            input >> number_route;

            const auto& stop_list = Router::Instance().get_route(number_route).stops;
            for(const auto& stop : stop_list)
            {
                output << stop << " ";
            }
            output << std::endl;
        }
        std::cerr << "--> end" << std::endl;
    }

}
