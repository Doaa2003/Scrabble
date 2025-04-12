#include "board.hpp"
#include <fstream>
#include <iostream>

// Initialisation par défaut du plateau avec les bonus
Board::Board() {
    // Triple word spots
    {
        Spot s(1, 3);
        static const unsigned char sz = 9;
        static unsigned char triple_words[sz] = {
            0, 7, 14,
            105, 119,
            210, 217, 224
        };
        for (unsigned char i = 0; i < sz; ++i) {
            spots[triple_words[i]] = s;
        }
    }

    // Double word spots
    {
        Spot s(1, 2);
        static const unsigned char sz = 17;
        static unsigned char double_words[sz] = {
            16, 28,
            32, 42,
            48, 56,
            64, 70,
            112,
            154, 160,
            168, 176,
            182, 192,
            196, 208
        };
        for (unsigned char i = 0; i < sz; ++i) {
            spots[double_words[i]] = s;
        }
    }

    // Triple letter spots
    {
        Spot s(3, 1);
        static const unsigned char sz = 12;
        static unsigned char triple_letters[sz] = {
            20, 24,
            76, 80, 84, 88,
            136, 140, 144, 148,
            200, 204
        };
        for (unsigned char i = 0; i < sz; ++i) {
            spots[triple_letters[i]] = s;
        }
    }

    // Double letter spots
    {
        Spot s(2, 1);
        static const unsigned char sz = 24;
        static unsigned char double_letters[sz] = {
            3, 11,
            36, 38,
            45, 52, 59,
            92, 96, 98, 102,
            108, 116,
            122, 126, 128, 132,
            165, 172, 179,
            186, 188,
            213, 221
        };
        for (unsigned char i = 0; i < sz; ++i) {
            spots[double_letters[i]] = s;
        }
    }
}

// Sauvegarde du plateau dans un fichier
void Board::save(std::ostream& out) {
    for (unsigned char i = 0; i < 225; ++i) {
        if (i % 15 == 0) {
            out << std::endl;
        }
        if (spots[i].letter) {
            out << spots[i].letter;
        } else {
            out << ".";
        }
    }
}

// Chargement du plateau depuis un fichier
void Board::load(std::istream& in) {
    char c;
    for (unsigned char i = 0; i < 225; ++i) {
        in >> c;
        if (c != '.') {
            spots[i].letter = c;
        } else {
            spots[i].letter = 0;
        }
    }
}

// Accès aux cases
Spot Board::operator()(unsigned char l, unsigned char c) const {
    return spots[l * 15 + c];
}

Spot& Board::operator()(unsigned char l, unsigned char c) {
    return spots[l * 15 + c];
}

// Affichage classique du plateau
std::ostream& operator<<(std::ostream& out, const Board& b) {
    static const char* grey = "\033[1;90m";
    static const char* reset = "\033[1;0m";

    // Ligne supérieure
    for (int i = 0; i < 67; ++i) out << "#";
    out << std::endl;

    for (unsigned char i = 0; i < 15; ++i) {
        out << "#  ";
        for (unsigned char j = 0; j < 15; ++j) {
            bool above = (i > 0 && (b(i - 1, j).letter != 0));
            bool weak_above = (above || (i > 0 && j > 0 && b(i - 1, j - 1).letter != 0));
            bool below = (b(i, j).letter != 0);
            bool weak_below = (below || (j > 0 && b(i, j - 1).letter != 0));

            if (weak_above && weak_below) out << ":";
            else if (weak_above) out << "'";
            else if (weak_below) out << ".";
            else out << grey << "+" << reset;

            if (above && below) out << "---";
            else if (above || below) out << "---";
            else out << "   ";

            if (j == 14) {
                if (above && below) out << ":";
                else if (above) out << "'";
                else if (below) out << ".";
                else out << grey << "+" << reset;
            }
        }
        out << "  #" << std::endl << "#  ";
        for (unsigned char j = 0; j < 15; ++j) {
            bool left = (j > 0 && b(i, j - 1).letter != 0);
            bool right = b(i, j).letter != 0;

            if (left || right) out << "| ";
            else out << "  ";

            out << b(i, j) << " ";

            if (j == 14) {
                if (right) out << "|";
                else out << " ";
            }
        }
        out << "  #" << std::endl;
    }

    out << "#  ";
    for (unsigned char j = 0; j < 15; ++j) {
        bool above = (b(14, j).letter != 0);
        bool weak_above = (above || (j > 0 && b(14, j - 1).letter != 0));

        if (weak_above) out << "'";
        else out << grey << "+" << reset;

        if (above) out << "---";
        else out << "   ";

        if (j == 14) {
            if (above) out << ".";
            else out << grey << "+" << reset;
        }
    }
    out << "  #" << std::endl;
    for (int i = 0; i < 67; ++i) out << "#";
    out << std::endl;

    return out;
}
