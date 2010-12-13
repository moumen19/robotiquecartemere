#include "Sensors.hpp"

Sensor::Sensor()
{
	this->a_id = 0;
	this->a_size = 0;
	this->a_name = "";
}

Sensor::Sensor(int id, std::string name, int size)
{
	this->a_id = id;
	this->a_size = size;
	this->a_name = name;
}

Sensor::~Sensor()
{

}

int Sensor::getId()
{
	return this->a_id;
}

int Sensor::getSize()
{
	return this->a_size;
}

std::string Sensor::getName()
{
	return this->a_name;
}

void Sensor::setId(int id)
{
	this->a_id = id;
}

void Sensor::setSize(int size)
{
	this->a_size = size;
}

void Sensor::setName(std::string name)
{
	this->a_name = name;
}





Sensors::Sensors()
{

}

Sensors::~Sensors()
{

}


void Sensors::addSensor(int id, std::string name, int size)
{
	Sensor * s = new Sensor(id, name, size);
	this->a_sensorName[s->getName()] = s;
	this->a_sensorId[s->getId()] = s;
}


bool Sensors::sensorExist(int id)
{
	if(a_sensorId.find(id) != a_sensorId.end())
		return true;
	return false;
}

bool Sensors::sensorExist(std::string name)
{
	if(a_sensorName.find(name) != a_sensorName.end())
		return true;
	return false;
}

Sensor Sensors::getSensor(int id)
{
	if(a_sensorId.find(id) != a_sensorId.end())
		return *a_sensorId[id];

	_DEBUG("Aucun capteur ne possede l'identifiant specifie", WARNING);	// A remplacer par une exeption
	
	Sensor s;
	return s;
}

Sensor Sensors::getSensor(std::string name)
{
	if(a_sensorName.find(name) != a_sensorName.end())
		return *a_sensorName[name];

	_DEBUG("Aucun capteur ne possede le nom specifie", WARNING);	// A remplacer par une exeption

	Sensor s;
	return s;
}

