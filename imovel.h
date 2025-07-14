#ifndef IMOVEL_H
#define IMOVEL_H

#include <string>

enum class TipoImovel { Casa, Apartamento, Terreno };

class Imovel {
public:
    int id;
    TipoImovel tipo;
    int proprietarioId;
    double lat;
    double lng;
    double preco;
    std::string endereco;

    Imovel(int id, TipoImovel tipo, int proprietarioId, double lat, double lng, double preco, const std::string& endereco);
};

#endif