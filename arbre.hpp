#ifndef ARBRE_HPP
#define ARBRE_HPP
#include <iostream>
#include <vector>
#include <fstream>
#include "noeud.hpp"
#include <set>
using namespace std;
class Arbre
{
public:
    Arbre();
    ~Arbre();
    // Ajouter un mot dans l'arbre
    int getnb();
    void insererMot(const string& mot);
    bool rechercherMot(const string& mot) const;
    vector<string> genererFormesGaddag(const string& mot)const;
    void construireGaddag(const vector<string>& mots);
    void chargerDictionnaire(const string& fichier);
    Noeud* getRacine() const;
    bool motDansDictionnaireBrut(const std::string& mot) const;

private:
    Noeud* racine;
    int nombreMots;
    set<string> dictionnaireBrut;
};

#endif // ARBRE_HPP
