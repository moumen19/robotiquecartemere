#include "Constraint.hpp"

Constraint::Constraint()
{
    _DEBUG("Initialisation du module de stockage des contraintes", INFORMATION);
}

Constraint::~Constraint()
{

}

double Constraint::get(int nb)
{
    return this->a_data[nb];
}

void Constraint::set(int nb, double value)
{
    this->a_data[nb] = value;
}
