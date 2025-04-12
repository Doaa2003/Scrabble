#include "explorateur.hpp"

#include <stack>
#include <iostream>

// Constructeur
Exploration::Exploration() {}

// ================================
// FONCTION : avancer
// ================================
Position Exploration::avancer(Position pos, Direction dir, bool avant)
{
    if (dir == HORIZONTAL)
        pos.colonne += (avant ? 1 : -1);
    else
        pos.ligne += (avant ? 1 : -1);

    return pos;
}
/*Position Exploration::avancer(Position pos, Direction dir, bool avant)
{
    if (dir == HORIZONTAL) {
        if (avant && pos.colonne < 14) {
            pos.colonne++;
        } else if (!avant && pos.colonne > 0) {
            pos.colonne--;
        }
    } else {
        if (avant && pos.ligne < 14) {
            pos.ligne++;
        } else if (!avant && pos.ligne > 0) {
            pos.ligne--;
        }
    }

    // Vérifier que la nouvelle position reste dans les bornes
    if (pos.ligne < 0 || pos.ligne >= 15 || pos.colonne < 0 || pos.colonne >= 15) {
        std::cout << "Erreur : tentative d'accès en dehors des limites du plateau (" << pos.ligne << ", " << pos.colonne << ")\n";
        return Position{-1, -1};  // Indiquer une position invalide
    }

    return pos;
}*/

// ================================
// FONCTION : calculerScore (simple)
// ================================
int Exploration::calculerScore(const Coup &coup, const Board &plateau, const SacDeLettres &sac)
{
    return static_cast<int>(coup.mot.size()); // Simple : 1 point par lettre
}

// ================================
// FONCTION : verifierMotsPerpendiculaires
// ================================
bool Exploration::verifierMotsPerpendiculaires(const Board &plateau, const Arbre &dico, const Position &positionLettrePosee, Direction directionPerpendiculaire)
{
    string mot = "";
    Position position = positionLettrePosee;

    // Reculer pour trouver le début du mot dans la direction perpendiculaire
    while (plateau(position.ligne, position.colonne).letter != 0) {
        if (directionPerpendiculaire == HORIZONTAL) {
            position.colonne--;
        } else {
            position.ligne--;
        }
    }

    // Avancer d'une position pour commencer le mot
    if (directionPerpendiculaire == HORIZONTAL) {
        position.colonne++;
    } else {
        position.ligne++;
    }

    // Construire le mot complet
    Position lecture = position;
    while ((plateau(lecture.ligne, lecture.colonne).letter != 0 || lecture == positionLettrePosee)) {
        if (lecture == positionLettrePosee) {
            // La lettre qu'on est en train de poser
            mot += plateau(positionLettrePosee.ligne, positionLettrePosee.colonne).letter;
        } else {
            mot += plateau(lecture.ligne, lecture.colonne).letter;
        }

        if (directionPerpendiculaire == HORIZONTAL) {
            lecture.colonne++;
        } else {
            lecture.ligne++;
        }
    }

    // Si le mot est de taille 1 => pas un vrai mot croisé
    if (mot.size() <= 1) return true;

    // Vérifie si le mot est dans le dictionnaire
    return dico.rechercherMot(mot);
}

// ================================
// FONCTION : explorerDepuisAncrage
// ================================
void Exploration::explorerDepuisAncrage(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur, const Position &ancrage, Direction direction)
{
    std::stack<EtatExploration> pile;
    pile.push({ancrage, dico.getRacine(), "", lettresJoueur, direction, false, ancrage, false});

    while (!pile.empty())
    {
        EtatExploration etat = pile.top();
        pile.pop();

        char lettrePlateau = plateau(etat.position.ligne, etat.position.colonne).letter;

        if (lettrePlateau != '\0')
        {
            if (etat.noeud->getEnfants().count(lettrePlateau))
            {
                Noeud *suivant = etat.noeud->getEnfant(lettrePlateau);
                std::string nouveauMot = etat.mot + lettrePlateau;
                Position prochainePosition = avancer(etat.position, etat.direction, etat.avant);

                pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, etat.avant, etat.positionDepart, true});
            }
            continue;
        }

        if (!etat.avant)
        {
            if (etat.noeud->getEnfants().count('+'))
            {
                Noeud *suivant = etat.noeud->getEnfant('+');
                std::string nouveauMot = etat.mot + '+';
                Position prochainePosition = avancer(etat.positionDepart, etat.direction, true);

                pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, true, etat.positionDepart, etat.utiliseLettrePlateau});
            }
        }
        else
        {
            for (size_t i = 0; i < etat.lettresRestantes.size(); ++i)
            {
                char lettre = etat.lettresRestantes[i].getCaractere();

                if (etat.noeud->getEnfants().count(lettre))
                {
                    Noeud *suivant = etat.noeud->getEnfant(lettre);
                    std::string nouveauMot = etat.mot + lettre;
                    Position prochainePosition = avancer(etat.position, etat.direction, etat.avant);

                    auto nouvellesLettres = etat.lettresRestantes;
                    nouvellesLettres.erase(nouvellesLettres.begin() + i);

                    if (verifierMotsPerpendiculaires(plateau, dico, etat.position, etat.direction))
                    {
                        if (suivant->getestTerminal() && etat.utiliseLettrePlateau)
                        {
                            Coup coup;
                            coup.mot = nouveauMot;
                            coup.ligne = etat.positionDepart.ligne;
                            coup.colonne = etat.positionDepart.colonne;
                            coup.direction = etat.direction;
                            coup.score = static_cast<int>(coup.mot.size());

                            coupsTrouves.push_back(coup);
                            motsTrouves.push_back(coup.mot);

                            std::cout << "Mot trouvé: " << coup.mot << " Score: " << coup.score << "\n";
                        }

                        pile.push({prochainePosition, suivant, nouveauMot, nouvellesLettres, etat.direction, etat.avant, etat.positionDepart, true});
                    }
                }
            }
        }
    }
}

