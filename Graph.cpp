/*
Hiero Henrique Barcelos Costa 202065136A
Matheus Cardoso Faesy 202065065A
*/
#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <string>
#include <bits/stdc++.h>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
 **************************************************************************************************/

// Constructor
Graph::Graph(int order, int numeroRotulos)
{
    this->order = order;
    this->conexGraph = false;
    this->first_node = NULL;
    this->last_node = NULL;
    this->number_edges = 0;
    this->numeroRotulos = 0;
    this->melhorInstancia = -1;
    insertAllNodes();
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != NULL)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

void Graph::printGraph(ofstream &output_file)
{
    Node *p = this->first_node;
    Edge *aux = p->getFirstEdge();

    output_file << "strict graph{" << endl;
    while (p != NULL)
    {

        aux = p->getFirstEdge();
        while (aux != NULL)
        {

            output_file << p->getId() << " -- " << aux->getTargetId() << "  [ label = " << aux->getRotulo() << " ]" << endl;
            aux = aux->getNextEdge();
        }
        p = p->getNextNode();
    }
    output_file << "}" << endl;
}

// Getters
int Graph::getNumRotulos()
{
    return this->numeroRotulos;
}
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{
    return this->number_edges / 2;
}
// Function that verifies if the graph is directed

Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

bool Graph::getConexGraph()
{
    return this->conexGraph;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/

// implementacao abaixo:

void Graph::insertNode(int id)
{
    // so cria o no e deixa ele no espaço
    Node *p = new Node(id);
    if (this->first_node == NULL)
    {
        this->first_node = p;
    }
    else
    {
        this->last_node->setNextNode(p);
    }
    this->last_node = p;
    this->last_node->setNextNode(NULL);
    this->order++;
    delete p;
}

void Graph::insertAllNodes()
{
    for (int i = 0; i < this->order; i++)
    {
        Node *p = new Node(i);
        if (this->first_node == NULL)
        {
            this->first_node = p;
        }
        else
        {
            this->last_node->setNextNode(p);
        }
        this->last_node = p;
    }
}

bool Graph::verificaAresta(int id, int target_id)
{
    Node *p = getNode(id);
    for (Edge *g = p->getFirstEdge(); g != NULL; g = g->getNextEdge())
    {

        if (g->getTargetId() == target_id)
        {
            return true;
        }
    }
    return false;
}

void Graph::insertEdge(int id, int target_id, int rotulo)
{
    // junta os nos entre si
    if (searchNode(id)) //<-- ta sendo direcionado prestar atenção nisso.
    {

        if (!verificaAresta(id, target_id))
        {
            Node *p = getNode(id);
            Node *sup = getNode(target_id);
            p->insertEdge(target_id, rotulo);
            sup->insertEdge(id, rotulo);
            this->number_edges += 1;

            bool found = (std::find(rotulos.begin(), rotulos.end(), rotulo) != rotulos.end());

            if (!(found))
            {
                this->rotulos.push_back(rotulo);
                aumentaQtdRotulos();
            }
        }
    }
}

void Graph::removeNode(int id) // pfv dps me ajudem a revisar esse removeNode
{
    Node *p;
    if (this->last_node != NULL)
    {
        if (this->first_node == this->last_node)
        {
            this->first_node = NULL;
            p = NULL;
        }
        else
        {
            Node *previousN = this->first_node;
            Node *nextN;
            Node *aux;
            Edge *sup;
            int count_edges = 0;

            p = getNode(id);

            while (p != previousN->getNextNode())
            {
                previousN->getNextNode();
            }
            nextN = p->getNextNode();

            previousN->setNextNode(nextN);
            if (previousN->getNextNode() == NULL)
            {
                last_node = previousN;
            }

            for (Node *i = first_node; i != NULL; i->getNextNode())
            {
                Edge *k = i->getFirstEdge();
                while (k != NULL)
                {
                    sup = k;
                    k->getNextEdge();

                    if (k->getTargetId() == p->getId())
                    {
                        sup->setNextEdge(k->getNextEdge());
                        k = NULL;
                        k = sup->getNextEdge();
                    }
                }
            }

            p->removeAllEdges();
            p = NULL;
        }
        order--;
    }
    else
        cout << "ERRO: o grafo esta vazio!" << endl;
}

bool Graph::searchNode(int id)
{
    // so verifica se exste o no ali ou nao

    for (Node *p = this->first_node; p != NULL; p = p->getNextNode())
    {
        if (p->getId() == id)
        {
            return true;
        }
    }
    return false;
}

Node *Graph::getNode(int id)
{
    // pega o no na lista de nos
    Node *p = first_node;
    if (searchNode(id))
    {
        while (p != NULL && p->getId() != id)
        {
            p = p->getNextNode();
        }
        return p;
    }
    return p;
}

///////////////////////GULOSOS///////////////////////FAMINTOS////////////////////

Graph *Graph::guloso(ofstream &output_file)
{

    //// função pra ver se é conexo
    if (this->verificaConexo(this)) // verifica se o grafo gerADOR É CONEXO
    {

        // INICIO FUNÇÃO ORDENA CANDIDATOS
        std::list<int> arestasPorRotulo; // fila que aramzena os rotulos em ordem decrecente
        std::list<int>::iterator iterador;

        int vetorarestasPorRotulo[this->numeroRotulos]; // vetor que faz relação posição=Rotulo e valor da posição=numero de arestas;
        int vetorAux[this->numeroRotulos];

        for (int i = 0; i < this->numeroRotulos; i++)
        {
            // repetir para o numero de rotulos
            vetorarestasPorRotulo[i] = contaRotulo(i); // para a posição i(que representa o Rótulo i) atribui o valor numero de arestas do Rotulo i
            vetorAux[i] = 0;
        }

        pair<int, int> maiorRotulo; // par que representa o maior rotulo atual <numero de arestas,Rotulo>
        maiorRotulo.first = 0;      // inicia o numero de arestas como 0
        maiorRotulo.second = -1;    // inicia o Rotulo como -1

        while (arestasPorRotulo.size() < this->numeroRotulos) // enquanto o tamanho da fila for menor que o numero de Rotulos
        {

            for (int j = 0; j < this->numeroRotulos; j++) // repetir para o numero de rotulos
            {
                if (maiorRotulo.first <= vetorarestasPorRotulo[j]) // se o numero de arestas do maior Rotulo Atual for menor ou igual do numero de arestas do rotulo a ser analisado (precisa ser menor ou IGUAL pq caso exista um Rotulo com 0 arestas iria bugar o codigo)
                {
                    maiorRotulo.first = vetorarestasPorRotulo[j]; // numero de arestas do maior rotulo igual ao numero de arestas do rotulo analisado
                    maiorRotulo.second = j;                       // maior Rotulo igual ao Rotulo analisado
                }
            }
            vetorAux[maiorRotulo.second] = maiorRotulo.first;
            arestasPorRotulo.push_back(maiorRotulo.second); // adiciona na parte de tras da fila o maior Rotulo da iteração

            vetorarestasPorRotulo[maiorRotulo.second] = -1; // modifica no vetor o RRotulo que ja foi adicionado para que ele não se repita

            maiorRotulo.first = 0;   // reinicia o numero de arestas como 0
            maiorRotulo.second = -1; // reinicia o Rotulo como -1
        }

        // FIM FUNÇÃO ORDENA CANDIDATOS

        Graph *q = new Graph(this->order, -1); //-1 pois começamos a contar os rotulos do 0, então -1 significa que não a rotulos

        while (!q->verificaConexo(q)) // Enquanto o grafo solução não for conexo repita
        {

            // Heuristica
            int rotuloAdicionado = arestasPorRotulo.front();

            q->adicionaRotulo(rotuloAdicionado, q, this); // chama a função q coloca o Rotulo no grafo e adiciona as arestas desse Rotulo

            arestasPorRotulo.pop_front(); // remove o topo da fila
        }

        return q;
    }
    else
    {

        return NULL;
    }
}

Graph *Graph::gulosoRandomizadoAux(float alfa, int instancia, int numIteracoes, ofstream &output_file)
{
    Graph *melhorSolucao = this;

    while (instancia < numIteracoes)
    {
        Graph *itAtual = this;

        cout << instancia << endl;

        Graph *aux = itAtual->gulosoRandomizado(alfa, output_file);

        if (melhorSolucao->getNumRotulos() > aux->getNumRotulos())
        {
            melhorSolucao = aux;
            melhorSolucao->melhorInstancia = instancia;
        }

        instancia++;
    }

    return melhorSolucao;
}

Graph *Graph::gulosoRandomizado(float alfa, ofstream &output_file)
{ // deve ser iniciado com Instancia 0 e Grafo melhorSolução==GrafoGerador

    // INICIO FUNÇÃO ORDENA CANDIDATOS
    std::list<int> arestasPorRotulo; // fila que aramzena os rotulos em ordem decrecente
    std::list<int>::iterator iterador;
    int vetorarestasPorRotulo[this->numeroRotulos]; // vetor que faz relação posição=Rotulo e valor da posição=numero de arestas;
    int vetorAux[this->numeroRotulos];
    for (int i = 0; i < this->numeroRotulos; i++)
    {                                              // repetir para o numero de rotulos
        vetorarestasPorRotulo[i] = contaRotulo(i); // para a posição i(que representa o Rótulo i) atribui o valor numero de arestas do Rotulo i
        vetorAux[i] = 0;
    }

    pair<int, int> maiorRotulo; // par que representa o maior rotulo atual <numero de arestas,Rotulo>
    maiorRotulo.first = 0;      // inicia o numero de arestas como 0
    maiorRotulo.second = -1;    // inicia o Rotulo como -1

    while (arestasPorRotulo.size() < this->numeroRotulos) // enquanto o tamanho da fila for menor que o numero de Rotulos
    {
        for (int i = 0; i < this->numeroRotulos; i++) // repetir para o numero de rotulos
        {
            if (maiorRotulo.first <= vetorarestasPorRotulo[i]) // se o numero de arestas do maior Rotulo Atual for menor ou igual do numero de arestas do rotulo a ser analisado (precisa ser menor ou IGUAL pq caso exista um Rotulo com 0 arestas iria bugar o codigo)
            {
                maiorRotulo.first = vetorarestasPorRotulo[i]; // numero de arestas do maior rotulo igual ao numero de arestas do rotulo analisado
                maiorRotulo.second = i;                       // maior Rotulo igual ao Rotulo analisado
            }
        }
        vetorAux[maiorRotulo.second] = maiorRotulo.first;
        arestasPorRotulo.push_back(maiorRotulo.second); // adiciona na parte de tras da fila o maior Rotulo da iteração

        vetorarestasPorRotulo[maiorRotulo.second] = -1; // modifica no vetor o RRotulo que ja foi adicionado para que ele não se repita

        maiorRotulo.first = 0;   // reinicia o numero de arestas como 0
        maiorRotulo.second = -1; // reinicia o Rotulo como -1
    }
    // FIM FUNÇÃO ORDENA CANDIDATOS

    Graph *q = new Graph(this->order, 0); //-1 pois começamos a contar os rotulos do 0, então -1 significa que não a rotulos
    while (!q->verificaConexo(q))         // Enquanto o grafo solução não for conexo repita
    {

        int numeroCadidatosPlausiveis = ceil(arestasPorRotulo.size() * alfa); // usa o alfa para saber quantas posições da fila temos que analisar
                                                                              // sorteia um numero entre 0 e numeroCadidatosPlausiveis
        int k = (rand() % numeroCadidatosPlausiveis);

        int contador = 0;
        for (iterador = arestasPorRotulo.begin(); iterador != arestasPorRotulo.end(); iterador++) // percorre todos os valores da lista
        {

            if (contador == k)
            {                                                 // quando o contador for igual ao numero sorteado
                int rotuloAdicionado = *iterador;             // rotulo adicionado recebe o valor que esta no iterador
                q->adicionaRotulo(rotuloAdicionado, q, this); // chama a função q coloca o Rotulo no grafo e adiciona as arestas desse Rotulo
                arestasPorRotulo.erase(iterador);
                break; // remove a posição sorteada da fila
            }
            contador++;
        }
    }
    // chama o método recursivamente
    // delete q;
    // arestasPorRotulo.~list();
    return q;
}

Graph *Graph::gulosoRandomizadoReativoAux(int instancia, ofstream &output_file)
{
    output_file << "Algoritmo Guloso Radomizado Reativo" << endl;

    int numAlfa; // encontra o num total de alfas
    cout << "Digite o Numero de alfas a serem adicionados:" << endl;
    cin >> numAlfa;
    float alfa[numAlfa];     // guarda todos os alfas em um mesmo vetor
    float probAlfa[numAlfa]; // guarda a probabilidade de todos os alfas, utilizar a posição do vetor alfa como referencia
    float mediaAlfa[numAlfa];
    int vezesUsada[numAlfa];
    for (int i = 0; i < numAlfa; i++)
    {
        cout << "Digite o alfa " << i << ":" << endl;
        cin >> alfa[i];

        probAlfa[i] = 0;
        mediaAlfa[i] = 0;
        vezesUsada[i] = 0;
    }
    int numdInteracoes;
    cout << "Digite o numero de Iteracoes:" << endl;
    cin >> numdInteracoes;

    int clo = clock();

    Graph *melhorSolucao = this;
    float qualidade[numAlfa];

    while (instancia < numdInteracoes)
    {

        Graph *itAtual = this;
        if (instancia < numAlfa)
        {

            Graph *aux = itAtual->gulosoRandomizado(alfa[instancia], output_file);

            vezesUsada[instancia] = vezesUsada[instancia] + 1;
            mediaAlfa[instancia] = (mediaAlfa[instancia] * (vezesUsada[instancia] - 1) + aux->getNumRotulos()) / vezesUsada[instancia];

            if (melhorSolucao->getNumRotulos() > aux->getNumRotulos())
            {
                melhorSolucao = aux;
                melhorSolucao->melhorInstancia = instancia;
            }
        }
        else
        {
            float somaDasQualidades = 0;
            for (int i = 0; i < numAlfa; i++)
            {
                qualidade[i] = (float)melhorSolucao->getNumRotulos() / mediaAlfa[i];
                somaDasQualidades = somaDasQualidades + qualidade[i];
            }
            for (int j = 0; j < numAlfa; j++)
            {
                probAlfa[j] = qualidade[j] / somaDasQualidades;
            }

            int alfaEscolhido = funcEscolheAlfa(numAlfa, alfa, probAlfa, output_file);

            Graph *aux = itAtual->gulosoRandomizado(alfa[alfaEscolhido], output_file);
            vezesUsada[alfaEscolhido] = vezesUsada[alfaEscolhido] + 1;
            mediaAlfa[alfaEscolhido] = (mediaAlfa[alfaEscolhido] * (vezesUsada[alfaEscolhido] - 1) + aux->getNumRotulos()) / vezesUsada[alfaEscolhido];

            if (melhorSolucao->getNumRotulos() > aux->getNumRotulos())
            {
                melhorSolucao = aux;
                melhorSolucao->melhorInstancia = instancia;
            }
        }
        instancia++;
    }
    /* for (int j = 0; j < numAlfa; j++)
         {
             output_file << "O alfa " << alfa[j] << " teve: " << endl;
             output_file << "Probabilidade final de " << probAlfa[j] << endl;
             output_file << "Media final de " << mediaAlfa[j] << endl;
             output_file << "Numero de particopações total igual a " << vezesUsada[j] << endl;
             output_file << endl;
         }*/

    output_file << "tempo de execucao: " << (clock() - clo) << " millisegundos" << endl;

    return melhorSolucao;
}

void Graph::aumentaQtdRotulos()
{
    this->numeroRotulos = 1 + this->numeroRotulos;
}

void Graph::adicionaRotulo(int rotuloAnalisado, Graph *grafoNovo, Graph *grafoOriginal)
{
    // adiciona o Rotulo no grafoNovo
    list<int> aux = grafoNovo->rotulos;
    aux.push_back(rotuloAnalisado);
    grafoNovo->rotulos = aux;
    grafoNovo->aumentaQtdRotulos();
    Node *it;
    Edge *it2;
    // percorre todas as arestas
    for (Node *it = grafoOriginal->getFirstNode(); it != NULL; it = it->getNextNode())
    {

        for (Edge *it2 = it->getFirstEdge(); it2 != NULL; it2 = it2->getNextEdge())
        {
            if (it2->getRotulo() == rotuloAnalisado) // se o rotulo da aresta for igual ao rotuloAnalisado
            {
                grafoNovo->insertEdge(it->getId(), it2->getTargetId(), rotuloAnalisado); // adiciona a aresta no grafoNovo
            }
        }
    }
    // aux.~list();
}

bool Graph::verificaConexo(Graph *grafo) // ta errado
{

    // com o id do vértice acha o vertice que deve ser analisado
    int idParametro = grafo->getFirstNode()->getId();
    // cria um vetor que marca quais vértices ja foram analisados
    bool visitados[grafo->getOrder()];
    // cria o vetor ligacao que verifica se tem caminho entre os vertices dizendo quais vértices se ligam de alguma maneira ao primeiro vertice
    bool ligacao[grafo->getOrder()];
    // cria uma fila que diz quais vertices ainda precisam ser analisados
    list<int> fila;
    // adiciona o vertice inicial nele
    fila.push_front(idParametro);

    for (int i = 0; i < grafo->getOrder(); i++)
    {
        visitados[i] = false;
        ligacao[i] = false;
    }

    // começa iteração (enquanto a fila não estiver vazia repita)
    while (!(fila.empty()))
    {
        // pega um vértice a ser analisado da fila
        int aux = fila.front();
        int IdAnalisado = aux;
        Node *V;
        V = getNode(fila.front());
        // exclui ele da fila
        fila.pop_front();
        // verifica se o vértice a ser analisado ja foi analisado. (se ele ja foi acaba essa iteração)
        Edge *it;
        if (visitados[IdAnalisado] == false)
        {
            // marca o vértice como visitado;
            visitados[IdAnalisado] = true;
            // marca ele como fazendo parrte da ligacao

            // tem caminho
            ligacao[IdAnalisado] = true;
            // adiciona todos os vértices adjacentes a ele na fila
            for (Edge *it = V->getFirstEdge(); it != NULL; it = it->getNextEdge())
            {
                int verticeAdjacente = it->getTargetId();
                fila.push_front(verticeAdjacente);
            }
        }
    }

    bool conexo = true;
    for (int j = 0; j < grafo->getOrder(); j++)
    {
        if (ligacao[j] == false)
        {

            conexo = false;
            grafo->conexGraph = false;
        }
        else
        {
            grafo->conexGraph = true;
        }
    }
    fila.~list();
    return conexo;
}

int Graph::contaRotulo(int rotuloAnalisado)
{

    int numeroArestas = 0;
    Node *it;
    Edge *it2;

    for (Node *it = this->getFirstNode(); it != NULL; it = it->getNextNode())
    {

        for (Edge *it2 = it->getFirstEdge(); it2 != NULL; it2 = it2->getNextEdge())
        {
            if (it2->getRotulo() == rotuloAnalisado)
            {
                numeroArestas++;
            }
        }
    }

    return numeroArestas / 2; // tem q voltar dividido por 2 pq todas arestas foram adicionados 2 vezes, logo serão somadas duas vezes
}

int Graph::funcEscolheAlfa(int numAlfa, float *alfa, float *probAlfa, ofstream &output_file)
{

    int vetValoresDaProbabilidade[numAlfa];
    int posicao = 0;
    for (int i = 0; i < numAlfa; i++)
    {
        vetValoresDaProbabilidade[i] = 0;
    }

    for (int j = 0; j < numAlfa; j++)
    {
        vetValoresDaProbabilidade[j] = posicao + probAlfa[j] * 100;
        posicao = vetValoresDaProbabilidade[j];
    }
    int valorProb = (int)(rand() % 100);
    int alfaRetornado = -1;
    for (int k = 0; k < numAlfa; k++)
    {
        if (valorProb <= vetValoresDaProbabilidade[k])
        {
            if (k == 0)
            {
                alfaRetornado = k;
            }
            else
            {
                if (valorProb > vetValoresDaProbabilidade[k - 1])
                {
                    alfaRetornado = k;
                }
            }
        }
    }
    return alfaRetornado;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////FECHO TRANSITIVO E BUSCAS///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////CAMINHAMENTO,ALGORITMOS E ORDENACAO TOPOLOGICA///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Graph *Graph::getVertexInduced(int *listIdNodes, int tam)
{
    Graph *subGrafo = new Graph(this->getOrder(), this->getNumRotulos());
    // para todo noh da lista faça

    for (int i = 0; i < tam; i++)
    {

        // incluir noh no subgrafo
        if (this->searchNode(listIdNodes[i]))
        {
            subGrafo->insertNode(listIdNodes[i]);
        }
    }
    Node *p;
    Node *orig;
    Edge *aux;
    bool verificaSeTem = false;
    // para todo noh do subgrafo,
    for (p = subGrafo->getFirstNode(); p != NULL; p = p->getNextNode())
    {
        orig = getNode(p->getId());

        // verificar as arestas no grafo original.
        for (aux = orig->getFirstEdge(); aux != NULL; aux = aux->getNextEdge())
        {

            // se a aresta do vertice pra onde ela aponta existir

            verificaSeTem = subGrafo->searchNode(aux->getTargetId());
            if (verificaSeTem)
            {
                // incluir a aresta no noh do subgrafo;
                subGrafo->insertEdge(p->getId(), aux->getTargetId(), aux->getRotulo());
            }
        }
    }
    // retorna subgrafo
    return subGrafo;
}

Graph *Graph::agmPrim(ofstream &output_file)
{
    int tamanho;
    tamanho = this->order;
    int *listaNos = new int[tamanho];
    for (int i = 0; i < tamanho; i++)
    {
        listaNos[i] = i;
    }
    Graph *grafoVI;
    grafoVI = this->getVertexInduced(listaNos, tamanho);
    Graph *grafoX = new Graph(this->getOrder(), 0);
    Node *p;
    // para todo noh da lista faça
    for (p = grafoVI->getFirstNode(); p != NULL; p = p->getNextNode())
    {
        grafoX->insertNode(p->getId());
    }
    bool adicionados[this->order]; // marca quais vértices ja possuem um caminho
    for (int j = 0; j < this->order; j++)
    {
        adicionados[j] = false;
    }
    adicionados[1] = true;
    std::list<int> vertices; // marca quais vértices ja possuem um caminho
    std::list<int>::iterator k;
    vertices.push_front(1); // adiciona o primeiro vértice na lista

    bool todosVerticesAdicionados = false;

    while (todosVerticesAdicionados == false) // repetir até ter um caminho para todos os vértices
    {
        int vertice1; // nó que vai armazenar o vértice de onde vai sair a aresta
        int vertice2; // nó que vai armazenar o vértice que a aresta vai chegar
        int rotulo;
        for (k = vertices.begin(); k != vertices.end(); k++) // percorre todos vértices da lista
        {
            Node *verticeAnalisado = grafoVI->getNode(*k);                                        // vertice analisado
            for (Edge *it = verticeAnalisado->getFirstEdge(); it != NULL; it = it->getNextEdge()) // percorre todas arestas do vertice analisado
            {
                int verticeAdjacente = it->getTargetId(); // pega o vértice alvo dessa aresta

                if (adicionados[verticeAdjacente] == false) // se o vértice alvo não foi adicionado
                {
                    rotulo = it->getRotulo();
                    vertice1 = verticeAnalisado->getId(); // lembra do nó que esta saindo essa aresta
                    vertice2 = verticeAdjacente;          // lembra do nó onde esta chegando essa arresta
                }
            }
        }
        // adiciona uma aresta entre o vértice 1 e 2 que possui custo = menorCusto
        grafoX->insertEdge(vertice1, vertice2, rotulo);

        vertices.push_front(vertice2); // adiciona o vertice 2 na lista vertices
        adicionados[vertice2] = true;  // marcar o vertice 2 como adicionado
        int contador = 0;
        for (int i = 0; i < (getOrder()); i++) // verificar se todos vértices ja foram adicionados se sim todosVerticesAdicionados=true
        {
            if (adicionados[i] == true)
            {
                contador++;
            }
        }
        if (contador == (this->order))
        {
            todosVerticesAdicionados = true;
        }
    }
    delete[] listaNos;

    return grafoX;
}

int Graph::sorteia(int tamanho)
{
    int valorSort = (int)(rand() % tamanho);
    return valorSort;
}

Graph *Graph::refinamento(ofstream &output_file, Graph *grafo)
{
    // acha o vértice inicial
    int menorOutDegree = 9999999;
    std::list<int> nodesIniciais;
    std::list<int>::iterator itrBegin, itrLast;
    for (Node *a = this->getFirstNode(); a != NULL; a = a->getNextNode())
    { // para todos os vértices
        // acha quais tem o menor Out Degree
        if (a->getDegree() < menorOutDegree)
        { // se o atual for menor

            // limpa a lista
            itrBegin = nodesIniciais.begin();
            itrLast = nodesIniciais.end();
            nodesIniciais.erase(itrBegin, itrLast);

            // atualiza o menor OutDegree
            menorOutDegree = a->getDegree();

            // Adiciona na lista
            nodesIniciais.push_back(a->getId());
        }
        else if (a->getDegree() == menorOutDegree)
        { // se o atual for igual ao menor

            // Adiciona na lista
            nodesIniciais.push_back(a->getId());
        }
        else
        { // se o atual for maior que o menor
        }
    }

    // sorteia entre os menores
    int numSorteado = sorteia(nodesIniciais.size());

    // pega na lista o Node sorteado
    itrBegin = nodesIniciais.begin();
    advance(itrBegin, numSorteado);
    Node *noInicial = getNode(*itrBegin);

    // limpa a lista
    itrBegin = nodesIniciais.begin();
    itrLast = nodesIniciais.end();
    nodesIniciais.clear();

    // criar um gravo vazio
    Graph *grafoX = new Graph(this->getOrder(), -1);

    bool adicionados[this->order]; // marca quais vértices ja possuem um caminho
    for (int j = 0; j < this->order; j++)
    {
        adicionados[j] = false; // inicia todos vértices como não visitados
    }
    bool todosVerticesAdicionados = false;

    // marca o vértice inicial como visitado
    adicionados[noInicial->getId()] = true;
    // cout << "no Inicial = " << noInicial->getId() << endl;

    // criar uma lista de de arestas plausiveis (toda vez q um nó for adicionado adicionar todas as arestas dele)
    std::list<int> arestasPlausiveis;
    std::list<int>::iterator itrAresta,itrAresta1,itrAresta2,itrAresta3;

    for (Edge *c = noInicial->getFirstEdge(); c != 0; c = c->getNextEdge())
    {
        arestasPlausiveis.push_back(c->getTargetId());
        arestasPlausiveis.push_back(c->getOrigemId());
        arestasPlausiveis.push_back(c->getRotulo());
    }

    // criar um vetor de rotulos ja utilizados
    // cout << "tamamnho rAdc: " << this->getNumRotulos() << endl;
    bool rAdc[grafo->getNumRotulos()]; // marca quais rotulos ja foram adicionados
    for (int j = 0; j < grafo->getNumRotulos(); j++)
    {
        rAdc[j] = false; // inicia todos rotulos como não adicionados
    }

    std::list<int> arestasASortear;
    std::list<int>::iterator itrBegin3, itrLast3, itr2;

    std::list<int> vizinhosASortear;
    std::list<int>::iterator itrBegin2, itrLast2;

    while (todosVerticesAdicionados == false) // repetir até ter um caminho para todos os vértices
    {
        bool houveADC = false;
        // passar pela lista de arestas
        for (itrAresta = arestasPlausiveis.begin(); itrAresta != arestasPlausiveis.end(); itrAresta++)
        {
            int targetID = *itrAresta;
            itrAresta++;
            int origemID = *itrAresta;
            itrAresta++;
            int rotulo = *itrAresta;
            if (adicionados[targetID] == true && rAdc[rotulo] == true)
            { // Se uma aresta chegar até um nó ja visitado, usando um rotulo ja utilizado adiciona ela no grafo
                houveADC = true;
                grafoX->insertEdge(origemID, targetID, rotulo);
                grafoX->insertEdge(targetID, origemID, rotulo);
                itrAresta3=itrAresta;
                itrAresta--;
                itrAresta2=itrAresta;
                itrAresta--;
                itrAresta1=itrAresta;
                break;
            }

            // Se uma aresta chegar até um nó não visitado e usar um rotulo ja utilizado adiciona a aresta e remove ela da lista
            if (adicionados[targetID] == false)
            {
                if (rAdc[rotulo] == true)
                {
                    // cout << "entrou aqui 1" << endl;
                    grafoX->insertEdge(origemID, targetID, rotulo);
                    grafoX->insertEdge(targetID, origemID, rotulo);
                    // arestasPlausiveis.erase(itrAresta);
                    houveADC = true;
                    adicionados[targetID] = true;
                    // cout << "marcando o vertice " << itrAresta->getTargetId() << " como visitado" << endl;
                    for (Edge *c = this->getNode(targetID)->getFirstEdge(); c != 0; c = c->getNextEdge())
                    {
                        arestasPlausiveis.push_back(c->getTargetId());
                        arestasPlausiveis.push_back(c->getOrigemId());
                        arestasPlausiveis.push_back(c->getRotulo());
                    }
                
                itrAresta3=itrAresta;
                itrAresta--;
                itrAresta2=itrAresta;
                itrAresta--;
                itrAresta1=itrAresta;
                break;
                }
                else
                {
                }
            }
            // Se uma aresta chegar até um nó não visitado e usar um rotulo não utilizado, não fazer nada
        }
        if(houveADC==true){
        arestasPlausiveis.erase(itrAresta3);
        arestasPlausiveis.erase(itrAresta2);
        arestasPlausiveis.erase(itrAresta1);
        }


        if (houveADC == false)
        {
            // cout << "nao houve adicao antes" << endl;
            // Se não Houve a adição de alguma aresta no passo anterior faça
            // achar o nó vizinho que menos arestas (do grafo novo) tocam
            // CRIAR UM VETOR nós vizinhos[this-order]
            int vizinhos[this->order];
            // se o nó ja tiver sido adicionado vizinhos[i]=9999999
            for (int i = 0; i < this->order; i++)
            {
                if (adicionados[i] == true)
                {
                    vizinhos[i] = 9999999;
                    // cout << "vizinho " << i << " ja adicionado" << endl;
                }
                else
                { // se o nó n tiver sido adicionado vizinhos[i]=0
                    // cout<<"vizinho "<<i<<" nao adicionado"<<endl;
                    vizinhos[i] = 0;
                }
            }
            // passar por todas arestas da lista, toda vez que passar por uma aresta somar 1 no vizinho[targetid] correspondente
            for (itrAresta = arestasPlausiveis.begin(); itrAresta != arestasPlausiveis.end(); itrAresta++)
            {
                int targetID = *itrAresta;
                itrAresta++;
                int origemID = *itrAresta;
                itrAresta++;
                int rotulo = *itrAresta;
                // cout << "vizinho " << itrAresta->getTargetId() << " possui " << vizinhos[itrAresta->getTargetId()] + 1 << " arestas" << endl;
                vizinhos[targetID]++;
            }

            int menorVizinho = 9999999;

            for (int i = 0; i < this->order; i++) // acha os menores vizinhos
            {
                if (vizinhos[i] < menorVizinho && vizinhos[i] != 0)
                { // se achar um vizinho menor
                    // cout << "vizinho menor encontrado = " << i << endl;
                    menorVizinho = vizinhos[i]; // atualiza o menor vizinho
                    // apaga a lista
                    itrBegin2 = vizinhosASortear.begin();
                    itrLast2 = vizinhosASortear.end();
                    vizinhosASortear.erase(itrBegin2, itrLast2);
                    // adiciona o novo menor valor na lista
                    vizinhosASortear.push_back(i);
                }
                else if (vizinhos[i] == menorVizinho && vizinhos[i] != 0)
                { // se achar um vizinho igual
                    // cout << "vizinho " << i << " igual encontrado" << endl;
                    vizinhosASortear.push_back(i);
                }
            }

            // cout << "Tamanho final da lista = " << vizinhosASortear.size() << endl;

            // sortear um dos vizinhos de menor arestas
            int numSorteado2 = sorteia(vizinhosASortear.size());
            int vizinhoEscolhido = -1;

            // pega na lista o Node sorteado
            itrBegin2 = vizinhosASortear.begin();
            advance(itrBegin2, numSorteado2);
            vizinhoEscolhido = *itrBegin2;

            // cout << "vizinho sorteado = " << vizinhoEscolhido << endl;

            itrBegin2 = vizinhosASortear.begin();
            itrLast2 = vizinhosASortear.end();
            vizinhosASortear.erase(itrBegin2, itrLast2);

            // sortear uma das arestas adicionar ela no grafo, removendo ela da lista e atualizando os rotulos
            for (itrAresta = arestasPlausiveis.begin(); itrAresta != arestasPlausiveis.end(); itrAresta++) // passo por todas arestas plausiveis
            {
                int targetID = *itrAresta;
                itrAresta++;
                int origemID = *itrAresta;
                itrAresta++;
                int rotulo = *itrAresta;
                if (targetID == vizinhoEscolhido) // se essa aresta toca no vértice sorteado
                {
                    // cout << "Aresta " << itrAresta->getOrigemId() << " -- " << itrAresta->getTargetId() << " adicionada para o sorteio" << endl;
                    arestasASortear.push_back(targetID);
                    arestasASortear.push_back(origemID);
                    arestasASortear.push_back(rotulo); // bota ela a ser sorteada;
                }
            }
            // cout << "tamanho final da lista: " << arestasASortear.size() << endl;

            // sortear uma das arestas

            int numSorteado3 = sorteia(arestasASortear.size() / 3);

            // pega na lista a aresta sorteada
            itrBegin3 = arestasASortear.begin();

            advance(itrBegin3, numSorteado3 * 3);

            int targetID = *itrBegin3;
            itrBegin3++;
            int origemID = *itrBegin3;
            itrBegin3++;
            int rotulo = *itrBegin3;

            // cout << "Rotulo " << rotulo << " Adicionado" << endl;
            // cout << "vertice adicionado = " << origem << " -- " << target << endl;

            adicionados[targetID] = true;
            // cout << "marcando o vertice " << target << " como visitado" << endl;

            rAdc[rotulo] = true;

            // adiciona a arresta sorteada
            grafoX->insertEdge(origemID, targetID, rotulo);
            grafoX->insertEdge(targetID, origemID, rotulo);

            for (Edge *c = this->getNode(targetID)->getFirstEdge(); c != 0; c = c->getNextEdge())
            {

                arestasPlausiveis.push_back(c->getTargetId());
                arestasPlausiveis.push_back(c->getOrigemId());
                arestasPlausiveis.push_back(c->getRotulo());
            }

            itrBegin3 = arestasASortear.begin();
            itrLast3 = arestasASortear.end();
            arestasASortear.erase(itrBegin3, itrLast3);
        }

        int contador = 0;
        for (int i = 0; i < (getOrder()); i++) // verificar se todos vértices ja foram adicionados se sim todosVerticesAdicionados=true
        {
            if (adicionados[i] == true)
            {
                contador++;
            }
        }
        if (contador == (this->order))
        {
            todosVerticesAdicionados = true;
        }
        // cout << "teste2" << endl;
    }
    arestasPlausiveis.clear();
    arestasASortear.clear();
    vizinhosASortear.clear();

    //cout << "retorna" << endl;
    return grafoX;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Graph *Graph::buscaLocalAleatoria(Graph *grafo, ofstream &output_file)
{

    Graph *novoGrafo = grafo;

    // cout << "teste 2.1" << endl;
    int rotuloAleatorio = novoGrafo->buscaRotuloAleatorio(this->numeroRotulos);

    // cout << "teste 2.2" << endl;
    adicionaRotulo(rotuloAleatorio, novoGrafo, this);
    //////
    // cout << "teste 2.3" << endl;
    Graph *solucao = novoGrafo->refinamento(output_file, this);
    // cout << "teste 2.4" << endl;

    if (grafo->getNumRotulos() >= solucao->getNumRotulos())
    {
        return solucao;
    }
    else
    {
        return grafo;
    }
    return NULL;
}
Graph *Graph::realizaPertubacao(Graph *grafo)
{
    for (int i = 0; i < (this->getNumRotulos()/25); i++)
    {
        // cout << "teste 2.1" << endl;
        // cout << "num rotulos 2.1: " << grafo->getNumRotulos() << endl;
        int rotuloAleatorio = grafo->buscaRotuloAleatorio(this->getNumRotulos());
        // cout << "num de rot " << rotuloAleatorio << endl;
        adicionaRotulo(rotuloAleatorio, grafo, this);
        // cout << "num rotulos 2.2: " << grafo->getNumRotulos() << endl;
    }
    return grafo;
}
int Graph::buscaRotuloAleatorio(int numRotTOTAL)
{
    bool found = true;
    int rotuloAleatorio;
    int i = 0;

    while (found)
    {
        rotuloAleatorio = rand() % numRotTOTAL;
        // cout << "rotuloAleatorio " << rotuloAleatorio << endl;
        found = (std::find(rotulos.begin(), rotulos.end(), rotuloAleatorio) != rotulos.end());
    }
    return rotuloAleatorio;
}

bool Graph::buscaRotulo(int rotulo)
{
    bool found = false;
    while (!found)
    {
        found = (std::find(rotulos.begin(), rotulos.end(), rotulo) != rotulos.end());
    }
    return true;
}

Graph *Graph::preencheGraph(Graph *grafo)
{
    Node *grafoNode;

    for (Node *p = this->getFirstNode(); p != NULL; p = p->getNextNode())
    {
        grafoNode = grafo->getNode(p->getId());
        for (Edge *q = p->getFirstEdge(); q != NULL; q = q->getNextEdge())
        {
            //
            if (grafo->buscaRotulo(q->getRotulo()))
            {
                if (!grafo->verificaAresta(grafoNode->getId(), q->getTargetId()))
                {
                    grafoNode->insertEdge(q->getTargetId(), q->getRotulo());
                }
            }
            //
        }
    }
    return grafo;
}
Graph *Graph::ils(ofstream &output_file)
{
    Graph *novoGraph;
    Graph *melhorSol;
    for (int i = 0; i < 30; i++)
    {
        novoGraph = gulosoRandomizado(0.1, output_file);
        if (i == 0)
        {
            melhorSol = novoGraph;
        }
        else
        {
            if (novoGraph->getNumRotulos() < melhorSol->getNumRotulos())
            {
                melhorSol = novoGraph;
            }
        }
    }

    int x = 100;
    int j = 0;

    // cout << "teste 1" << endl;
    for (int i = 0; i < x; i++)
    {
        j = 0;
        while (j < x)
        {

            // cout << "teste 2" << endl;
            novoGraph = this->buscaLocalAleatoria(novoGraph, output_file);

            if (melhorSol->getNumRotulos() > novoGraph->getNumRotulos())
            {
                // cout << "teste 3" << endl;
                melhorSol = novoGraph;
            }
            // novoGraph = this->preencheGraph(novoGraph);
            j++;
        }

        novoGraph = this->realizaPertubacao(novoGraph);
    }
    // cout << "teste 4" << endl;
    return melhorSol;
}