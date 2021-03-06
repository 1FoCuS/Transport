#include "manager.h"

int main(int , char *[])
{
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    std::cout.precision(6);

    try
    {
        Manager::Instance().run(input, output);

    } catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