/*void Exploration::explorerDepuisAncrage(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur, const Position &ancrage, Direction direction)
{
    std::stack<EtatExploration> pile;

    // Initialisation de la pile avec ton Arbre
    pile.push({ancrage, dico.getRacine(), "", lettresJoueur, direction, false, ancrage, false});

    while (!pile.empty())
    {
        EtatExploration etat = pile.top();
        pile.pop();

        Position pos = etat.position;
        
        // Vérifie les limites du plateau
        if (pos.ligne < 0 || pos.ligne >= 14 || pos.colonne < 0 || pos.colonne >= 14)
            continue;
        char lettrePlateau = plateau(pos.ligne, pos.colonne).letter;
        //std::cout << "Position actuelle: " << pos.ligne << ", " << pos.colonne << "\n"<<std::endl;
        //std::cout << "lettrePlateau: " << lettrePlateau << "\n"<<std::endl;

        // === CASE OCCUPÉE ===
        if (lettrePlateau != '\0')
        {
            if (!etat.noeud->getEnfants().count(lettrePlateau))
                continue;

            Noeud *suivant = etat.noeud->getEnfant(lettrePlateau);
            std::string nouveauMot = etat.mot + lettrePlateau;
            Position prochainePosition = avancer(pos, etat.direction, etat.avant);

            pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, etat.avant, etat.positionDepart, true});
            continue;
        }

        // === CASE VIDE ===
        // 1️⃣ Utiliser le "+" si pas encore utilisé
        if (!etat.avant && etat.noeud->getEnfants().count('+'))
        {
            Noeud *suivant = etat.noeud->getEnfant('+');
            std::string nouveauMot = etat.mot + '+';
            Position prochainePosition = avancer(etat.positionDepart, etat.direction, true);

            pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, true, etat.positionDepart, etat.utiliseLettrePlateau});
        }

        // 2️⃣ Essayer toutes les lettres restantes
        for (size_t i = 0; i < etat.lettresRestantes.size(); ++i)
        {
            char lettre = etat.lettresRestantes[i].getCaractere();

            if (!etat.noeud->getEnfants().count(lettre))
                continue;

            auto nouvellesLettres = etat.lettresRestantes;
            nouvellesLettres.erase(nouvellesLettres.begin() + i);

            Noeud *suivant = etat.noeud->getEnfant(lettre);
            std::string nouveauMot = etat.mot + lettre;

            Direction perpendiculaire = (etat.direction == HORIZONTAL) ? VERTICAL : HORIZONTAL;
            if (!verifierMotsPerpendiculaires(plateau, dico, pos, perpendiculaire))
                continue;

            if (suivant->getestTerminal() && etat.utiliseLettrePlateau)
            {
                Coup coup;
                coup.mot = nouveauMot;
                coup.ligne = etat.positionDepart.ligne;
                coup.colonne = etat.positionDepart.colonne;
                coup.direction = etat.direction;
                coup.score = static_cast<int>(coup.mot.size());

                coupsTrouves.push_back(coup);
                motsTrouves.push_back(coup.mot);

                std::cout << "✅ Mot trouvé: " << coup.mot << " Score: " << coup.score << "\n";
            }

            Position prochainePosition = avancer(pos, etat.direction, etat.avant);
            pile.push({prochainePosition, suivant, nouveauMot, nouvellesLettres, etat.direction, etat.avant, etat.positionDepart, true});
        }
    }
}*/
/*void Exploration::explorerDepuisAncrage(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur, const Position &ancrage, Direction direction)
{
    std::stack<EtatExploration> pile;

    // Initialisation de la pile avec la position d'ancrage
    pile.push({ancrage, dico.getRacine(), "", lettresJoueur, direction, false, ancrage, false});

    while (!pile.empty())
    {
        EtatExploration etat = pile.top();
        pile.pop();

        Position pos = etat.position;

        // Vérifie si la position est dans les limites du plateau (0 à 14)
        if (pos.ligne < 0 || pos.ligne >= 15 || pos.colonne < 0 || pos.colonne >= 15)
            continue;

        char lettrePlateau = plateau(pos.ligne, pos.colonne).letter;

        // Si la case est déjà occupée par une lettre
        if (lettrePlateau != '\0')
        {
            // Si la lettre sur le plateau correspond à celle dans l'arbre (dico)
            if (etat.noeud->getEnfants().count(lettrePlateau))
            {
                Noeud *suivant = etat.noeud->getEnfant(lettrePlateau);
                std::string nouveauMot = etat.mot + lettrePlateau;
                Position prochainePosition = avancer(pos, etat.direction, etat.avant);

                pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, etat.avant, etat.positionDepart, true});
            }
            continue;  // Si la lettre est déjà sur le plateau, on continue
        }

        // Si la case est vide
        // 1. Essayer de placer le "+"
        if (!etat.avant && etat.noeud->getEnfants().count('+'))
        {
            Noeud *suivant = etat.noeud->getEnfant('+');
            std::string nouveauMot = etat.mot + '+';
            Position prochainePosition = avancer(etat.positionDepart, etat.direction, true);

            pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, true, etat.positionDepart, etat.utiliseLettrePlateau});
        }

        // 2. Essayer toutes les lettres restantes
        for (size_t i = 0; i < etat.lettresRestantes.size(); ++i)
        {
            char lettre = etat.lettresRestantes[i].getCaractere();

            // Vérifie si le noeud contient l'enfant correspondant à la lettre
            if (!etat.noeud->getEnfants().count(lettre))
                continue;

            auto nouvellesLettres = etat.lettresRestantes;
            nouvellesLettres.erase(nouvellesLettres.begin() + i);  // Supprimer la lettre utilisée

            Noeud *suivant = etat.noeud->getEnfant(lettre);
            std::string nouveauMot = etat.mot + lettre;

            // Direction perpendiculaire pour vérifier les mots croisés
            Direction perpendiculaire = (etat.direction == HORIZONTAL) ? VERTICAL : HORIZONTAL;

            // Vérification des mots croisés
            if (!verifierMotsPerpendiculaires(plateau, dico, pos, perpendiculaire))
                continue;  // Si le mot croisé n'est pas valide, on continue

            // Si le noeud est terminal et qu'une lettre du plateau est utilisée, on ajoute le coup
            if (suivant->getestTerminal() && etat.utiliseLettrePlateau)
            {
                Coup coup;
                coup.mot = nouveauMot;
                coup.ligne = etat.positionDepart.ligne;
                coup.colonne = etat.positionDepart.colonne;
                coup.direction = etat.direction;
                coup.score = static_cast<int>(coup.mot.size());

                coupsTrouves.push_back(coup);
                motsTrouves.push_back(coup.mot);

                std::cout << "✅ Mot trouvé: " << coup.mot << " Score: " << coup.score << "\n";
            }

            // Calculer la prochaine position à explorer
            Position prochainePosition = avancer(pos, etat.direction, etat.avant);
            pile.push({prochainePosition, suivant, nouveauMot, nouvellesLettres, etat.direction, etat.avant, etat.positionDepart, true});
        }
    }
}
*/

