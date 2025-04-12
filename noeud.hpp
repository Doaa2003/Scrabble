#ifndef NOEUD_HPP
#define NOEUD_HPP

#include <iostream>
#include <map>
using namespace std;

class Noeud
{
public:
    Noeud();
    bool getestTerminal() const;
    const map<char, Noeud*>& getEnfants() const;
    Noeud* getEnfant(char l); // Renvoie un pointeur vers un enfant
    void ajouterEnfant(char l);
    void setEstTerminal(bool T);
private:
    bool estTerminal;  // Indique si ce nœud marque la fin d'un mot
    map<char, Noeud*> enfants; // Stocke les enfants sous forme de pointeurs
};

#endif // NOEUD_HPP
