#include "Data.hpp"

Data::Data()
{
    a_data.resize(4);
    _DEBUG("Initialisation du module de stockage de donnees", INFORMATION);
}

Data::~Data()
{

}

void* Data::get(int nb, DataOption::Place place)
{
    if(place == DataOption::FIRST)
        return get(nb, 0);
    else
        return get(nb, a_data[nb].size()-1);
}

void* Data::get(int nb, int index)
{
    return a_data[nb][index];
}

void Data::set(int nb, void* value)
{
    a_data[nb].push_back(value);
    _DEBUG("Ajout d'une donnée au module de stockage de donnees", INFORMATION);
}
