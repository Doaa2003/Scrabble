#include "arbre.hpp"
#include  "noeud.hpp"
#include <algorithm>
#include <string>
Arbre::Arbre() {
    racine = new Noeud();
    nombreMots = 0;
}

Arbre::~Arbre() {
    delete racine;
}
int Arbre::getnb(){
    return nombreMots;
}

Noeud* Arbre::getRacine() const {
    return racine;
}

void Arbre::insererMot(const string &mot){
    Noeud* courant = racine;
            for (char l : mot) {
                courant->ajouterEnfant(l);
                courant = courant->getEnfant(l);
            }
            courant->setEstTerminal(true);
}

// Vérifier si un mot est présent dans l'arbre
bool Arbre::rechercherMot(const string& mot) const{
    Noeud* courant = racine;
    for (char lettre : mot) {
        courant = courant->getEnfant(lettre);
        if (!courant) return false;
    }

    return courant->getestTerminal();
}


// Générer toutes les formes d’un mot pour le Gaddag
vector<string> Arbre::genererFormesGaddag(const string& mot) const{
    vector<string> variantes;
    for (size_t i = 1; i < mot.size(); ++i)
    {
        string pg = mot.substr(0, i);           
        string pd = mot.substr(i);              
        reverse(pg.begin(), pg.end());     
        variantes.push_back(pg + '+' + pd);
    }
    return variantes;
}

// Construire le Gaddag à partir d'une liste de mots
void Arbre::construireGaddag(const vector<string>& mots) {
    for (const string& mot : mots) {
        vector<string> formes = genererFormesGaddag(mot);
        for (const string& forme : formes) {
            insererMot(forme);
        }
        nombreMots++;
    }
}


// Charger un dictionnaire à partir d’un fichier texte
void Arbre::chargerDictionnaire(const string& fichier) {
    ifstream fluxFichier(fichier);
    string mot;
    vector<string> mots;
    while (fluxFichier >> mot) {
        mots.push_back(mot);
        dictionnaireBrut.insert(mot); // Ajout dans le dictionnaire brut
    }
    construireGaddag(mots);
}


bool Arbre::motDansDictionnaireBrut(const std::string& mot) const {
    return dictionnaireBrut.count(mot) > 0;
}
