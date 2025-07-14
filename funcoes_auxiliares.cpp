#define _USE_MATH_DEFINES
#include "funcoes_auxiliares.h"
#include "imovel.h"
#include <cmath>
#include <string>

double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dlat = deg2rad(lat2 - lat1);
    double dlon = deg2rad(lon2 - lon1);
    double a = std::pow(std::sin(dlat/2), 2) +
               std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
               std::pow(std::sin(dlon/2), 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_R * c;
}

TipoImovel stringParaTipo(const std::string& tipoStr) {
    if (tipoStr == "Casa") {
        return TipoImovel::Casa;
    } else if (tipoStr == "Apartamento") {
        return TipoImovel::Apartamento;
    } else if (tipoStr == "Terreno") {
        return TipoImovel::Terreno;
    }
    return TipoImovel::Terreno;
}