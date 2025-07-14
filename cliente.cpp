#include "cliente.h"

Cliente::Cliente(int id, const std::string& telefone, const std::string& nome){
    this -> id = id;
    this -> telefone = telefone;
    this -> nome = nome;
}
