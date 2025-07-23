#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>     // Para usar funções como std::sort
#include <string>
#include "corretor.h"
#include "cliente.h"
#include "imovel.h"
#include "funcoes_auxiliares.h"

using namespace std;

// Variáveis globais para gerar ID únicos e que auto-incrementam
int nextCorretorId = 1;
int nextClienteId = 1;
int nextImovelId = 1;

int main() {
    // Declaração de vetores para armazenar os objetos de corretores, clientes e imóveis
    vector<Corretor> corretores;
    vector<Cliente> clientes;
    vector<Imovel> imoveis;

    // Leitura dos dados dos corretores
    int numCorretores;
    cin >> numCorretores;
    for (int i = 0; i < numCorretores; ++i) {
        string telefone;
        int avaliadorInt;
        double lat, lng;
        
        // Lê os dados formatados da entrada padrão
        cin >> telefone >> avaliadorInt >> lat >> lng;
        bool avaliador = (avaliadorInt == 1);
        
        string nome;
        // Lê o resto da linha para capturar o nome completo, ignorando espaços em branco no início
        getline(cin >> ws, nome); 
        
        // Cria um objeto Corretor com os dados lidos e um ID único, e o adiciona ao vetor de corretores
        corretores.push_back(Corretor(nextCorretorId++, telefone, avaliador, lat, lng, nome));
    }

    // Leitura dos Dados dos Clientes
    int numClientes;
    cin >> numClientes; // Lê o número total de clientes
    for (int i = 0; i < numClientes; ++i) {
        string telefone;
        string nome;
        
        cin >> telefone;
        getline(cin >> ws, nome);
        
        // Cria um objeto Cliente e o adiciona ao vetor de clientes
        clientes.push_back(Cliente(nextClienteId++, telefone, nome));
    }

    // Leitura dos Dados dos Imóveis
    int numImoveis;
    cin >> numImoveis; // Lê o número total de imóveis
    for (int i = 0; i < numImoveis; ++i) {
        string tipoStr; // Tipo do imóvel lido como string
        int proprietarioId;
        double lat, lng, preco;
        
        cin >> tipoStr >> proprietarioId >> lat >> lng >> preco;
        string endereco;
        getline(cin >> ws, endereco);
        
        // Converte a string do tipo de imóvel para o enum correspondente
        TipoImovel tipo = stringParaTipo(tipoStr); 
        
        // Cria um objeto Imovel e o adiciona ao vetor de imóveis
        imoveis.push_back(Imovel(nextImovelId++, tipo, proprietarioId, lat, lng, preco, endereco));
    }

    // Filtragem dos Corretores que são Avaliadores
    vector<Corretor> avaliadores; // Cria um novo vetor para armazenar apenas os corretores avaliadores
    for (const auto& corretor : corretores) {
        if (corretor.avaliador) { // Verifica se o corretor tem a flag 'avaliador' como true
            avaliadores.push_back(corretor); // Adiciona o corretor ao vetor de avaliadores
        }
    }

    // Se não houver corretores avaliadores o programa encerra
    if (avaliadores.empty()) {
        return 0;
    }

    // Preparação para a Distribuição dos Imóveis
    // Ordena os imóveis por ID
    sort(imoveis.begin(), imoveis.end(), [](const Imovel& a, const Imovel& b) {
        return a.id < b.id;
    });

    // Distribuição dos Imóveis entre os Avaliadores
    // Cria um vetor de vetores, onde cada vetor interno conterá os imóveis atribuídos a um avaliador
    vector<vector<Imovel>> imoveisPorAvaliador(avaliadores.size());
    for (size_t i = 0; i < imoveis.size(); ++i) {
        // Usa o operador módulo (%) para distribuir os imóveis de forma circular entre os avaliadores
        int indexAvaliador = i % avaliadores.size(); 
        imoveisPorAvaliador[indexAvaliador].push_back(imoveis[i]);
    }

    // Geração da Agenda de Visitas para cada Avaliador
    // Cria uma estrutura para armazenar a agenda final
    vector<vector<pair<int, Imovel>>> agenda(avaliadores.size());

    // Itera sobre cada avaliador para montar sua agenda individual
    for (size_t i = 0; i < avaliadores.size(); ++i) {
        // Se um avaliador não recebeu nenhum imóvel, pula para o próximo
        if (imoveisPorAvaliador[i].empty()) {
            continue;
        }

        // Define a localização inicial como a do próprio avaliador
        double currentLat = avaliadores[i].lat;
        double currentLng = avaliadores[i].lng;
        // Define o horário de início do trabalho como 9:00
        int currentTime = 9 * 60; 

        // Algoritmo do Vizinho Mais Próximo para otimizar a rota de visitas
        vector<bool> visited(imoveisPorAvaliador[i].size(), false); // Marca quais imóveis já foram visitados
        int numVisited = 0;
        int totalImoveis = imoveisPorAvaliador[i].size();
        vector<Imovel>& imoveisDoAvaliador = imoveisPorAvaliador[i]; // Referência para os imóveis do avaliador atual

        // Continua até que todos os imóveis atribuídos tenham sido agendados
        while (numVisited < totalImoveis) {
            int nextIndex = -1;     // Índice do próximo imóvel a ser visitado
            double minDist = -1;    // Distância mínima encontrada até agora

            // Encontra o imóvel não visitado mais próximo da localização atual
            for (int j = 0; j < totalImoveis; ++j) {
                if (!visited[j]) { // Considera apenas os imóveis ainda não visitados
                    // Calcula a distância haversine da localização atual até o imóvel
                    double dist = haversine(currentLat, currentLng, imoveisDoAvaliador[j].lat, imoveisDoAvaliador[j].lng);
                    // Se for o primeiro imóvel verificado ou se for mais perto que o mínimo atual, atualiza
                    if (nextIndex == -1 || dist < minDist) {
                        minDist = dist;
                        nextIndex = j;
                    }
                }
            }

            // Se um próximo imóvel foi encontrado
            if (nextIndex != -1) {
                visited[nextIndex] = true; // Marca o imóvel como visitado
                numVisited++;
                
                // Calcula o tempo de viagem
                int travelTime = static_cast<int>(minDist * 2.0); 
                currentTime += travelTime; // Adiciona o tempo de viagem ao horário atual
                
                // Adiciona o agendamento à agenda do avaliador
                agenda[i].push_back(make_pair(currentTime, imoveisDoAvaliador[nextIndex]));
                
                // Adiciona o tempo da avaliação
                currentTime += 60; 
                
                // Atualiza a localização atual para a do imóvel que acabou de ser visitado
                currentLat = imoveisDoAvaliador[nextIndex].lat;
                currentLng = imoveisDoAvaliador[nextIndex].lng;
            }
        }
    }

    // Impressão das Agendas
    // Itera sobre os avaliadores para imprimir suas agendas formatadas
    for (size_t i = 0; i < avaliadores.size(); ++i) {
        // Pula avaliadores sem agenda
        if (agenda[i].empty()) {
            continue;
        }
        cout << "Corretor " << avaliadores[i].id << endl; // Imprime o ID do corretor
        
        // Itera sobre cada agendamento na agenda do corretor
        for (const auto& agendamento : agenda[i]) {
            int minutos = agendamento.first;
            int horas = minutos / 60;
            int mins = minutos % 60;
            
            // Imprime o horário formatado e o ID do imóvel
            cout << setfill('0') << setw(2) << horas << ":" 
                      << setfill('0') << setw(2) << mins
                      << " Imóvel " << agendamento.second.id << endl;
        }
        
        // Lógica para imprimir uma linha em branco entre as agendas de diferentes corretores
        bool hasNext = false;
        // Verifica se existe algum corretor com uma agenda não vazia
        for (size_t j = i+1; j < avaliadores.size(); ++j) {
            if (!agenda[j].empty()) {
                hasNext = true;
                break;
            }
        }
        // Se houver imprime uma linha em branco para separação
        if (hasNext) {
            cout << endl;
        }
    }

    return 0;
}
