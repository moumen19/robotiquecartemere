#include "DataFusion.hpp"

DataFusion::DataFusion(Data *sensor, Data *environment)
{
    a_sensorsData = sensor;
    a_environmentData = environment;

    _DEBUG("DataFusion.cpp", "Initialisation du module de fusion de donnees", INFORMATION);
}

DataFusion::~DataFusion()
{

}

void DataFusion::run()
{

}
