#ifndef TURNOS_H
#define TURNOS_H

#include "funciones.h"
#include "utility.h"
#include <iostream>
#include <string>

// FUNCIONES DE VISUALIZACIÓN

// muestra la mano numerada
void mostrarFichasPila(pilasFicha pila) {
    NodoFicha *actual = pila.tope;
    int contador = 1;
    
    if (actual == nullptr) {
        std::cout << "No hay fichas";
        return;
    }
    
    while (actual != nullptr) {
        std::cout << contador << ".[" << actual->ficha.lado1 << "|" << actual->ficha.lado2 << "] ";
        actual = actual->siguiente;
        contador++;
    }
}

// muestra la mesa y las puntas
void mostrarMesa(const Mesa &mesa) {
    if (mesa.left == nullptr) {
        std::cout << "[MESA VACÍA]" << std::endl;
        return;
    }
    
    std::cout << "MESA: ";
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        std::cout << "[" << current->ficha.lado1 << "|" << current->ficha.lado2 << "]";
        if (current->next != nullptr) std::cout << "-";
        current = current->next;
    }
    std::cout << "  (Izq:" << valueLeft(mesa) << " | Der:" << valueRight(mesa) << ")" << std::endl;
}


// FUNCIONES AUXILIARES DE JUEGO

// NUEVA FUNCION "ESPIA"
// Solo "ve" la ficha en la posición, PERO NO LA SACA.
// Esto arregla el bug de que la mano se desordenaba.
Ficha verFichaPorPosicion(pilasFicha mano, int posicion) {
    NodoFicha *actual = mano.tope;
    int contador = 1;
    
    // 1. buscar la ficha en esa posicion
    while (actual != nullptr && contador < posicion) {
        actual = actual->siguiente;
        contador++;
    }
    
    // si el num no existe
    if (actual == nullptr) {
        Ficha fichaInvalida = {-1, -1};
        return fichaInvalida;
    }
    
    // Devuelve la ficha, NO la saca
    return actual->ficha;
}


