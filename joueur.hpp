#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <vector>
#include "lettre.hpp"
#include "sac_lettre.hpp"


class Joueur {
private:
    std::vector<Lettre> lettresJoueur;
    static const int NB_LETRES_DEBUT = 7;

public:
    // Constructeur : initialise les 7 lettres du joueur à partir du sac de lettres
    Joueur(SacDeLettres& sac);

    // Fonction pour piocher une lettre depuis le sac
    void piocherLettre(SacDeLettres& sac);

    // Fonction pour recharger les lettres du joueur depuis le sac
    void recharger(SacDeLettres& sac);

    // Affiche les lettres du joueur
    void afficherLettres() const;

    // Fonction statique pour obtenir les points d'une lettre donnée
    int getPoints(const Lettre& lettre);

    std::vector<Lettre> getLettres() const;
};

#endif // JOUEUR_HPP
