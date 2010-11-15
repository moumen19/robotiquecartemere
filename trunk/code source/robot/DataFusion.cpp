#include "DataFusion.hpp"

DataFusion::DataFusion(Data *sensor, Data *environment)
{
    a_sensorsData = sensor;
    a_environmentData = environment;

    if(_DEBUG_MODE)
        _DEBUG::addMessage("DataFusion.cpp", "Initialisation du module de fusion de donnees", INFORMATION);
}

DataFusion::~DataFusion()
{

}

void DataFusion::run()
{

}
