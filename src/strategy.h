#pragma once
#include <Arduino.h>

void execute_strategy();
void end_match();

// Inicialización de estrategia
void strategy_init();

// Katas básicas
bool kata_burst_frontal();
bool kata_sweep();
bool kata_retreat_and_ram();

// Katas intermedias
bool kata_flank_push();
bool kata_spiral_drive();
bool kata_stop_wait_strike();

// Katas avanzadas
bool kata_torque_burst();
bool kata_fakeout_pounce();
bool kata_adaptive_pressure();
bool kata_cooperative_rim_lift();

// Control de activación
void set_kata_enabled(const char *kata_name, bool enabled);
bool kata_is_enabled(const char *kata_name);
