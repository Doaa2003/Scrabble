#include "lettre.hpp"
#include <vector>



Lettre::Lettre(char l, int p) : caractere(l), points(p) {}

char Lettre::getCaractere() const {
    return caractere;
}

int Lettre::getPoints() const {
    return points;
}
