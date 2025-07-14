#ifndef CORRETOR_H
#define CORRETOR_H

#include <string>

class Corretor {
public:
    int id;
    std::string telefone;
    bool avaliador;
    double lat;
    double lng;
    std::string nome;

    Corretor(int id, const std::string& telefone, bool avaliador, double lat, double lng, const std::string& nome);
};

#endif