/*
Hiero Henrique Barcelos Costa 202065136A
Matheus Cardoso Faesy 202065065A
*/
/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <sstream>
#include <string>

using namespace std;

class Graph
{

    //Atributes
private:
    int order;
    int number_edges;
    bool conexGraph;
    Node *first_node;
    Node *last_node;
    std::list<int> rotulos;
    std::list<int>::iterator iteradorRotulos;
    int numeroRotulos;

public:
    int melhorInstancia;
    //Constructor
    Graph(int order, int numeroRotulos);
    //Destructor
    ~Graph();
    //Getters
    int getOrder();
    int getNumberEdges();
    Node *getFirstNode();
    Node *getLastNode();
    bool getConexGraph();
    int getNumRotulos();
    //Other methods
    void insertNode(int id);
    void insertEdge(int id, int target_id, int rotulo);
    void removeNode(int id);
    bool searchNode(int id);
    Node *getNode(int id);
    void insertAllNodes();
    bool verificaAresta(int id, int target_id);

    void funcAtualizaProbAlfas(float numeroDeRotulos, int numAlfa, float *alfa, float *probAlfa, float *mediaAlfa,ofstream &output_file);

    int funcEscolheAlfa(int numAlfa, float *alfa, float *probAlfa,ofstream &output_file);

    void aumentaQtdRotulos();
    int contaRotulo(int rotuloAnalisado);
    void adicionaRotulo(int rotuloAnalisado, Graph *grafoNovo, Graph *grafoOriginal);
    bool verificaConexo(Graph *grafo);

    Graph *guloso(ofstream &output_file);
    Graph *gulosoRandomizadoAux(float alfa, int instancia, int numIteracoes, ofstream &output_file);
    Graph *gulosoRandomizado(float alfa, ofstream &output_file);
    Graph *gulosoRandomizadoReativoAux(int instancia, ofstream &output_file);

    void fechoTransitivoDireto(ofstream &output_file, int id);

    void printGraph(ofstream &output_file);

    Graph *getVertexInduced(int *listIdNodes, int tam);
    Graph *agmPrim(ofstream &output_file);

    //methods phase1
    float greed();
    float greedRandom();
    float greedRactiveRandom();

private:
    //Auxiliar methods
};

#endif // GRAPH_H_INCLUDED