#include "manager.h"

int main(int , char *[])
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    try
    {
        Manager::Instance().run(input, std::cout);

    } catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }


}





