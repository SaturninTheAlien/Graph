#include <iostream>
#include <optional>
#include <string>
#include "Graph.hpp"
#include "GraphTest.hpp"

using namespace std;


int main()
{

    std::cout<<"Hello world"<<std::endl;
    try
    {
        graphTest();
    }
    catch (const std::exception& e)
    {
        cout<<e.what()<<std::endl;
    }
    catch (const char*e)
    {
        cout<<e<<std::endl;
    }
    catch(...)
    {
        cout<<"Unknown expeption!"<<std::endl;
    }
    std::cout<<"\nThe end"<<std::endl;

    return 0;
}