// FUNCIÓN PRINCIPAL DE TURNO
bool jugarTurno(Juego &juego, Mesa &mesa) {
    // 1. Limpiamos consola
    limpiarConsola(); 
    
    Jugador &jugadorActual = juego.jugadores[juego.turnoActual];
    
    // 2. Mostramos info
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "🎯 TURNO DE: " << jugadorActual.nombre << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    mostrarMesa(mesa);
    std::cout << "\n💼 Tu mano: ";
    mostrarFichasPila(jugadorActual.mano);
    std::cout << "\n🎲 Fichas en el pozo: " << contarFichasEnPila(juego.pozo) << std::endl;
    
    int valorIzq = valueLeft(mesa);
    int valorDer = valueRight(mesa);
    
    // 3. Revisamos si puede jugar
    bool tieneJugadas = tieneJugadaValida(jugadorActual.mano, valorIzq, valorDer);
    
    if (!tieneJugadas) {
        std::cout << "\n❌ No tienes jugadas válidas." << std::endl;
        
        // 4. Si no puede, roba
        if (!pilaVacia(juego.pozo)) {
            std::cout << "🎴 Robando del pozo..." << std::endl;
            robarDelPozo(juego, juego.turnoActual);
            
            std::cout << "💼 Tu mano actualizada: ";
            mostrarFichasPila(jugadorActual.mano);
            std::cout << std::endl;
            
            // si sigue sin poder, pasa
            tieneJugadas = tieneJugadaValida(jugadorActual.mano, valorIzq, valorDer);
            if (!tieneJugadas) {
                std::cout << "❌ Aún no puedes jugar. Pasas turno." << std::endl;
                jugadorActual.paso = true;
                return false; // Termina el turno
            }
            std::cout << "✅ ¡Ahora puedes jugar!" << std::endl;
        } else {
            // no hay pozo, pasa
            std::cout << "🚫 No hay fichas en el pozo. Pasas turno." << std::endl;
            jugadorActual.paso = true;
            return false; // Termina el turno
        }
    }
    
    // 5. BUCLE DE ACCION (para no perder turno si se equivoca)
    jugadorActual.paso = false; 
    
    while (true) {
        std::cout << "\n¿Qué quieres hacer?" << std::endl;
        std::cout << "1. Jugar una ficha" << std::endl;
        std::cout << "2. Robar del pozo (si hay)" << std::endl;
        
        int opcion;
        if (!(std::cin >> opcion)) { // Validacion de numero
             std::cout << "❌ Error: Debes ingresar un NÚMERO." << std::endl;
             std::cin.clear();
             std::cin.ignore(10000, '\n');
             continue; // repite
        }

        switch (opcion) {
            
            // --- CASO 1: Jugar ---
            case 1: {
                std::cout << "\n¿Qué ficha quieres jugar? (número): ";
                int numFicha;
                
                if (!(std::cin >> numFicha)) { // Validacion
                     std::cout << "❌ Error: Debes ingresar un NÚMERO." << std::endl;
                     std::cin.clear();
                     std::cin.ignore(10000, '\n');
                     continue; 
                }
                
                // 1. Usamos la función "espía" (VER). La mano NO se modifica.
                Ficha fichaAJugar = verFichaPorPosicion(jugadorActual.mano, numFicha);
                
                // si puso un num invalido
                if (fichaAJugar.lado1 == -1) {
                    std::cout << "❌ Ficha inválida. Introduce un número de tu mano." << std::endl;
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    continue; // no pierde turno
                }
                
                // si la ficha no se puede jugar
                if (!puedeJugarFicha(fichaAJugar, valorIzq, valorDer)) {
                    std::cout << "❌ Esa ficha no se puede jugar. Intenta con otra." << std::endl;
                    // Ya no hay que devolverla, porque nunca la sacamos
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    continue; // no pierde turno
                }
                
                // Preguntar lado
                char lado;
                bool colocada = false;
                
                if (valorIzq == -1 && valorDer == -1) {
                    // si es la primera ficha, se pone sola
                    colocada = placeLeft(mesa, fichaAJugar);
                } else {
                    std::cout << "¿Dónde quieres colocarla? (I)zquierda o (D)erecha: ";
                    std::cin >> lado;
                    
                    if (lado == 'I' || lado == 'i') {
                        colocada = placeLeft(mesa, fichaAJugar);
                    } else if (lado == 'D' || lado == 'd') {
                        colocada = placeRight(mesa, fichaAJugar);
                    } else {
                        std::cout << "❌ Lado inválido. Intenta de nuevo." << std::endl;
                        std::cout << "Presiona ENTER para continuar...";
                        std::cin.ignore(); std::cin.get();
                        continue; 
                    }

                    if (!colocada) {
                        std::cout << "❌ Esa ficha no va en ese lado. Intenta de nuevo." << std::endl;
                        std::cout << "Presiona ENTER para continuar...";
                        std::cin.ignore(); std::cin.get();
                        continue;
                    }
                }

                // --- EXITO! ---
                std::cout << "✅ ¡Ficha colocada!" << std::endl;
                
                // 2. AHORA SÍ, sacamos la ficha de la mano
                Ficha fichaDummy; // variable temporal, no importa
                sacarFichaEspecifica(jugadorActual.mano, fichaAJugar.lado1, fichaAJugar.lado2, fichaDummy);
                
                // 3. GANO!
                if (pilaVacia(jugadorActual.mano)) {
                    std::cout << "\n🎉🎉🎉 ¡" << jugadorActual.nombre << " GANÓ LA RONDA! 🎉🎉🎉" << std::endl;
                    return true; // Termina el turno (GANA)
                }
                
                return false; // Jugada valida, termina turno

            } // Fin Case 1
            
            // --- CASO 2: Robar ---
            case 2: {
                if (robarDelPozo(juego, juego.turnoActual)) {
                    std::cout << "✅ Robaste: [" << verTopePila(jugadorActual.mano).lado1 
                         << "|" << verTopePila(jugadorActual.mano).lado2 << "]" << std::endl;
                    
                    std::cout << "💼 Tu mano actualizada: ";
                    mostrarFichasPila(jugadorActual.mano);
                    std::cout << std::endl;
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    
                    // mostramos el estado actualizado
                    limpiarConsola();
                    std::cout << std::string(60, '=') << std::endl;
                    std::cout << "🎯 TURNO DE: " << jugadorActual.nombre << std::endl;
                    std::cout << std::string(60, '=') << std::endl;
                    mostrarMesa(mesa);
                    std::cout << "\n💼 Tu mano: ";
                    mostrarFichasPila(jugadorActual.mano);
                    std::cout << "\n🎲 Fichas en el pozo: " << contarFichasEnPila(juego.pozo) << std::endl;
                    
                    continue; // vuelve al menu (1. Jugar 2. Robar)

                } else {
                    std::cout << "❌ No hay fichas en el pozo." << std::endl;
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    continue; // vuelve al menu
                }
            } // Fin Case 2
            
            // --- Default: opcion invalida ---
            default: {
                std::cout << "❌ Opción inválida. Elige 1 o 2." << std::endl;
                std::cout << "Presiona ENTER para continuar...";
                std::cin.ignore(); std::cin.get();
                continue; // vuelve al menu
            }
        } // Fin Switch
    } // Fin while(true)
}


