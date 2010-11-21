#include "Data.hpp"

Data::Data()
{
    this->a_data.resize(4);
    _DEBUG("Initialisation du module de stockage de donnees", INFORMATION);
}

Data::~Data()
{

}

boost::any Data::get(int nb, DataOption::Place place)
{
    if(place == DataOption::FIRST)
        return this->get(nb, 0);
    else
        return this->get(nb, this->a_data[nb].size()-1);
}

boost::any Data::get(int nb, int index)
{
    return this->a_data[nb][index];
}

void Data::set(int nb, boost::any value)
{
    this->a_data[nb].push_back(value);
    _DEBUG("Ajout d'une donnee au module de stockage de donnees", INFORMATION);
}
