/*
Hiero Henrique Barcelos Costa 202065136A
Matheus Cardoso Faesy 202065065A
*/
#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Edge's methods
**************************************************************************************************/

// Constructor
Edge::Edge(int target_id){

    this->target_id = target_id;
    this->next_edge = NULL;
    this->rotulo = NULL;

}

// Destructor
Edge::~Edge(){

    if (this->next_edge != NULL){
        delete this->next_edge;
        this->next_edge = NULL;
    }

}

// Getters
int Edge::getTargetId(){

    return this->target_id;

}

Edge* Edge::getNextEdge(){

    return this->next_edge;

}

int Edge::getRotulo(){

    return this->rotulo;

}

// Setters
void Edge::setNextEdge(Edge* edge){

    this->next_edge = edge;

}

void Edge::setRotulo(int rotulo){

    this->rotulo = rotulo;

}
