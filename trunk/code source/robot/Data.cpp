#include "Data.hpp"

Data::Data()
{
    _DEBUG("Data.cpp", "Initialisation du module de stockage de donnees", INFORMATION);
}

Data::~Data()
{

}

double Data::get(int nb, DataOption::Place place)
{
    if(place == DataOption::FIRST)
        return get(nb, 1);
    else
        return get(nb, a_data[nb].size()-1);
}

double Data::get(int nb, int index)
{
    return a_data[nb][index];
}

void Data::set(int nb, double value)
{
    a_data[nb].push_back(value);
}
