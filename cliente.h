#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

class Cliente {
public:
    int id;
    std::string telefone;
    std::string nome;

    Cliente(int id, const std::string& telefone, const std::string& nome);
};

#endif