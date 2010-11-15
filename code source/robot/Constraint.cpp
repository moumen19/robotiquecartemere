#include "Constraint.hpp"

Constraint::Constraint()
{
    if(_DEBUG_MODE)
        _DEBUG::addMessage("Constraint.cpp", "Initialisation du module de stockage des contraintes", INFORMATION);
}

Constraint::~Constraint()
{

}

double Constraint::get(int nb)
{
    return a_data[nb];
}

void Constraint::set(int nb, double value)
{
    a_data[nb] = value;
}
