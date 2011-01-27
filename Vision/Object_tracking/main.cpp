
//#include "CamShiftObjectTracking.hpp"
#include "objectTracking.hpp"
#include <iostream>

int main( int argc, char** argv )
{
    int center=0;

    setupTracking();

    for(;;)
    {
        if(!Routine(center))
            break;
        std::cout<<center <<std::endl;
    }

    return 0;
}
