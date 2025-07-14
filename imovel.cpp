#include "imovel.h"

Imovel::Imovel(int id, TipoImovel tipo, int proprietarioId, double lat, double lng, double preco, const std::string& endereco){
    this -> id = id;
    this -> tipo = tipo;
    this -> proprietarioId = proprietarioId;
    this -> lat = lat;
    this -> lng = lng;
    this -> preco = preco;
    this -> endereco = endereco;
}
