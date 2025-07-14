#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include "corretor.h"
#include "cliente.h"
#include "imovel.h"
#include "funcoes_auxiliares.h"

int nextCorretorId = 1;
int nextClienteId = 1;
int nextImovelId = 1;

int main() {
    std::vector<Corretor> corretores;
    std::vector<Cliente> clientes;
    std::vector<Imovel> imoveis;

    int numCorretores;
    std::cin >> numCorretores;
    for (int i = 0; i < numCorretores; ++i) {
        std::string telefone;
        int avaliadorInt;
        double lat, lng;
        std::cin >> telefone >> avaliadorInt >> lat >> lng;
        bool avaliador = (avaliadorInt == 1);
        std::string nome;
        std::getline(std::cin >> std::ws, nome);
        corretores.push_back(Corretor(nextCorretorId++, telefone, avaliador, lat, lng, nome));
    }

    int numClientes;
    std::cin >> numClientes;
    for (int i = 0; i < numClientes; ++i) {
        std::string telefone;
        std::string nome;
        std::cin >> telefone;
        std::getline(std::cin >> std::ws, nome);
        clientes.push_back(Cliente(nextClienteId++, telefone, nome));
    }

    int numImoveis;
    std::cin >> numImoveis;
    for (int i = 0; i < numImoveis; ++i) {
        std::string tipoStr;
        int proprietarioId;
        double lat, lng, preco;
        std::cin >> tipoStr >> proprietarioId >> lat >> lng >> preco;
        std::string endereco;
        std::getline(std::cin >> std::ws, endereco);
        TipoImovel tipo = stringParaTipo(tipoStr);
        imoveis.push_back(Imovel(nextImovelId++, tipo, proprietarioId, lat, lng, preco, endereco));
    }

    std::vector<Corretor> avaliadores;
    for (const auto& corretor : corretores) {
        if (corretor.avaliador) {
            avaliadores.push_back(corretor);
        }
    }

    if (avaliadores.empty()) {
        return 0;
    }

    std::sort(imoveis.begin(), imoveis.end(), [](const Imovel& a, const Imovel& b) {
        return a.id < b.id;
    });

    std::vector<std::vector<Imovel>> imoveisPorAvaliador(avaliadores.size());
    for (int i = 0; i < imoveis.size(); ++i) {
        int indexAvaliador = i % avaliadores.size();
        imoveisPorAvaliador[indexAvaliador].push_back(imoveis[i]);
    }

    std::vector<std::vector<std::pair<int, Imovel>>> agenda(avaliadores.size());

    for (int i = 0; i < avaliadores.size(); ++i) {
        if (imoveisPorAvaliador[i].empty()) {
            continue;
        }

        double currentLat = avaliadores[i].lat;
        double currentLng = avaliadores[i].lng;
        int currentTime = 9 * 60;

        std::vector<bool> visited(imoveisPorAvaliador[i].size(), false);
        int numVisited = 0;
        int totalImoveis = imoveisPorAvaliador[i].size();
        std::vector<Imovel>& imoveisDoAvaliador = imoveisPorAvaliador[i];

        while (numVisited < totalImoveis) {
            int nextIndex = -1;
            double minDist = -1;

            for (int j = 0; j < totalImoveis; ++j) {
                if (!visited[j]) {
                    double dist = haversine(currentLat, currentLng, imoveisDoAvaliador[j].lat, imoveisDoAvaliador[j].lng);
                    if (nextIndex == -1 || dist < minDist) {
                        minDist = dist;
                        nextIndex = j;
                    }
                }
            }

            if (nextIndex != -1) {
                visited[nextIndex] = true;
                numVisited++;
                int travelTime = static_cast<int>(minDist * 2.0);
                currentTime += travelTime;
                agenda[i].push_back(std::make_pair(currentTime, imoveisDoAvaliador[nextIndex]));
                currentTime += 60;
                currentLat = imoveisDoAvaliador[nextIndex].lat;
                currentLng = imoveisDoAvaliador[nextIndex].lng;
            }
        }
    }

    for (int i = 0; i < avaliadores.size(); ++i) {
        if (agenda[i].empty()) {
            continue;
        }
        std::cout << "Corretor " << avaliadores[i].id << std::endl;
        for (const auto& agendamento : agenda[i]) {
            int minutos = agendamento.first;
            int horas = minutos / 60;
            int mins = minutos % 60;
            std::cout << std::setfill('0') << std::setw(2) << horas << ":" 
                      << std::setfill('0') << std::setw(2) << mins
                      << " Imóvel " << agendamento.second.id << std::endl;
        }
        bool hasNext = false;
        for (int j = i+1; j < avaliadores.size(); ++j) {
            if (!agenda[j].empty()) {
                hasNext = true;
                break;
            }
        }
        if (hasNext) {
            std::cout << std::endl;
        }
    }

    return 0;
}