#include "DataFusion.hpp"

DataFusion::DataFusion(Data *sensor, Data *environment)
{
    a_sensorsData = sensor;
    a_environmentData = environment;
}

DataFusion::~DataFusion()
{

}

void DataFusion::run()
{

}