/*void Exploration::explorerDepuisAncrage(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur, const Position &ancrage, Direction direction)
{
    std::stack<EtatExploration> pile;

    // Initialisation : on commence "avant" le +
    pile.push({ancrage, dico.getRacine(), "", lettresJoueur, direction, false, ancrage, false});

    while (!pile.empty())
    {
        EtatExploration etat = pile.top();
        pile.pop();

        Position prochainePosition = avancer(etat.position, etat.direction, etat.avant);

        // 🎯 Si la case est déjà occupée -> on avance, mais on n'utilise pas la lettre pour construire le mot principal
        if (plateau(etat.position.ligne, etat.position.colonne).letter != '\0')
        {
            // ✅ On avance seulement
            pile.push({prochainePosition, etat.noeud, etat.mot, etat.lettresRestantes, etat.direction, etat.avant, etat.positionDepart, etat.utiliseLettrePlateau || true});
            continue;
        }

        // 🟢 Si on est avant le "+", on place le + et on repart dans l'autre sens
        if (!etat.avant)
        {
            if (etat.noeud->getEnfants().count('+'))
            {
                Noeud *suivant = etat.noeud->getEnfant('+');
                std::string nouveauMot = etat.mot + '+';
                Position positionApresAncrage = avancer(etat.positionDepart, etat.direction, true);

                pile.push({positionApresAncrage, suivant, nouveauMot, etat.lettresRestantes, etat.direction, true, etat.positionDepart, etat.utiliseLettrePlateau});
            }
        }
        else
        {
            // 🔤 Essayer toutes les lettres du joueur pour la case vide
            for (size_t i = 0; i < etat.lettresRestantes.size(); ++i)
            {
                char lettre = etat.lettresRestantes[i].getCaractere();

                if (etat.noeud->getEnfants().count(lettre))
                {
                    Noeud *suivant = etat.noeud->getEnfant(lettre);
                    std::string nouveauMot = etat.mot + lettre;
                    auto nouvellesLettres = etat.lettresRestantes;
                    nouvellesLettres.erase(nouvellesLettres.begin() + i);

                    // ✅ Vérifier si les mots croisés sont valides
                    if (verifierMotsPerpendiculaires(plateau, dico, etat.position, etat.direction))
                    {
                        // 🏆 Vérifier si c’est un mot terminal et connecté au plateau
                        if (suivant->getestTerminal() && etat.utiliseLettrePlateau)
                        {
                            Coup coup;
                            coup.mot = nouveauMot;
                            coup.ligne = etat.positionDepart.ligne;
                            coup.colonne = etat.positionDepart.colonne;
                            coup.direction = etat.direction;
                            coup.score = static_cast<int>(coup.mot.size());

                            coupsTrouves.push_back(coup);
                            motsTrouves.push_back(coup.mot);

                            std::cout << "Mot trouvé: " << coup.mot << " Score: " << coup.score << "\n";
                        }

                        // ➕ Continuer l'exploration
                        pile.push({prochainePosition, suivant, nouveauMot, nouvellesLettres, etat.direction, etat.avant, etat.positionDepart, true});
                    }
                }
            }
        }
    }
}*/

