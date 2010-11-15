#include "BusI2C.hpp"

BusI2C::BusI2C()
{
    if(_DEBUG_MODE)
        _DEBUG::addMessage("BusI2c.cpp", "Initialisation du module de communication par bus I2C", INFORMATION);
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
