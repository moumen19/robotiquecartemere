#include "DataFusion.hpp"

DataFusion::DataFusion(Data *sensor, Data *environment)
{
    this->a_sensorsData = sensor;
    this->a_environmentData = environment;

    _DEBUG("Initialisation du module de fusion de donnees", INFORMATION);
}

DataFusion::~DataFusion()
{

}

void DataFusion::run()
{

}