/*void Exploration::explorerDepuisAncrage(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur, const Position &ancrage, Direction direction)
{
    std::stack<EtatExploration> pile;

    pile.push({ancrage, dico.getRacine(), "", lettresJoueur, direction, false, ancrage, false});

    while (!pile.empty())
    {
        EtatExploration etat = pile.top();
        pile.pop();

        Position pos = etat.position;

        // Si la position est hors du plateau, on ignore
        if (pos.ligne < 0 || pos.ligne >= 15 || pos.colonne < 0 || pos.colonne >= 15)
            continue;

        char lettrePlateau = plateau(pos.ligne, pos.colonne).letter;

        Position prochainePosition = avancer(pos, etat.direction, etat.avant);

        // === Si la case est déjà occupée ===
        if (lettrePlateau != '\0')
        {
            if (etat.noeud->getEnfants().count(lettrePlateau))
            {
                Noeud *suivant = etat.noeud->getEnfant(lettrePlateau);
                std::string nouveauMot = etat.mot + lettrePlateau;

                pile.push({prochainePosition, suivant, nouveauMot, etat.lettresRestantes, etat.direction, etat.avant, etat.positionDepart, true});
            }
            continue;
        }

        // === Si on est avant le "+", on pose le '+' et on change de sens ===
        if (!etat.avant && etat.noeud->getEnfants().count('+'))
        {
            Noeud *suivant = etat.noeud->getEnfant('+');
            std::string nouveauMot = etat.mot + '+';
            Position positionApresAncrage = avancer(etat.positionDepart, etat.direction, true);

            pile.push({positionApresAncrage, suivant, nouveauMot, etat.lettresRestantes, etat.direction, true, etat.positionDepart, etat.utiliseLettrePlateau});
        }

        // === Sinon, essayer toutes les lettres du joueur ===
        if (etat.avant)
        {
            for (size_t i = 0; i < etat.lettresRestantes.size(); ++i)
            {
                char lettre = etat.lettresRestantes[i].getCaractere();

                if (!etat.noeud->getEnfants().count(lettre))
                    continue;

                auto nouvellesLettres = etat.lettresRestantes;
                nouvellesLettres.erase(nouvellesLettres.begin() + i);

                Noeud *suivant = etat.noeud->getEnfant(lettre);
                std::string nouveauMot = etat.mot + lettre;

                if (!verifierMotsPerpendiculaires(plateau, dico, pos, etat.direction))
                    continue;

                if (suivant->getestTerminal() && etat.utiliseLettrePlateau)
                {
                    Coup coup;
                    coup.mot = nouveauMot;
                    coup.ligne = etat.positionDepart.ligne;
                    coup.colonne = etat.positionDepart.colonne;
                    coup.direction = etat.direction;
                    coup.score = static_cast<int>(coup.mot.size());

                    coupsTrouves.push_back(coup);
                    motsTrouves.push_back(coup.mot);

                    std::cout << "✅ Mot trouvé: " << coup.mot << " Score: " << coup.score << "\n";
                }

                pile.push({prochainePosition, suivant, nouveauMot, nouvellesLettres, etat.direction, etat.avant, etat.positionDepart, true});
            }
        }
    }
}
*/

// ================================
// FONCTION : chercherMeilleursCoups
// ================================
void Exploration::chercherCoups(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur)
{
    for (int ligne = 0; ligne < 15; ++ligne)
    {
        for (int colonne = 0; colonne < 15; ++colonne)
        {
            Position pos = {ligne, colonne};
            
            if (plateau(ligne, colonne).letter != '\0')
            {
                explorerDepuisAncrage(plateau, dico, lettresJoueur, pos, HORIZONTAL);
                explorerDepuisAncrage(plateau, dico, lettresJoueur, pos, VERTICAL);
            }
        }
    }
}

