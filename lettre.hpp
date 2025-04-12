#ifndef LETTRE_HPP
#define LETTRE_HPP

class Lettre {
private:
    char caractere;
    int points;

public:
    Lettre(char c, int p);
    char getCaractere() const;
    int getPoints() const;
};

#endif
