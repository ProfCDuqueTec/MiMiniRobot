/**
 * strategy.h
 * Kaizen Dōjō — Katas de Ataque (interfaz)
 *
 * Declaraciones de funciones de alto nivel (katas) para integrar
 * en el FSM principal del robot. Cada kata es una unidad de comportamiento.
 */

#ifndef STRATEGY_H
#define STRATEGY_H

#include <Arduino.h>

// Función principal que se llama en loop()
void execute_strategy();

// Interrupción / fin de combate (ya existe en el repo)
void end_match();

// ----- Katas (declaraciones) -----
// Básicas
bool kata_burst_frontal();        // Burst frontal: aceleración máxima y empuje
bool kata_sweep();                // Barrido lateral para localizar oponentes
bool kata_retreat_and_ram();      // Retroceso corto + embestida

// Intermedias
bool kata_flank_push();           // Flanqueo y empuje lateral
bool kata_spiral_drive();         // Aproximación en espiral y strike
bool kata_stop_wait_strike();     // Detener - esperar - contraataque

// Avanzadas
bool kata_torque_burst();         // Control adaptativo de par (empuje)
bool kata_fakeout_pounce();       // Finta y pounce
bool kata_adaptive_pressure();    // Presión adaptativa por telemetría
bool kata_cooperative_rim_lift(); // Experimental (si reglamento lo permite)

// ----- Utilidades/help -----
void strategy_init();             // Inicialización específica de estrategia (llamar en setup)
void set_kata_enabled(const char *kata_name, bool enabled); // toggle (opcional)
bool kata_is_enabled(const char *kata_name);

#endif // STRATEGY_H