// FUNCIONES DE CONFIGURACIÓN DE JUEGO

// Pide los datos de los jugadores (¡CON VALIDACIÓN!)
void configurarJugadores(Juego &juego) {
    // Bucle para validar el numero de jugadores
    while (true) {
        std::cout << "¿Cuántos jugadores van a jugar? (2-4): ";
        if (!(std::cin >> juego.numJugadores)) {
            std::cout << "❌ Error: Debes ingresar un NÚMERO." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        if (juego.numJugadores >= 2 && juego.numJugadores <= 4) {
            break; // Numero valido, salimos
        } else {
            std::cout << "❌ Opción inválida. Tiene que ser entre 2 y 4 jugadores." << std::endl;
        }
    }

    // Pide nombres y crea sus pilas vacias
    for(int i = 0; i < juego.numJugadores; i++) {
        std::cout << "Nombre del jugador " << (i + 1) << ": ";
        std::cin >> juego.jugadores[i].nombre;
        crearPila(juego.jugadores[i].mano);
        juego.jugadores[i].puntos = 0;
        juego.jugadores[i].paso = false;
    }
}

// reparte 7 a cada uno
void repartirFichas(Juego &juego) {
    std::cout << "\n🎴 Repartiendo fichas..." << std::endl;
    for(int jugador = 0; jugador < juego.numJugadores; jugador++) {
        for(int ficha = 0; ficha < 7; ficha++) {
            Ficha fichaRobada = sacarFichaPila(juego.pozo);
            insertarFichaPila(juego.jugadores[jugador].mano, fichaRobada);
        }
    }
    std::cout << "✅ ¡Fichas repartidas! Cada jugador tiene 7 fichas." << std::endl;
}

// FUNCIONES DE CONTROL DE FLUJO

// Prepara la ronda
void iniciarRonda(Juego &juego, Mesa &mesa) {
    // 1. Limpiar todo
    limpiarPila(juego.pozo);
    clearMesa(mesa);
    
    for(int i = 0; i < juego.numJugadores; i++) {
        limpiarPila(juego.jugadores[i].mano);
        juego.jugadores[i].paso = false;
    }
    
    // 2. crear y barajar pozo
    generarTodasLasFichas(juego.pozo);
    mezclarPozo(juego.pozo);
    
    // 3. repartir
    repartirFichas(juego);
    
    // 4. JUGADA INICIAL AUTOMATICA
    int fichaDobleInicial = -1;
    int indiceJugadorInicial = determinarQuienEmpieza(juego, fichaDobleInicial);
    juego.turnoActual = indiceJugadorInicial; 
    
    std::cout << "\n🎮 El primer jugador es: " << juego.jugadores[indiceJugadorInicial].nombre << std::endl;

    if (fichaDobleInicial >= 0) {
        // tiene un doble, lo juega
        std::cout << "💎 ¡Empieza colocando la ficha doble [" << fichaDobleInicial << "|" << fichaDobleInicial << "]!" << std::endl;

        Ficha fichaInicio;
        // se la sacamos
        sacarFichaEspecifica(
            juego.jugadores[indiceJugadorInicial].mano,
            fichaDobleInicial, 
            fichaDobleInicial, 
            fichaInicio
        );
        // la ponemos
        placeLeft(mesa, fichaInicio);

        // IMPORTANTE: pasa el turno
        siguienteTurno(juego);
        std::cout << "Turno inicial completado. Ahora le toca a: " << juego.jugadores[juego.turnoActual].nombre << std::endl;

    } else {
        // no hay dobles, empieza normal
        std::cout << "(No hay fichas dobles, el juego empieza normal)" << std::endl;
        std::cout << "Le toca a: " << juego.jugadores[juego.turnoActual].nombre << std::endl;
    }
}

// bucle de una ronda (turno tras turno)
int jugarRonda(Juego &juego, Mesa &mesa) {
    bool hayGanador = false;
    int turnosSinJugar = 0; // para la "tranca"
    
    while (!hayGanador) {
        // 1. jugar turno
        hayGanador = jugarTurno(juego, mesa);
        
        // 2. si no gano...
        if (!hayGanador) {
            
            // revisamos si paso (para la tranca)
            if (juego.jugadores[juego.turnoActual].paso) {
                turnosSinJugar++; 
                
                // si todos pasaron == TRANCA
                if (turnosSinJugar >= juego.numJugadores) {
                    std::cout << "\n🚫 ¡JUEGO TRANCADO! Nadie puede jugar." << std::endl;
                    
                    std::cout << "\n📋 Puntos en mano de cada jugador:"<< std::endl;
                    for (int i = 0; i < juego.numJugadores; i++)
                    {
                        int puntosEnMano = calcularPuntosManos(juego.jugadores[i].mano);
                        std::cout << "  " << juego.jugadores[i].nombre << ": " << puntosEnMano << " puntos" << std::endl;
                    }

                    // buscamos ganador por puntos
                    int ganadorTranca = encontrarGanadorPorTranca(juego);
                    std::cout << "\n🏆 Gana por tener menos puntos: " << juego.jugadores[ganadorTranca].nombre << std::endl;

                    int puntosGanados = calcularPuntosRondas(juego, ganadorTranca);
                    juego.jugadores[ganadorTranca].puntos += puntosGanados;

                    std::cout << "💰 Puntos obtenidos: " << puntosGanados << std::endl;
                    return ganadorTranca; // se acabo la ronda
                }
            } else {
                turnosSinJugar = 0; // alguien jugo, resetea el contador
            }
            
            siguienteTurno(juego);
            
            // Pausa
            std::cout << "\nPresiona ENTER para continuar...";
            std::cin.ignore();
            std::cin.get();
        } else {
            // ¡Hubo un ganador! (se quedo sin fichas)
            int ganador = juego.turnoActual;
            int puntosGanados = calcularPuntosRondas(juego,ganador);
            juego.jugadores[ganador].puntos += puntosGanados;

            std::cout << "💰 Puntos obtenidos: " << puntosGanados << std::endl;
            return ganador; // se acabo la ronda
        }
    }
    return -1; // no deberia llegar aqui
}

// PARTIDA COMPLETA (SE REPITEN LAS RONDAS)

// imprime el resumen
void mostrarResumenRonda(Juego &juego, int numeroRonda, int ganadorRonda){
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "        RESUMEN DE LA RONDA " << numeroRonda << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "\n🏆 Ganador: " << juego.jugadores[ganadorRonda].nombre << std::endl;

    std::cout << "\n📋 Fichas restantes:" << std::endl;
    for (int i = 0; i < juego.numJugadores; i++)
    {
        int puntosEnMano = calcularPuntosManos(juego.jugadores[i].mano);
        if (i == ganadorRonda && puntosEnMano == 0)
        {
            std::cout << "  " << juego.jugadores[i].nombre << ": 0 puntos  (¡sin fichas!)" << std::endl;

        }else{
            std::cout << "  " << juego.jugadores[i].nombre << ": " << puntosEnMano << " puntos" << std::endl;
        }
    }
    mostrarPuntosJugadores(juego);
}

// revisa si alguien llego al puntaje
bool hayGanadorFinal(Juego &juego, int puntajeObjetivo){
    for (int i = 0; i < juego.numJugadores; i++)
    {
        if (juego.jugadores[i].puntos >= puntajeObjetivo)
        {
            return true;
        }
    }
    return false;
}

// Encuentra al jugador con MAS puntos
// OJO: bug arreglado aqui, empezar en 1 y comparar >
int encontrarGanadorFinal (Juego &juego){
    int ganador = 0;
    int maxPuntos = juego.jugadores[0].puntos;

    for (int i = 1; i < juego.numJugadores; i++) 
    {
        if (juego.jugadores[i].puntos > maxPuntos) {
            maxPuntos = juego.jugadores[i].puntos;
            ganador = i;
        }
    }
    return ganador;
}


// Bucle principal (ronda tras ronda)
void jugarPartidaCompleta(Juego &juego, Mesa &mesa, int puntajeObjetivo){
    int numeroRonda = 1;
    
    std::cout << "\n🎮 ¡COMIENZA LA PARTIDA!" << std::endl;
    std::cout << "🎯 Objetivo: " << puntajeObjetivo << " puntos" << std::endl;

    while (!hayGanadorFinal(juego, puntajeObjetivo))
    {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "           RONDA " << numeroRonda << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        iniciarRonda(juego,mesa);

        std::cout << "\nPRESIONA ENTER para empezar la ronda...";
        std::cin.ignore();
        std::cin.get();

        int ganadorRonda = jugarRonda(juego,mesa);
        mostrarResumenRonda(juego,numeroRonda, ganadorRonda);

        if (hayGanadorFinal(juego, puntajeObjetivo))
        {
            break; // se acabo
        }

        numeroRonda ++;
        std::cout << "\nPresiona ENTER para continuar a la siguiente ronda...";
        std::cin.ignore();
        std::cin.get();
    }

    // --- FIN DE PARTIDA ---
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "        🎊 FIN DE LA PARTIDA 🎊" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    int ganadorFinal = encontrarGanadorFinal(juego);

    std::cout << "\n🏆🏆🏆 ¡GANADOR FINAL: " << juego.jugadores[ganadorFinal].nombre << "! 🏆🏆🏆" << std::endl;
    std::cout << "🌟 Puntuación final: " << juego.jugadores[ganadorFinal].puntos << " puntos" << std::endl;

    // Mostrar clasificacion
    std::cout << "\n📊 CLASIFICACIÓN FINAL:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    // burbuja simple para ordenar la clasificacion
    for (int i = 0; i < juego.numJugadores - 1; i++) {
        for (int j = 0; j < juego.numJugadores - i - 1; j++) {
            if (juego.jugadores[j].puntos < juego.jugadores[j + 1].puntos) {
                Jugador temp = juego.jugadores[j];
                juego.jugadores[j] = juego.jugadores[j + 1];
                juego.jugadores[j + 1] = temp;
            }
        }
    }

    // imprimir con medallas
    for (int i = 0; i < juego.numJugadores; i++) {
        std::cout << (i + 1) << ". " << juego.jugadores[i].nombre << ": " 
             << juego.jugadores[i].puntos << " puntos";
        if (i == 0) std::cout << " 🥇";
        else if (i == 1) std::cout << " 🥈";
        else if (i == 2) std::cout << " 🥉";
        std::cout << std::endl;
    }
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "\nTotal de rondas jugadas: " << numeroRonda << std::endl;
}

#endif
