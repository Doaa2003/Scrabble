#ifndef SACDELETTRES_HPP
#define SACDELETTRES_HPP

#include "lettre.hpp"
#include <vector>

class SacDeLettres {
private:
    std::vector<Lettre> lettres; // Stocke chaque lettre avec ses points
    std::vector<int> quantites;  // Nb restants de chaque lettre
    int totalLettres;            // Nb total de lettres restantes

public:
    SacDeLettres();
    Lettre piocherLettre();
    bool estVide();
    int getPointsDeLaLettre(char lettre) const;

};

#endif
