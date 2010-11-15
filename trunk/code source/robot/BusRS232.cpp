#include "BusRS232.hpp"

BusRS232::BusRS232()
{
    _DEBUG("BusRS232.cpp", "Initialisation du module de communication par bus RS232", INFORMATION);
}

BusRS232::BusRS232(std::string port)
{

}

BusRS232::~BusRS232()
{

}

bool BusRS232::open(std::string port)
{
    return true;
}

void BusRS232::close()
{

}

std::string BusRS232::receive()
{
    return "";
}

void BusRS232::send(std::string msg)
{

}
