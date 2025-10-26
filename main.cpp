#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "funciones.h"
#include "utility.h"
#include "turnos.h"

// Aqui si se puede usar 'using namespace std;' porque es un .cpp
using namespace std;

// la funcion limpiarConsola se movio a utility.h

int main() {
    // 1. crear estructuras
    Juego miJuego;
    Mesa mesa;
    
    // 2. inicializar
    crearPila(miJuego.pozo);
    initMesa(mesa);
    
    // semilla para el rand(). Si no, el juego seria igual cada vez
    srand(time(0)); 

    // 3. Bienvenida
    cout << "🎲 BIENVENIDO AL JUEGO DE DOMINÓ 🎲" << endl;
    cout << "===================================" << endl;
    
    // 4. Configurar jugadores (con validacion)
    configurarJugadores(miJuego);
    
    // 5. Configurar puntaje (ELIMINADO - AHORA SON 3 RONDAS)
    // ...
    
    limpiarConsola(); // limpiamos antes de empezar
    
    // 6. la funcion que hace todo (CAMBIO: sin puntaje)
    jugarPartidaCompleta(miJuego, mesa);
    
    // 7. IMPORTANTE: Limpiar memoria (delete)
    cout << "\n🧹 Limpiando memoria..." << endl;
    limpiarPila(miJuego.pozo);
    for(int i = 0; i < miJuego.numJugadores; i++) {
        limpiarPila(miJuego.jugadores[i].mano); // limpiar manos
    }
    clearMesa(mesa); // limpiar mesa
    
    cout << "\n¡Gracias por jugar! Hasta la próxima 👋" << endl;
    return 0;
}
