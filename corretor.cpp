#include "corretor.h"

Corretor::Corretor(int id, const std::string& telefone, bool avaliador, double lat, double lng, const std::string& nome)
    : id(id), telefone(telefone), avaliador(avaliador), lat(lat), lng(lng), nome(nome) {}