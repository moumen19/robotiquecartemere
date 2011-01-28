
//#include "CamShiftObjectTracking.hpp"
#include "objectTracking.hpp"
#include <iostream>

int main( int argc, char** argv )
{
    int center=0;

    setupTracking();

    for(;;)
    {
        bool test;
        test = Routine(center);
        std::cout<<center <<"       "<< (bool)test<< std::endl;

    }

    return 0;
}
