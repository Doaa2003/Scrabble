#ifndef EXPLORATEUR_HPP
#define EXPLORATEUR_HPP

#include "board.hpp"
#include "arbre.hpp"
#include "lettre.hpp"
#include "sac_lettre.hpp"

#include <vector>
#include <string>

// Direction de placement du mot
enum Direction
{
    HORIZONTAL,
    VERTICAL
};

// Position sur le plateau
struct Position
{
    int ligne;
    int colonne;

    bool operator==(const Position &other) const
    {
        return ligne == other.ligne && colonne == other.colonne;
    }
};

// Structure représentant un coup trouvé
struct Coup
{
    std::string mot;
    int ligne;
    int colonne;
    Direction direction;
    int score = 0;
};

// Structure de la pile d’exploration
struct EtatExploration
{
    Position position;
    Noeud *noeud;
    std::string mot;
    std::vector<Lettre> lettresRestantes;
    Direction direction;
    bool avant;
    Position positionDepart;
    bool utiliseLettrePlateau;
};

class Exploration
{
public:
    Exploration();

    void chercherCoups(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur);
    Coup trouverMeilleurCoup(const Board &plateau, const SacDeLettres &sac);
    void appliquerCoup(Board &plateau, const Coup &coup);

private:
    std::vector<Coup> coupsTrouves;
    std::vector<std::string> motsTrouves;

    void explorerDepuisAncrage(const Board &plateau, const Arbre &dico, const std::vector<Lettre> &lettresJoueur, const Position &ancrage, Direction direction);
    Position avancer(Position pos, Direction dir, bool avant);
    int calculerScore(const Coup &coup, const Board &plateau, const SacDeLettres &sac);
    bool verifierMotsPerpendiculaires(const Board &plateau, const Arbre &dico, const Position &positionLettrePosee, Direction directionPerpendiculaire);
};

#endif // EXPLORATEUR_HPP