/*void Exploration::chercherCoups(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur)
{
    for (int ligne = 0; ligne < 15; ++ligne)
    {
        for (int colonne = 0; colonne < 15; ++colonne)
        {
            if (plateau(ligne, colonne).letter != '\0')
            {
                // Explore autour de la lettre du plateau
                Position pos;

                // Case au-dessus
                if (ligne > 0 && plateau(ligne - 1, colonne).letter == '\0')
                {
                    pos = {ligne - 1, colonne};
                    explorerDepuisAncrage(plateau, dico, lettresJoueur, pos, VERTICAL);
                }

                // Case en-dessous
                if (ligne < 14 && plateau(ligne + 1, colonne).letter == '\0')
                {
                    pos = {ligne + 1, colonne};
                    explorerDepuisAncrage(plateau, dico, lettresJoueur, pos, VERTICAL);
                }

                // Case à gauche
                if (colonne > 0 && plateau(ligne, colonne - 1).letter == '\0')
                {
                    pos = {ligne, colonne - 1};
                    explorerDepuisAncrage(plateau, dico, lettresJoueur, pos, HORIZONTAL);
                }

                // Case à droite
                if (colonne < 14 && plateau(ligne, colonne + 1).letter == '\0')
                {
                    pos = {ligne, colonne + 1};
                    explorerDepuisAncrage(plateau, dico, lettresJoueur, pos, HORIZONTAL);
                }
            }
        }
    }
}
*/
// ================================
// FONCTION : trouverMeilleurCoup
// ================================

Coup Exploration::trouverMeilleurCoup(const Board &plateau, const SacDeLettres &sac)
{
    if (coupsTrouves.empty()) {
        std::cout << "Aucun coup trouvé." << std::endl;
        return Coup();  // Retourne un coup vide si aucun coup n'a été trouvé
    }

    const Coup* meilleurCoup = &coupsTrouves.front();
    int meilleurScore = 0;

    for (const auto& coup : coupsTrouves) {
        int score = calculerScore(coup, plateau, sac);  // Calcule le score pour chaque coup
        if (score > meilleurScore) {
            meilleurCoup = &coup;
            meilleurScore = score;
        }
    }

    std::cout << "Meilleur coup : " << meilleurCoup->mot << " à (" << meilleurCoup->ligne << ", " << meilleurCoup->colonne << ") "
              << (meilleurCoup->direction == HORIZONTAL ? "Horizontal" : "Vertical")
              << " avec un score de " << meilleurScore << " points." << std::endl;

    return *meilleurCoup;  // Retourne le meilleur coup trouvé
}

// ================================
// FONCTION : appliquerCoup
// ================================
/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    Position pos = {coup.ligne, coup.colonne};

    for (char lettre : coup.mot)
    {
        if (lettre != '+')
        {
            if (plateau(pos.ligne, pos.colonne).letter == '\0')
            {
                plateau(pos.ligne, pos.colonne).letter = lettre;
            }
        }

        pos = avancer(pos, coup.direction, true);
    }

    std::cout << "Coup appliqué: " << coup.mot << " à (" << coup.ligne << ", " << coup.colonne << ")\n";
}*/

/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    // Position de départ = ancrage
    Position pos = {coup.ligne, coup.colonne};

    bool avantPlus = true; // On commence avant le '+'

    for (size_t i = 0; i < coup.mot.size(); ++i)
    {
        char lettre = coup.mot[i];

        if (lettre == '+')
        {
            // Quand on rencontre le +, on revient à la position de départ
            // Et on change de sens (on avance vers l'avant désormais)
            avantPlus = false;
            pos = {coup.ligne, coup.colonne}; // retour case départ
            continue; // on passe au prochain caractère
        }

        // Vérifie si la case est vide pour placer la lettre
        if (plateau(pos.ligne, pos.colonne).letter == '\0')
        {
            plateau(pos.ligne, pos.colonne).letter = lettre;
        }

        // Avancer dans la bonne direction
        pos = avancer(pos, coup.direction, !avantPlus);
    }

    std::cout << "Coup appliqué: " << coup.mot
              << " à (" << coup.ligne << ", " << coup.colonne << ")"
              << " en " << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";
}
*/
/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    Position pos = {coup.ligne, coup.colonne};
    bool avantPlus = true;

    for (size_t i = 0; i < coup.mot.size(); ++i)
    {
        int b = coup.mot.size();

        char lettre = coup.mot[i];

        if (lettre == '+')
        {
            b = i;
            avantPlus = false;
            pos = {coup.ligne, coup.colonne}; // retour case départ
            continue;
        }

        // ✅ Place la lettre d'abord
        if (plateau(pos.ligne, pos.colonne).letter == '\0' && i>b)
        {
            if(coup.direction == HORIZONTAL)
            {
                plateau(pos.ligne, pos.colonne+1).letter = lettre;
            }
            else
            {
                plateau(pos.ligne+1, pos.colonne).letter = lettre;
            }
        }
        else if (plateau(pos.ligne, pos.colonne).letter == '\0')
        {
            plateau(pos.ligne, pos.colonne).letter = lettre;
        }

        // ✅ Puis avance après avoir posé la lettre
        pos = avancer(pos, coup.direction, !avantPlus);
    }

    std::cout << "Coup appliqué: " << coup.mot
              << " à (" << coup.ligne << ", " << coup.colonne << ")"
              << " en " << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";
}
*/

