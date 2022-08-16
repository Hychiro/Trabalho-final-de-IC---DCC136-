/*
Hiero Henrique Barcelos Costa 202065136A
Matheus Cardoso Faesy 202065065A
*/
#include "Node.h"
#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Node's methods
**************************************************************************************************/

// Constructor
Node::Node(int id){

    this->id = id;
    this->degree = 0;
    this->weight = 0;
    this->first_edge = NULL;
    this->last_edge = NULL;
    this->next_node = NULL;

};

// Destructor
Node::~Node(){

    Edge* next_edge = this->first_edge;

    while(next_edge != NULL){

        Edge* aux_edge = next_edge->getNextEdge();
        delete next_edge;
        next_edge = aux_edge;

    }

};

// Getters
Edge* Node::getFirstEdge(){

    return this->first_edge;

}

Edge* Node::getLastEdge(){

    return this->last_edge;

}

int Node::getId(){

    return this->id;

}

int Node::getDegree(){

    return this->degree;

}


float Node::getWeight(){

    return this->weight;

}

Node* Node::getNextNode(){

    return this->next_node;

}

// Setters

	void Node::setNextNode(Node* next_node){
	
	    this->next_node = next_node;
	
	}



// Other methods
void Node::insertEdge(int target_id, int rotulo){
    // Verifies whether there are at least one edge in the node
    if(this->first_edge != NULL){
        // Allocating the new edge and keeping the integrity of the edge list
        Edge* edge = new Edge(target_id, this->getId());
        edge->setRotulo(rotulo);
        this->last_edge->setNextEdge(edge);
        this->last_edge = edge;
        this->degree=this->degree+1;

    }
    else{
         // Allocating the new edge and keeping the integrity of the edge list
        this->first_edge = new Edge(target_id, this->getId());
        this->first_edge->setRotulo(rotulo);
        this->last_edge = this->first_edge;
        this->degree=this->degree+1;


    }

}

void Node::removeAllEdges(){
    // Verifies whether there are at least one edge in the node
    if(this->first_edge != NULL){

        Edge* next = NULL;
        Edge* aux = this->first_edge;
        // Removing all edges of the node
        while(aux != NULL){

            next = aux->getNextEdge();
            delete aux;
            aux = next;
        }

    }
    this->degree=0;

    this->first_edge = this->last_edge = NULL;

}

int Node::removeEdge(int id,Node* target_node){
    // Verifies whether the edge to remove is in the node
    if(this->searchEdge(id)){

        Edge* aux = this->first_edge;
        Edge* previous = NULL;
        // Searching for the edge to be removed
        while(aux->getTargetId() != id){

            previous = aux;
            aux = aux->getNextEdge();

        }
        // Keeping the integrity of the edge list
        if(previous != NULL)
            previous->setNextEdge(aux->getNextEdge());

        else
            this->first_edge = aux->getNextEdge();

        if(aux == this->last_edge)
            this->last_edge = previous;

        if(aux->getNextEdge() == this->last_edge)
            this->last_edge = aux->getNextEdge();

        delete aux;
        // Verifies whether the graph is directed
        //if(directed)
        //    this->decrementOutDegree();

        //else{

        //    this->decrementInDegree();
        //    target_node->decrementInDegree();

        //}
        this->decrementDegree();
        //getNode(target_node->getTargetId())->decrementInDegree();

        return 1;

    }

    return 0;

}

bool Node::searchEdge(int target_id){
     // Verifies whether there are at least one edge in the node
    if(this->first_edge != NULL){
        // Searching for a specific edge of target id equal to target id
        for(Edge* aux = this->first_edge; aux != NULL; aux = aux->getNextEdge())
            if(aux->getTargetId() == target_id)
                return true;

    }

    return false;

}

void Node::incrementDegree(){

    this->degree++;

}

void Node::decrementDegree(){

    this->degree--;

}


Edge* Node::hasEdgeBetween(int target_id)
{

    for(Edge *auxEdge = this->first_edge; auxEdge != NULL; auxEdge = auxEdge->getNextEdge())
    {
        if(auxEdge->getTargetId() == target_id)
            return auxEdge;
    }
    return NULL;
}
