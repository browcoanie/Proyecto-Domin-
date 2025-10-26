#ifndef UTILITY_H
#define UTILITY_H

#include "funciones.h"
#include <cstdlib> // para system()

// truco para limpiar la consola (cls en windows, clear en mac/linux)
inline void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


// ESTRUCTURAS PARA LA MESA (LISTA DOBLEMENTE ENLAZADA)

// el vagon de la mesa (DOBLE)
// necesita 'prev' y 'next' para poner por los dos lados
struct mesaNode {
    Ficha ficha;
    mesaNode* prev;
    mesaNode* next;
    mesaNode(const Ficha& f) : ficha(f), prev(nullptr), next(nullptr) {}
};

// la mesa, solo importan las 2 puntas
struct Mesa {
    mesaNode* left;
    mesaNode* right;
    Mesa() : left(nullptr), right(nullptr) {} // la crea vacia
};


// FUNCIONES PARA LA MESA

// inicia la mesa vacia
inline void initMesa(Mesa &mesa) {
    mesa.left = nullptr;
    mesa.right = nullptr;
}

// que numero hay a la izq?
inline int valueLeft(const Mesa &mesa) {
    if (mesa.left == nullptr) return -1; // -1 == mesa vacia
    return mesa.left->ficha.lado1;
}

// que numero hay a la der?
inline int valueRight(const Mesa &mesa) {
    if (mesa.right == nullptr) return -1; // -1 == mesa vacia
    return mesa.right->ficha.lado2;
}

// voltea la ficha [1|5] -> [5|1]
inline void swapSides(Ficha &ficha) {
    int temp = ficha.lado1;
    ficha.lado1 = ficha.lado2;
    ficha.lado2 = temp;
}

// revisa que la ficha sea real
inline bool isValidFicha(const Ficha& ficha) {
    return (ficha.lado1 >= 0 && ficha.lado1 <= 6) && 
           (ficha.lado2 >= 0 && ficha.lado2 <= 6);
}

// Poner a la izq
inline bool placeLeft(Mesa &mesa, Ficha ficha) {
    if (!isValidFicha(ficha)) return false;

    // si la mesa esta vacia
    if (mesa.left == nullptr) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false; 
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }

    // si la mesa NO esta vacia
    int valorIzquierdo = valueLeft(mesa);

    // lado2 encaja
    if (ficha.lado2 == valorIzquierdo) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode; // el nuevo es la punta
        return true;
    }

    // lado1 encaja, hay que voltearla
    if (ficha.lado1 == valorIzquierdo) {
        swapSides(ficha); // la volteamos
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode;
        return true;
    }

    // no encaja
    return false;
}

// Poner a la der (misma logica)
inline bool placeRight(Mesa &mesa, Ficha ficha) {
    if (!isValidFicha(ficha)) return false;

    // si la mesa esta vacia
    if (mesa.right == nullptr) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }

    // revisamos la punta derecha
    int valorDerecho = valueRight(mesa);

    // lado1 encaja
    if (ficha.lado1 == valorDerecho) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode->prev = mesa.right;
        mesa.right->next = newNode;
        mesa.right = newNode; // el nuevo es la punta
        return true;
    }

    // lado2 encaja, hay que voltearla
    if (ficha.lado2 == valorDerecho) {
        swapSides(ficha); // la volteamos
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode->prev = mesa.right;
        mesa.right->next = newNode;
        mesa.right = newNode;
        return true;
    }

    // no encaja
    return false;
}

// liberar memoria de la mesa
inline void clearMesa(Mesa &mesa) {
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        mesaNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    mesa.left = nullptr;
    mesa.right = nullptr;
}

#endif