/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    int ligne = coup.ligne;
    int colonne = coup.colonne;
    std::string mot = coup.mot;

    // Trouver la position du '+'
    int posPlus = mot.find('+');

    // Séparer le mot en deux parties : avant et après le '+'
    std::string avantPlus = mot.substr(0, posPlus);
    std::string apresPlus = mot.substr(posPlus + 1);

    // Placement selon la direction
    if (coup.direction == VERTICAL)
    {
        // 🔹 Avant le '+', vers le haut
        int idxLigne = ligne - 1;
        for (size_t i = 0; i < avantPlus.size(); ++i)
        {
            if (plateau(idxLigne, colonne).letter == '\0')
            {
                plateau(idxLigne, colonne).letter = avantPlus[i];
                std::cout << "Placer lettre '" << avantPlus[i] << "' à (" << idxLigne << ", " << colonne << ")\n";
            }
            idxLigne--; // vers le haut
        }

        // 🔹 Après le '+', vers le bas
        int idxLigneDroite = ligne;
        for (size_t i = 0; i < apresPlus.size(); ++i)
        {
            if (plateau(idxLigneDroite, colonne).letter == '\0')
            {
                plateau(idxLigneDroite, colonne).letter = apresPlus[i];
                std::cout << "Placer lettre '" << apresPlus[i] << "' à (" << idxLigneDroite << ", " << colonne << ")\n";
            }
            idxLigneDroite++; // vers le bas
        }
    }
    else if (coup.direction == HORIZONTAL)
    {
        // 🔹 Avant le '+', vers la gauche
        int idxColonne = colonne - 1;
        for (size_t i = 0; i < avantPlus.size(); ++i)
        {
            if (plateau(ligne, idxColonne).letter == '\0')
            {
                plateau(ligne, idxColonne).letter = avantPlus[i];
                std::cout << "Placer lettre '" << avantPlus[i] << "' à (" << ligne << ", " << idxColonne << ")\n";
            }
            idxColonne--; // vers la gauche
        }

        // 🔹 Après le '+', vers la droite
        int idxColonneDroite = colonne;
        for (size_t i = 0; i < apresPlus.size(); ++i)
        {
            if (plateau(ligne, idxColonneDroite).letter == '\0')
            {
                plateau(ligne, idxColonneDroite).letter = apresPlus[i];
                std::cout << "Placer lettre '" << apresPlus[i] << "' à (" << ligne << ", " << idxColonneDroite << ")\n";
            }
            idxColonneDroite++; // vers la droite
        }
    }

    std::cout << "Coup appliqué: " << coup.mot
              << " à (" << coup.ligne << ", " << coup.colonne << ")"
              << " en " << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";
}
*/
/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    int ligne = coup.ligne;
    int colonne = coup.colonne;
    const std::string& mot = coup.mot;

    // Trouver la position du '+'
    int posPlus = mot.find('+');

    // Séparer le mot en deux parties : avant et après le '+'
    std::string avantPlus = mot.substr(0, posPlus);
    std::string apresPlus = mot.substr(posPlus + 1);

    std::cout << "Appliquer le coup : " << mot << " à la position (" << ligne << ", " << colonne << ") en "
              << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";

    if (coup.direction == VERTICAL)
    {
        // Partie avant le '+', vers le haut
        int idxLigneAvant = ligne - 1;
        for (size_t i = 0; i < avantPlus.size(); ++i)
        {
            if (plateau(idxLigneAvant, colonne).letter == '\0')
            {
                plateau(idxLigneAvant, colonne).letter = avantPlus[i];
                std::cout << "Placer lettre '" << avantPlus[i] << "' à (" << idxLigneAvant << ", " << colonne << ")\n";
            }
            else
            {
                std::cout << "Case déjà occupée par '" << plateau(idxLigneAvant, colonne).letter << "' à (" << idxLigneAvant << ", " << colonne << ")\n";
            }
            idxLigneAvant--;
        }

        // Partie après le '+'
        std::cout << "Lettre d'ancrage déjà présente : '" << apresPlus[0] << "' à (" << ligne << ", " << colonne << ")\n";

        int idxLigneApres = ligne + 1; // Commencer après l'ancrage
        for (size_t i = 1; i < apresPlus.size(); ++i)
        {
            if (plateau(idxLigneApres, colonne).letter == '\0')
            {
                plateau(idxLigneApres, colonne).letter = apresPlus[i];
                std::cout << "Placer lettre '" << apresPlus[i] << "' à (" << idxLigneApres << ", " << colonne << ")\n";
            }
            else
            {
                std::cout << "Case déjà occupée par '" << plateau(idxLigneApres, colonne).letter << "' à (" << idxLigneApres << ", " << colonne << ")\n";
            }
            idxLigneApres++;
        }
    }
    else if (coup.direction == HORIZONTAL)
    {
        // Partie avant le '+', vers la gauche
        int idxColAvant = colonne - 1;
        for (size_t i = 0; i < avantPlus.size(); ++i)
        {
            if (plateau(ligne, idxColAvant).letter == '\0')
            {
                plateau(ligne, idxColAvant).letter = avantPlus[i];
                std::cout << "Placer lettre '" << avantPlus[i] << "' à (" << ligne << ", " << idxColAvant << ")\n";
            }
            else
            {
                std::cout << "Case déjà occupée par '" << plateau(ligne, idxColAvant).letter << "' à (" << ligne << ", " << idxColAvant << ")\n";
            }
            idxColAvant--;
        }

        // Partie après le '+'
        std::cout << "Lettre d'ancrage déjà présente : '" << apresPlus[0] << "' à (" << ligne << ", " << colonne << ")\n";

        int idxColApres = colonne + 1; // Commencer après l'ancrage
        for (size_t i = 1; i < apresPlus.size(); ++i)
        {
            if (plateau(ligne, idxColApres).letter == '\0')
            {
                plateau(ligne, idxColApres).letter = apresPlus[i];
                std::cout << "Placer lettre '" << apresPlus[i] << "' à (" << ligne << ", " << idxColApres << ")\n";
            }
            else
            {
                std::cout << "Case déjà occupée par '" << plateau(ligne, idxColApres).letter << "' à (" << ligne << ", " << idxColApres << ")\n";
            }
            idxColApres++;
        }
    }

    std::cout << "Coup appliqué terminé.\n";
}
*/

