#include <string>
using namespace std;

// Las fichas de dominó tienen dos números
struct Ficha
{
    int lado1;
    int lado2;
};

// Cada nodo guarda una ficha y apunta a la siguiente
struct NodoFicha
{
    Ficha ficha; // La ficha que se guarda
    NodoFicha *siguiente; // puntero al siguiente nodo
};

//Cola - Para el pozo
// El primero que entra el el primero que sale (FIFO)
struct colaFicha
{
    NodoFicha *frente; // Donde sacamos las fichas
    NodoFicha *final;   // Donde metemos nuevas fichas
};

// Pila - Para las manos de los jugadores (LIFO)
// El ultimo que entra es el primero que sale

struct pilasFicha
{
    NodoFicha *tope; // La ficha que esta arriba del todo
};

struct Jugador
{
    string nombre;
    pilasFicha mano; // Su mano de fichas (Como una pila)
    int puntos;
};

struct Juego
{
    Jugador jugadores[4];//Maximo 4 jugadores
    int numJugadores;
    colaFicha pozo; // fichas sobrantes 
};


// FUNCIONES PARA EL POZO

// Inicializa una cola vacia
void crearCola(colaFicha &cola){
    cola.frente = cola.final = nullptr;
}

// Verifica si la cola está vacía
bool colaVacia(colaFicha cola){
    return cola.frente == nullptr;
}


// Mete una ficha al final de la cola
void insertarFichaCola(colaFicha &cola, Ficha ficha) {
    NodoFicha *nuevaFicha = new NodoFicha;
    nuevaFicha-> ficha = ficha;
    nuevaFicha->siguiente = nullptr;

    // Si la cola está vacia, esta ficha es tanto el frente como el final
    if(colaVacia(cola)){
        cola.frente = cola.final = nuevaFicha;
    }
    else{
        // La añadimos al final y actulizamos el punteros
        cola.final -> siguiente  = nuevaFicha;
        cola.final = nuevaFicha;
    }
}


// Saca una ficha del frente de la cola
Ficha sacarFichaCola(colaFicha &cola){
    if(colaVacia(cola)){
        Ficha fichaVacia = {-1,-1}; // Ficha especial para indicar error
        return fichaVacia;
    }

    // Sacamos la ficha del frente
    NodoFicha *fichaEliminar = cola.frente;
    Ficha fichaRobada = fichaEliminar -> ficha;

    //Movemos el frente al siguiente
    cola.frente = cola.frente -> siguiente;

    // Si la cola quedo vacia, actualizamos al final tambien
    if (cola.frente == nullptr)
    {
        cola.final = nullptr;
    }

    // liberamos la memoria del nodo que sacamos
    delete fichaEliminar;
    return fichaRobada;
    
}

// Mira que ficha esta al frente sin sacarla
Ficha verFrenteCola(colaFicha cola){
    if (colaVacia(cola))
    {
        Ficha fichaVacia = {-1,-1};
        return fichaVacia;
    }
    return cola.frente -> ficha;
    
}

// FUNCIONES PARA EL MAZO

// Inicializa una pila vacía
void crearPila(pilasFicha &pila) {
    pila.tope = nullptr;
}

// Verifica si la pila está vacía

// Verifica si la pila está vacía
bool pilaVacia(pilasFicha pila) {
    return pila.tope == nullptr;
}


void insertarFichaPila(pilasFicha &pila, Ficha ficha){
    NodoFicha *nuevaFicha = new NodoFicha;
    nuevaFicha -> ficha = ficha;
    nuevaFicha -> siguiente = pila.tope; // La nueva apunta a la que estaba arriba
    pila.tope = nuevaFicha; // Ahora la nueva esta arriba

}

// Saca la ficha de arriba de la pila
Ficha sacarFichaPila(pilasFicha &pila) {
    if (pilaVacia(pila))
    {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }
    // Guardamos el nodo a eliminar
    NodoFicha *fichaEliminar = pila.tope;
    Ficha fichaRobada = fichaEliminar -> ficha;

    // Movemos el tope al siguiente
    pila.tope = pila.tope -> siguiente;

    // Liberamos la memoria
    delete fichaEliminar;

    return fichaRobada;
    
}

// FUNCIONES PARA EL DOMINÓ

// Crea una ficha con los numeros que le pasamos
Ficha crearFicha(int num1, int num2){
    Ficha nuevaFicha;
    nuevaFicha.lado1 = num1;
    nuevaFicha.lado2 = num2;
    return nuevaFicha;
}

// Liberar toda la memoria de una pila
void limpiarPila(pilasFicha &pila){
    while (!pilaVacia(pila))
    {
        sacarFichaPila(pila);
    }
    
}

// Liberar toda la memoria de una cola
void limpiarCola(colaFicha &cola){
    while (!colaVacia(cola))
    {
        sacarFichaCola(cola);
    }
    
}

// Generar los 28 fichas del domino y las mete en la cola

void generarTodasLasFichas(colaFicha &cola){
 // Generamos todas las combinaciones posibles
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            Ficha ficha = crearFicha(i, j);
            insertarFichaCola(cola, ficha);
        }
    }

}

// Cuenta cuántas fichas hay en una pila
int contarFichasEnPila(pilasFicha pila) {
    int total = 0;
    NodoFicha *actual = pila.tope;
    
    // Recorremos toda la pila contando
    while (actual != nullptr) {
        total++;
        actual = actual->siguiente;
    }
    return total;
}

// Cuenta cuántas fichas hay en una cola
int contarFichasEnCola(colaFicha cola) {
    int total = 0;
    NodoFicha *actual = cola.frente;
    
    // Recorremos toda la cola contando
    while (actual != nullptr) {
        total++;
        actual = actual->siguiente;
    }
    return total;
}

// Mezcla las fichas del pozo
void mezclarPozo(colaFicha &cola) {
   
    int totalFichas = contarFichasEnCola(cola);
    
    if (totalFichas <= 1) return; // No hay nada que mezclar
    
    Ficha *fichas = new Ficha[totalFichas];
    
    // Sacamos todas las fichas de la cola al arreglo
    for (int i = 0; i < totalFichas; i++) {
        fichas[i] = sacarFichaCola(cola);
    }
    
    for (int i = totalFichas - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // Intercambiamos fichas[i] con fichas[j]
        Ficha temp = fichas[i];
        fichas[i] = fichas[j];
        fichas[j] = temp;
    }
    
    // Volvemos a insertar las fichas mezcladas en la cola
    for (int i = 0; i < totalFichas; i++) {
        insertarFichaCola(cola, fichas[i]);
    }
    
    delete[] fichas;
}