#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <cstdlib>
using namespace std;

// =====================================================
// ESTRUCTURAS DE DATOS
// =====================================================

// Las fichas de dominó tienen dos números
struct Ficha {
    int lado1;
    int lado2;
};

// Cada nodo guarda una ficha y apunta a la siguiente
struct NodoFicha {
    Ficha ficha;
    NodoFicha *siguiente;
};

// Cola - Para el pozo (FIFO: First In First Out)
struct colaFicha {
    NodoFicha *frente; // Donde sacamos las fichas
    NodoFicha *final;  // Donde metemos nuevas fichas
};

// Pila - Para las manos de los jugadores (LIFO: Last In First Out)
struct pilasFicha {
    NodoFicha *tope; // La ficha que está arriba del todo
};

struct Jugador {
    string nombre;
    pilasFicha mano; // Su mano de fichas (como una pila)
    int puntos;
    bool paso; // Para saber si pasó en este turno
};

struct Juego {
    Jugador jugadores[4]; // Máximo 4 jugadores
    int numJugadores;
    colaFicha pozo;       // Fichas sobrantes
    int turnoActual;      // Índice del jugador actual
};

// =====================================================
// FUNCIONES PARA COLA (POZO)
// =====================================================

// Inicializa una cola vacía
inline void crearCola(colaFicha &cola) {
    cola.frente = cola.final = nullptr;
}

// Verifica si la cola está vacía
inline bool colaVacia(colaFicha cola) {
    return cola.frente == nullptr;
}

// Mete una ficha al final de la cola
inline void insertarFichaCola(colaFicha &cola, Ficha ficha) {
    NodoFicha *nuevaFicha = new NodoFicha;
    nuevaFicha->ficha = ficha;
    nuevaFicha->siguiente = nullptr;

    if (colaVacia(cola)) {
        cola.frente = cola.final = nuevaFicha;
    } else {
        cola.final->siguiente = nuevaFicha;
        cola.final = nuevaFicha;
    }
}

// Saca una ficha del frente de la cola
inline Ficha sacarFichaCola(colaFicha &cola) {
    if (colaVacia(cola)) {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }

    NodoFicha *fichaEliminar = cola.frente;
    Ficha fichaRobada = fichaEliminar->ficha;

    cola.frente = cola.frente->siguiente;

    if (cola.frente == nullptr) {
        cola.final = nullptr;
    }

    delete fichaEliminar;
    return fichaRobada;
}

// Mira qué ficha está al frente sin sacarla
inline Ficha verFrenteCola(colaFicha cola) {
    if (colaVacia(cola)) {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }
    return cola.frente->ficha;
}

// Cuenta cuántas fichas hay en una cola
inline int contarFichasEnCola(colaFicha cola) {
    int total = 0;
    NodoFicha *actual = cola.frente;

    while (actual != nullptr) {
        total++;
        actual = actual->siguiente;
    }
    return total;
}

// Libera toda la memoria de una cola
inline void limpiarCola(colaFicha &cola) {
    while (!colaVacia(cola)) {
        sacarFichaCola(cola);
    }
}

// =====================================================
// FUNCIONES PARA PILA (MANO DE JUGADORES)
// =====================================================

// Inicializa una pila vacía
inline void crearPila(pilasFicha &pila) {
    pila.tope = nullptr;
}

// Verifica si la pila está vacía
inline bool pilaVacia(pilasFicha pila) {
    return pila.tope == nullptr;
}

// Inserta una ficha en el tope de la pila
inline void insertarFichaPila(pilasFicha &pila, Ficha ficha) {
    NodoFicha *nuevaFicha = new NodoFicha;
    nuevaFicha->ficha = ficha;
    nuevaFicha->siguiente = pila.tope;
    pila.tope = nuevaFicha;
}

// Saca la ficha de arriba de la pila
inline Ficha sacarFichaPila(pilasFicha &pila) {
    if (pilaVacia(pila)) {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }

    NodoFicha *fichaEliminar = pila.tope;
    Ficha fichaRobada = fichaEliminar->ficha;

    pila.tope = pila.tope->siguiente;

    delete fichaEliminar;

    return fichaRobada;
}

// Mira la ficha del tope sin sacarla
inline Ficha verTopePila(pilasFicha pila) {
    if (pilaVacia(pila)) {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }
    return pila.tope->ficha;
}

// Cuenta cuántas fichas hay en una pila
inline int contarFichasEnPila(pilasFicha pila) {
    int total = 0;
    NodoFicha *actual = pila.tope;

    while (actual != nullptr) {
        total++;
        actual = actual->siguiente;
    }
    return total;
}

