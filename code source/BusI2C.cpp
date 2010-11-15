#include "BusI2C.hpp"

BusI2C::BusI2C()
{

}

BusI2C::~BusI2C()
{

}

bool BusI2C::open(std::string port)
{
    return true;
}

void BusI2C::close()
{

}

int BusI2C::getSlave()
{
    return -1;
}

void BusI2C::setSlave(int slave)
{

}

std::string BusI2C::receive()
{
    return "";
}

void BusI2C::send(std::string msg)
{

}
