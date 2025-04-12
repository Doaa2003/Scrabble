#include "board.hpp"
#include "arbre.hpp"
#include "joueur.hpp"
#include "explorateur.hpp"

#include <iostream>
#include <sstream>

int main()
{
    Board plateau;
    Arbre dico;

    dico.chargerDictionnaire("../data/dico.txt");

    std::stringstream ss;
    ss << "J.............." << std::endl ;
    ss << "O.............." << std::endl ;
    ss << "U.............." << std::endl ;
    ss << "R.............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;


/*   ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "....PROJET....." << std::endl ;
    ss << ".......O......." << std::endl ;
    ss << ".......U......." << std::endl ;
    ss << ".......E......." << std::endl ;
    ss << ".......U......." << std::endl ;
    ss << ".....SCRABBLE.." << std::endl ;
    ss << "..............." << std::endl ;
    ss << "..............." << std::endl ;
*/


    plateau.load(ss);

    std::cout << "Plateau initial :\n";
    std::cout << plateau << "\n";

    SacDeLettres sac;
    Joueur joueur(sac);
    joueur.afficherLettres();

    Exploration exploration;
    exploration.chercherCoups(plateau, dico, joueur.getLettres());

    Coup meilleurCoup = exploration.trouverMeilleurCoup(plateau, sac);
    exploration.appliquerCoup(plateau, meilleurCoup);
    std::cout << "Plateau après coup :\n";
    std::cout << plateau;

    if (meilleurCoup.score > 0)
    {
        exploration.appliquerCoup(plateau, meilleurCoup);
        std::cout << "Plateau après coup :\n";
        std::cout << plateau;
    }
    return 0;
}
