#include "Constraint.hpp"

Constraint::Constraint()
{

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
