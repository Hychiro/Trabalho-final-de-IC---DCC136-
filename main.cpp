/*
Hiero Henrique Barcelos Costa 202065136A
Matheus Cardoso Faesy 202065065A
*/
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <chrono>
#include <thread>
#include <time.h>

using namespace std;

Graph *leituraInstancia(ifstream &input_file, ofstream &output_file)
{

    // Variáveis para auxiliar na criação dos nós no Grafo

    int order;
    int numeroRotulos;
    int numeroRotulosTotais;

    // Pegando a ordem do grafo
    input_file >> order >> numeroRotulosTotais;
    int matrixCorAresta[order][order];

    for (int k = 0; k < order; k++)
    {
        for (int j = 0; j < order; j++)
        {
            matrixCorAresta[k][j] = -1;
        }
    }
    // Criando objeto grafo
    Graph *graph = new Graph(order, numeroRotulosTotais);
    int coluna = 1;
    int linha = 0;
    // Leitura de arquivo
    while (input_file >> numeroRotulos)
    {

        if (coluna < order)
        {

            matrixCorAresta[linha][coluna] = numeroRotulos;

            coluna++;
        }
        else if (linha < order)
        {
            linha++;

            coluna = linha + 1;
            matrixCorAresta[linha][coluna] = numeroRotulos;
            coluna++;
        }
        else
        {
            break;
        }
    }

    for (int o = 0; o < order; o++)
    {
        for (int p = 0; p < order; p++)
        {

            if (matrixCorAresta[o][p] != -1 && matrixCorAresta[o][p] < numeroRotulosTotais)
            {
                graph->insertEdge(o, p, matrixCorAresta[o][p]);
            }
        }
    }

    return graph;
}

int menu()
{

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Algoritmo ILS" << endl;
    
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;
}

void selecionar(int selecao, Graph *graph, ofstream &output_file)
{

    switch (selecao)
    {
    // Algoritmo Guloso;
    case 1:
    {
        
        int clo = clock();
        srand((unsigned)time(NULL));
        Graph *melhorSol;
        float media;
        cout<< "Executando ILS 10x"<<endl;
        for (int i = 0; i < 10; i++)
        {
             
            
            output_file << "ILS "<<i<< endl;

            Graph *novoGraph = graph->ils(output_file);
           
            if (i == 0)
            {
                melhorSol = novoGraph;
                media = novoGraph->getNumRotulos();
            }
            else
            {   
                media = novoGraph->getNumRotulos() + media;

                if (novoGraph->getNumRotulos() < melhorSol->getNumRotulos())
                {
                    melhorSol = novoGraph;
                }
            }
            // novoGrafo = graph->guloso(output_file);

            // cout<<"pre refinamento"<<endl;

            // ag = novoGrafo->refinamento(output_file, graph);

            // cout<<"pos refinamento"<<endl;

            output_file << "Quantidade minima de rotulos: " << novoGraph->getNumRotulos() << endl;

           
            
        }
        output_file << "Melhor Quantidade minima de rotulos: " << melhorSol->getNumRotulos() << endl;
        output_file << "Media rotulos: " << media/10 << endl;
        output_file << "tempo de execucao: " << (clock() - clo) << " millisegundos" << endl;
        output_file<< "Melhor Solucao"<<endl;
        melhorSol->printGraph(output_file);
        break;
    }
    
    default:
    {
        cout << " Error!!! invalid option!!" << endl;
    }
    }
}

int mainMenu(ofstream &output_file, Graph *graph)
{

    int selecao = 1;

    while (selecao != 0)
    {
        //system("cls");
        selecao = menu();

        if (output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;
    }

    return 0;
}

int main(int argc, char const *argv[])
{

    // Verificação se todos os parâmetros do programa foram entrados
    if (argc != 3)
    {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file>" << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if (input_file_name.find("v") <= input_file_name.size())
    {
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    // Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph *graph;

    if (input_file.is_open())
    {
        
        graph = leituraInstancia(input_file, output_file);
    }
    else
        cout << "Unable to open " << argv[1];

    mainMenu(output_file, graph);

    // Fechando arquivo de entrada
    input_file.close();

    // Fechando arquivo de saída
    output_file.close();

    return 0;
}
