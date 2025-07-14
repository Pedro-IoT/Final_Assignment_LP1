#ifndef FUNCOES_AUXILIARES_H
#define FUNCOES_AUXILIARES_H

#include <cmath>
#include <string>
#include "imovel.h"

constexpr double EARTH_R = 6371.0;

inline double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double haversine(double lat1, double lon1, double lat2, double lon2);
TipoImovel stringParaTipo(const std::string& tipoStr);

#endif