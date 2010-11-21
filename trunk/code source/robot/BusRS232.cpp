#include "BusRS232.hpp"

BusRS232::BusRS232()
{
    this->a_isOpen = false;
    this->a_port = "";

    _DEBUG("Initialisation du module de communication par bus RS232", INFORMATION);
}



BusRS232::~BusRS232()
{
    this->close();
}

bool BusRS232::open(std::string port)
{
    this->a_port = port;
    this->close();

    this->a_file.open(this->a_port.c_str());
    if(a_file.is_open())
    {
        this->a_isOpen = true;

        _DEBUG("Ouverture du port RS232", INFORMATION);

        return true;
    }

    _DEBUG("Echec de l'ouverture du port RS232", WARNING);

    return false;
}

void BusRS232::close()
{
    if(a_file.is_open())
        a_file.close();
    this->a_isOpen = false;

    _DEBUG("Fermerure du port RS232", INFORMATION);
}

std::string BusRS232::receive()
{
    return "";
}

void BusRS232::send(std::string msg)
{
    this->a_file << msg.c_str();
}