/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    int ligne = coup.ligne;
    int colonne = coup.colonne;
    const std::string& mot = coup.mot;

    int posPlus = mot.find('+');
    std::string avantPlus = mot.substr(0, posPlus);
    std::string apresPlus = mot.substr(posPlus + 1);

    std::string motNettoye = mot;

    std::cout << "Appliquer le coup : " << motNettoye << " à la position (" << ligne << ", " << colonne << ") en "
              << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";

    if (coup.direction == VERTICAL)
    {
        int idxLigneAvant = ligne - 1;
        for (char lettre : avantPlus)
        {
            if (plateau(idxLigneAvant, colonne).letter == '\0')
                plateau(idxLigneAvant, colonne).letter = lettre;
            idxLigneAvant--;
        }

        int idxLigneApres = ligne + 1;
        for (size_t i = 1; i < apresPlus.size(); ++i)
        {
            if (plateau(idxLigneApres, colonne).letter == '\0')
                plateau(idxLigneApres, colonne).letter = apresPlus[i];
            idxLigneApres++;
        }
    }
    else // HORIZONTAL
    {
        int idxColAvant = colonne - 1;
        for (char lettre : avantPlus)
        {
            if (plateau(ligne, idxColAvant).letter == '\0')
                plateau(ligne, idxColAvant).letter = lettre;
            idxColAvant--;
        }

        int idxColApres = colonne + 1;
        for (size_t i = 1; i < apresPlus.size(); ++i)
        {
            if (plateau(ligne, idxColApres).letter == '\0')
                plateau(ligne, idxColApres).letter = apresPlus[i];
            idxColApres++;
        }
    }

    std::cout << "Coup appliqué terminé.\n";
}
*/

/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    int ligne ;
    int colonne;
    if(coup.direction==HORIZONTAL){
        ligne = coup.ligne;
        colonne = coup.colonne+1;
    }
    else{
        ligne = coup.ligne+1;
        colonne = coup.colonne;
    }
    
    const std::string &mot = coup.mot;

    int posPlus = mot.find('+');

    std::string gauche = mot.substr(0, posPlus);         // partie avant le +
    std::string droite = mot.substr(posPlus + 1);        // partie après le +

    std::cout << "✅ Appliquer le coup : " << mot << " à la position (" << ligne << ", " << colonne << ") en "
              << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";

    if (coup.direction == VERTICAL)
    {
        // Poser vers l’arrière (avant le +)
        int idx = ligne;
        for (int i = gauche.size() - 1; i >= 0; --i) // attention ordre inverse !
        {
            idx--;
            if (idx >= 0 && plateau(idx, colonne).letter == '\0')
            {
                plateau(idx, colonne).letter = gauche[i];
                std::cout << "📝 Placer lettre '" << gauche[i] << "' à (" << idx << ", " << colonne << ")\n";
            }
        }

        // Poser vers l’avant (après le +)
        idx = ligne;
        for (char lettre : droite)
        {
            if (plateau(idx, colonne).letter == '\0')
            {
                plateau(idx, colonne).letter = lettre;
                std::cout << "📝 Placer lettre '" << lettre << "' à (" << idx << ", " << colonne << ")\n";
            }
            idx++;
        }
    }
    else if (coup.direction == HORIZONTAL)
    {
        // Poser vers l’arrière (avant le +)
        int idx = colonne;
        for (int i = gauche.size() - 1; i >= 0; --i) // attention ordre inverse !
        {
            idx--;
            if (idx >= 0 && plateau(ligne, idx).letter == '\0')
            {
                plateau(ligne, idx).letter = gauche[i];
                std::cout << "📝 Placer lettre '" << gauche[i] << "' à (" << ligne << ", " << idx << ")\n";
            }
        }

        // Poser vers l’avant (après le +)
        idx = colonne;
        for (char lettre : droite)
        {
            if (plateau(ligne, idx).letter == '\0')
            {
                plateau(ligne, idx).letter = lettre;
                std::cout << "📝 Placer lettre '" << lettre << "' à (" << ligne << ", " << idx << ")\n";
            }
            idx++;
        }
    }

    std::cout << "✅ Coup appliqué terminé.\n";
}
*/

