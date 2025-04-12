#include "joueur.hpp"
#include <iostream>
#include <cstdlib> 
#include <vector> 


// Constructeur : initialise les 7 lettres du joueur à partir du sac de lettres
Joueur::Joueur(SacDeLettres& sac) {
    for (int i = 0; i < NB_LETRES_DEBUT; ++i) {
        piocherLettre(sac);  // Pioche une lettre pour le joueur
    }
}

// Fonction pour piocher une lettre depuis le sac
/*void Joueur::piocherLettre(SacDeLettres& sac) {
    if (!sac.estVide()) {
        Lettre lettrePiochée = sac.piocherLettre();  // Pioche une lettre du sac
        lettresJoueur.push_back(lettrePiochée);  // Ajoute la lettre piochée à la main du joueur
    } else {
        std::cout << "Le sac de lettres est vide, impossible de piocher de nouvelles lettres." << std::endl;
    }


}*/
// Fonction pour piocher une lettre depuis le sac
//#include <vector>  // Pour utiliser std::vector

void Joueur::piocherLettre(SacDeLettres& sac) {
    // Créer les lettres spécifiques avec leurs points
    std::vector<Lettre> lettresSpecifiques = {
        Lettre('E', 1), Lettre('E', 1), Lettre('U', 1),
        Lettre('E', 1), Lettre('B', 3), Lettre('N', 1),
        Lettre('L', 1)
    };

    // Effacer les lettres existantes dans la main du joueur
    lettresJoueur.clear();  // S'assurer que la main du joueur est vide avant d'ajouter les nouvelles lettres

    // Ajouter directement ces lettres à la main du joueur
    for (const auto& lettre : lettresSpecifiques) {
        lettresJoueur.push_back(lettre);
    }

    // Afficher les lettres du joueur
    afficherLettres();
}



// Fonction pour recharger les lettres du joueur depuis le sac
void Joueur::recharger(SacDeLettres& sac) {
    while (lettresJoueur.size() < NB_LETRES_DEBUT && !sac.estVide()) {
        piocherLettre(sac);  // Pioche une nouvelle lettre du sac si le joueur en a moins de 7
    }
}

// Affiche les lettres du joueur et leurs points
void Joueur::afficherLettres() const {
    std::cout << "Lettres du joueur : ";
    for (const auto& lettre : lettresJoueur) {
        std::cout << lettre.getCaractere() << "(" << lettre.getPoints() << ") ";
    }
    std::cout << std::endl;
}

// Fonction statique pour obtenir les points d'une lettre donnée
int Joueur::getPoints(const Lettre& lettre) {
    return lettre.getPoints();
}

std::vector<Lettre> Joueur::getLettres() const {
    return lettresJoueur;
}

