#include "Sensors.hpp"

/**
 * Capteurs
 */
Sensor::Sensor()
{
	this->a_id = 0;
	this->a_size = 0;
	this->a_name = "";
}

/**
 * Constructeur
 * @param id - l'identifiant du capteur
 * @param name - le nom du capteur
 * @param size - la taille du message capteur
 */
Sensor::Sensor(int id, std::string name, int size)
{
	this->a_id = id;
	this->a_size = size;
	this->a_name = name;
}

/**
 * Destructeur
 */
Sensor::~Sensor()
{

}

/**
 * Methode retournant l'identifiant du capteur
 * @return l'identifiant du capteur
 */
int Sensor::getId()
{
	return this->a_id;
}

/**
 * Methode retournant la taille du message capteur
 * @return la taille du message capteur
 */
int Sensor::getSize()
{
	return this->a_size;
}
/**
 * Methode retournant le nom de capteur
 * @return le nom du capteur
 */
std::string Sensor::getName()
{
	return this->a_name;
}

/**
 * Methode definissant l'identifiant du capteur
 * @param id - l'identifiant du capteur
 */
void Sensor::setId(int id)
{
	this->a_id = id;
}

/**
 * Methode definissant la taille du message capteur
 * @param size - la taille du message capteur
 */
void Sensor::setSize(int size)
{
	this->a_size = size;
}

/**
 * Methode definissant le nom du capteur
 * @param name - le nom du capteur
 */
void Sensor::setName(std::string name)
{
	this->a_name = name;
}




/**
 * Constructeur
 */
Sensors::Sensors()
{

}

/**
 * Destructeur
 */
Sensors::~Sensors()
{

}

/**
 * Ajoute un capteur au robot
 * @param id - l'identifiant du capteur
 * @param name - nom du capteur
 * @param size - la taille du message capteur
 */throw std::out_of_range("ERREUR : Aucune trajectoire stocke... !")
void Sensors::addSensor(int id, std::string name, int size)
{
	Sensor * s = new Sensor(id, name, size);
	this->a_sensorName[s->getName()] = s;
	this->a_sensorId[s->getId()] = s;
}

/**
 * Methode testant l'existance d'un capteur
 * @param id - l'identifiant du capteur
 * @result true si le capteur existe, false sinon
 */
bool Sensors::sensorExist(int id)
{
	if(a_sensorId.find(id) != a_sensorId.end())
		return true;
	return false;
}

/**
 * Methode testant l'existance d'un capteur
 * @param name - le nom du capteur
 * @return true si le capteur existe, false sinon
 */
bool Sensors::sensorExist(std::string name)
{
	if(a_sensorName.find(name) != a_sensorName.end())
		return true;
	return false;
}

/**
 * Methode retournant un capteur
 * @param id - l'identifiant du capteur
 * @return le capteur en question
 */
Sensor Sensors::getSensor(int id)
{
	if(a_sensorId.find(id) == a_sensorId.end())
		throw std::out_of_range("ERREUR : Le capteur demande est inexistant !");
	
	return *a_sensorId[id];;
}

/**
 * Methode retournant un capteur
 * @param name - le nom du capteur
 * @return le capteur en question
 */
Sensor Sensors::getSensor(std::string name)
{
	if(a_sensorName.find(name) == a_sensorName.end())			
		throw std::out_of_range("ERREUR : Le capteur demande est inexistant !");

	return *a_sensorName[name];
}