/*void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    const std::string &mot = coup.mot;

    int posPlus = mot.find('+');
    std::string gauche = mot.substr(0, posPlus);         // partie avant le +
    std::string droite = mot.substr(posPlus + 1);        // partie après le +

    std::cout << "✅ Appliquer le coup : " << mot << " à la position (" << coup.ligne << ", " << coup.colonne << ") en "
              << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";

    // Position de départ (ancrage)
    Position ancrage = {coup.ligne, coup.colonne};

    // === 1. Placer les lettres vers l’arrière (avant le '+') ===
    Position pos = ancrage;
    for (char lettre : gauche)
    {
        pos = avancer(pos, coup.direction, false); // On recule

        if (pos.ligne >= 0 && pos.colonne >= 0 && pos.ligne < 15 && pos.colonne < 15 && plateau(pos.ligne, pos.colonne).letter == '\0')
        {
            plateau(pos.ligne, pos.colonne).letter = lettre;
            std::cout << "📝 Placer lettre '" << lettre << "' à (" << pos.ligne << ", " << pos.colonne << ")\n";
        }
    }

    // === 2. Placer les lettres vers l’avant (après le '+') ===
    pos = ancrage; // Revenir à la position d'ancrage
    for (char lettre : droite)
    {
        if (pos.ligne >= 0 && pos.colonne >= 0 && pos.ligne < 15 && pos.colonne < 15 && plateau(pos.ligne, pos.colonne).letter == '\0')
        {
            plateau(pos.ligne, pos.colonne).letter = lettre;
            std::cout << "📝 Placer lettre '" << lettre << "' à (" << pos.ligne << ", " << pos.colonne << ")\n";
        }

        pos = avancer(pos, coup.direction, true); // On avance
    }

    std::cout << "✅ Coup appliqué terminé.\n";
}*/


void Exploration::appliquerCoup(Board &plateau, const Coup &coup)
{
    const std::string &mot = coup.mot;
    std::cout << mot <<std:: endl;
    int posPlus = mot.find('+');
    std::string gauche = mot.substr(0, posPlus);         // avant +
    std::string droite = mot.substr(posPlus + 1);        // après +

    std::cout << "✅ Appliquer le coup : " << mot << " à la position (" << coup.ligne << ", " << coup.colonne << ") en "
              << (coup.direction == HORIZONTAL ? "HORIZONTAL" : "VERTICAL") << "\n";

    // Position de départ (ancrage)
    Position ancrage = {coup.ligne, coup.colonne};

    // === 1. Poser vers l'arrière (avant le +) ===
    Position pos = ancrage;
    if(gauche[0]== coup.mot[0])
        {
            int a = gauche.size();
            std::cout << "Lettre d'ancrage déjà présente : '" << gauche[0] << "' à (" << pos.ligne << ", " << pos.colonne << ")\n";
            char lettre = gauche[a-2];
            string g = "";
            std::cout << lettre <<std:: endl;
            for(int i = 0; i<a-1; i++)
            {
                //std::cout << "gauche1 " << gauche[i] <<std:: endl;
                g +=gauche[i+1];
               // std::cout << "gauche " << g <<std:: endl;
            }
            g[a]=lettre;
            //std::cout << "gauche2 " << g[a] <<std:: endl;
            gauche = g;
        }
    for (char lettre : gauche)
    {
        pos = avancer(pos, coup.direction, false); // recule
       
      
        if (pos.ligne >= 0 && pos.colonne >= 0 && pos.ligne < 15 && pos.colonne < 15 && plateau(pos.ligne, pos.colonne).letter == '\0')
        {
            plateau(pos.ligne, pos.colonne--).letter = lettre;
            std::cout << "📝 Placer lettre '" << lettre << "' à (" << pos.ligne << ", " << pos.colonne++ << ")\n";
        }
    }

    // === 2. Poser vers l’avant (après le +) ===
    pos = avancer(ancrage, coup.direction, true); // ⬅️ AVANCER DIRECTEMENT pour ne PAS réécrire la lettre de l'ancrage

    for (char lettre : droite)
    {
        
        if (pos.ligne >= 0 && pos.colonne >= 0 && pos.ligne < 15 && pos.colonne < 15 && plateau(pos.ligne, pos.colonne).letter == '\0')
        {
            plateau(pos.ligne, pos.colonne).letter = lettre;
            std::cout << "📝 Placer lettre '" << lettre << "' à (" << pos.ligne << ", " << pos.colonne << ")\n";
        }

        pos = avancer(pos, coup.direction, true); // avancer
    }

    std::cout << " Coup appliqué terminé.\n";
}