// Busca una ficha específica en la pila y la saca
inline bool sacarFichaEspecifica(pilasFicha &pila, int lado1, int lado2, Ficha &fichaEncontrada) {
    if (pilaVacia(pila)) {
        return false;
    }

    pilasFicha pilaAux;
    crearPila(pilaAux);
    bool encontrada = false;

    while (!pilaVacia(pila) && !encontrada) {
        Ficha actual = sacarFichaPila(pila);

        if ((actual.lado1 == lado1 && actual.lado2 == lado2) ||
            (actual.lado1 == lado2 && actual.lado2 == lado1)) {
            fichaEncontrada = actual;
            encontrada = true;
        } else {
            insertarFichaPila(pilaAux, actual);
        }
    }

    while (!pilaVacia(pilaAux)) {
        Ficha temp = sacarFichaPila(pilaAux);
        insertarFichaPila(pila, temp);
    }

    return encontrada;
}

// Libera toda la memoria de una pila
inline void limpiarPila(pilasFicha &pila) {
    while (!pilaVacia(pila)) {
        sacarFichaPila(pila);
    }
}


// FUNCIONES PARA FICHAS


// Crea una ficha con los números que le pasamos
inline Ficha crearFicha(int num1, int num2) {
    Ficha nuevaFicha;
    nuevaFicha.lado1 = num1;
    nuevaFicha.lado2 = num2;
    return nuevaFicha;
}

// Genera las 28 fichas del dominó y las mete en la cola
inline void generarTodasLasFichas(colaFicha &cola) {
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            Ficha ficha = crearFicha(i, j);
            insertarFichaCola(cola, ficha);
        }
    }
}

// Mezcla las fichas del pozo aleatoriamente (algoritmo Fisher-Yates)
inline void mezclarPozo(colaFicha &cola) {
    int totalFichas = contarFichasEnCola(cola);

    if (totalFichas <= 1) return;

    Ficha *fichas = new Ficha[totalFichas];

    for (int i = 0; i < totalFichas; i++) {
        fichas[i] = sacarFichaCola(cola);
    }

    for (int i = totalFichas - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        Ficha temp = fichas[i];
        fichas[i] = fichas[j];
        fichas[j] = temp;
    }

    for (int i = 0; i < totalFichas; i++) {
        insertarFichaCola(cola, fichas[i]);
    }

    delete[] fichas;
}


// FUNCIONES DE LÓGICA DE JUEGO


// Busca la ficha doble más alta en la mano de un jugador
inline int buscarFichaDobleMasAlta(pilasFicha mano) {
    int fichaDobleMaxima = -1;
    NodoFicha *actual = mano.tope;

    while (actual != nullptr) {
        if (actual->ficha.lado1 == actual->ficha.lado2) {
            if (actual->ficha.lado1 > fichaDobleMaxima) {
                fichaDobleMaxima = actual->ficha.lado1;
            }
        }
        actual = actual->siguiente;
    }

    return fichaDobleMaxima;
}

// Determina qué jugador empieza (quien tiene la ficha doble más alta)
// Retorna el índice del jugador y guarda la ficha en fichaEncontrada
inline int determinarQuienEmpieza(Juego &juego, int &fichaEncontrada) {
    int jugadorInicial = 0;
    int fichaDobleMaximaGlobal = -1;

    for (int i = 0; i < juego.numJugadores; i++) {
        int fichaDobleJugador = buscarFichaDobleMasAlta(juego.jugadores[i].mano);
        if (fichaDobleJugador > fichaDobleMaximaGlobal) {
            fichaDobleMaximaGlobal = fichaDobleJugador;
            jugadorInicial = i;
        }
    }

    fichaEncontrada = fichaDobleMaximaGlobal;
    return jugadorInicial;
}

// Avanza al siguiente jugador
inline void siguienteTurno(Juego &juego) {
    juego.turnoActual = (juego.turnoActual + 1) % juego.numJugadores;
}

// Verifica si una ficha puede jugarse en la mesa
inline bool puedeJugarFicha(Ficha ficha, int valorIzq, int valorDer) {
    if (valorIzq == -1 && valorDer == -1) {
        return true;
    }

    return (ficha.lado1 == valorIzq || ficha.lado2 == valorIzq ||
            ficha.lado1 == valorDer || ficha.lado2 == valorDer);
}

// Verifica si un jugador tiene alguna jugada válida
inline bool tieneJugadaValida(pilasFicha mano, int valorIzq, int valorDer) {
    NodoFicha *actual = mano.tope;

    while (actual != nullptr) {
        if (puedeJugarFicha(actual->ficha, valorIzq, valorDer)) {
            return true;
        }
        actual = actual->siguiente;
    }

    return false;
}

// Roba una ficha del pozo
inline bool robarDelPozo(Juego &juego, int indiceJugador) {
    if (colaVacia(juego.pozo)) {
        return false;
    }

    Ficha fichaRobada = sacarFichaCola(juego.pozo);
    insertarFichaPila(juego.jugadores[indiceJugador].mano, fichaRobada);
    return true;
}

#endif 