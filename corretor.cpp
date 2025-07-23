#include "corretor.h"

Corretor::Corretor(int id, const std::string& telefone, bool avaliador, double lat, double lng, const std::string& nome){
    this -> id = id;
    this -> telefone = telefone;
    this -> avaliador = avaliador;
    this -> lat = lat;
    this -> lng = lng;
    this -> nome = nome;
}

