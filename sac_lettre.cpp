#include "sac_lettre.hpp"
#include <cstdlib>
#include <ctime>

SacDeLettres::SacDeLettres() {
    char caracteres[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                         'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2,
                    1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

    int quantitesInit[] = {9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3,
                           6, 6, 2, 1, 6, 6, 6, 6, 2, 1, 1, 1, 1};

    totalLettres = 0;

    // init sac de lettres en remplissant les structures lettres et quantites
    for (int i = 0; i < 26; ++i) {
        lettres.push_back(Lettre(caracteres[i], points[i])); // Ajoute une lettre dans le vecteur lettres
        quantites.push_back(quantitesInit[i]); // Ajoute la quantité init de la lettre dans quantites
        totalLettres += quantitesInit[i];
    }

    // Initialisation du générateur de nombres aléatoires
    srand(static_cast<unsigned int>(time(0)));
}

// Piocher une lettre
Lettre SacDeLettres::piocherLettre() {
    if (totalLettres == 0) {
        return Lettre('\0', 0); // Sac vide
    }

    int indiceT = rand() % totalLettres; // Tirage d'un index entre 0 et totalLettres - 1
    int somme = 0;

    for (int i = 0; i < 26; ++i) {
        somme += quantites[i]; // Ajoute la quantité de la lettre actuelle
        if (indiceT < somme) {
            quantites[i]--;  // On retire une lettre du sac
            totalLettres--;
            return lettres[i];
        }
    }

    return Lettre('\0', 0); // Retour de la lettre vide en cas d'erreur
}

bool SacDeLettres::estVide(){
    if(totalLettres == 0){
        return true;
    }else{
        return false;
    }
}

int SacDeLettres::getPointsDeLaLettre(char lettre) const {
    for (size_t i = 0; i < lettres.size(); ++i) {
        if (lettres[i].getCaractere() == lettre) {
            return lettres[i].getPoints();
        }
    }
    return 0; // Si la lettre n'existe pas (ne devrait pas arriver)
}

