#include "noeud.hpp"
#include <fstream>

Noeud::Noeud() {
    estTerminal = false;
}

bool Noeud::getestTerminal() const {
    return estTerminal;
}

const map<char, Noeud*>& Noeud::getEnfants() const {
        return enfants;
    }
Noeud* Noeud::getEnfant(char l) {
    // Vérifie si l'enfant existe dans la map
    auto it = enfants.find(l);
    if (it != enfants.end()) {
        return it->second;
    }
    return nullptr;  // Retourne nullptr si l'enfant n'existe pas
}
void Noeud::ajouterEnfant(char l){
    if (enfants.find(l) == enfants.end()) {
        enfants[l] = new Noeud();
    }
}

void Noeud ::setEstTerminal(bool T){
    estTerminal = T;
}
