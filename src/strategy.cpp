/**
 * strategy.cpp
 * Implementación de Katas de Ataque - Kaizen Dōjō
 *
 * NOTA: Este archivo contiene plantillas (stubs) con pseudocódigo y recomendaciones.
 *       Los participantes deben completar la lógica dentro de cada kata según su diseño.
 */

#include "strategy.h"
#include "motors.h"   // funciones: set_motors(), move_forward(), move_backwards(), stop_movement()
#include "sensors.h"  // funciones/vars: read_all_sensors(), sensor_left, sensor_middle, sensor_right, line_sensor_left, line_sensor_right

// Pines / dispositivos auxiliares ya definidos en main (LED, DIP, start module)
// Mantener la compatibilidad con end_match() existente.
#define PIN_RED_LED       13
#define PIN_DIP_SW_1      7
#define PIN_START_MODULE  3

// --------------------------------------------------
// Parámetros por kata (valores sugeridos - ajustar Kaizen)
static const uint16_t POWER_HIGH = 230;      // PWM alta
static const uint16_t POWER_MED  = 140;      // PWM media
static const uint16_t POWER_LOW  = 90;       // PWM baja

static const uint16_t BURST_DURATION_MS = 300;     // burst frontal
static const uint16_t RETREAT_MS        = 350;     // retroceso antes de ram
static const uint16_t SWEEP_ANGLE_MS    = 400;     // tiempo de giro en sweep (usado como referencia)
static const uint16_t WAIT_STRIKE_MS    = 250;     // Stop-Wait-Strike window
static const uint16_t SPIRAL_STEP_MS    = 200;     // paso de reducción de radio en espiral

// Mapa de enable/disable (simple, por nombre)
struct KataFlag { const char *name; bool enabled; };
static KataFlag kata_flags[] = {
  { "burst_frontal", true },
  { "sweep", true },
  { "retreat_and_ram", true },
  { "flank_push", true },
  { "spiral_drive", true },
  { "stop_wait_strike", true },
  { "torque_burst", false },
  { "fakeout_pounce", true },
  { "adaptive_pressure", false },
  { "cooperative_rim_lift", false },
  { NULL, false }
};

// --------------------------------------------------
// Helper utilities (pequeñas rutinas de uso común)
void strategy_init() {
  // inicializaciones específicas de estrategia, si aplica
  // ej: leer DIP switches, inicializar variables de telemetría, etc.
  pinMode(PIN_DIP_SW_1, INPUT_PULLUP);
}

void set_kata_enabled(const char *kata_name, bool enabled) {
  for (int i=0; kata_flags[i].name != NULL; ++i) {
    if (strcmp(kata_flags[i].name, kata_name) == 0) {
      kata_flags[i].enabled = enabled;
      return;
    }
  }
}

bool kata_is_enabled(const char *kata_name) {
  for (int i=0; kata_flags[i].name != NULL; ++i) {
    if (strcmp(kata_flags[i].name, kata_name) == 0) return kata_flags[i].enabled;
  }
  return false;
}

// --------------------------------------------------
// Integración con execute_strategy existing flow.
// Ajusta esta función para llamar a la kata que prefieras por heurística o DIP.
void execute_strategy() {
  // Lectura sensorial de base
  read_all_sensors();

  // Prioridad absoluta: si detecta línea => recuperar inmediatamente
  if (line_sensor_left || line_sensor_right) {
    int reached = line_sensor_left; // 1 si izquierdo
    set_motors(-100, -100); delay(400);
    set_motors(-255, -255);
    while (line_sensor_left || line_sensor_right) read_all_sensors();
    if (reached) set_motors(255, -255);
    else set_motors(-255, 255);
    delay(180);
    return;
  }

  // Modo simple por DIP switch: si DIP = LOW => búsqueda (demo), si HIGH => activar kat a selec
  bool dip = (digitalRead(PIN_DIP_SW_1) == LOW);

  // Ejemplo simple: priorizar burst frontal si sensor_middle
  if (sensor_middle && kata_is_enabled("burst_frontal")) {
    kata_burst_frontal();
    return;
  }

  // Ejemplo de secuencia por prioridad
  if (sensor_left && kata_is_enabled("flank_push")) {
    kata_flank_push();
    return;
  }
  if (sensor_right && kata_is_enabled("flank_push")) {
    kata_flank_push();
    return;
  }

  // Si nada detectado: iniciar sweep si habilitado
  if (dip && kata_is_enabled("sweep")) {
    kata_sweep();
    return;
  }

  // Fallback: marcha lenta o stop (mantener compatibilidad con tu archivo original)
  if (digitalRead(PIN_DIP_SW_1) == LOW) {
    set_motors(127, 127);
  } else {
    stop_movement();
  }
}

// Mantengo end_match como estaba (interrupción)
void end_match() {
  stop_movement();
  while (true) { digitalWrite(PIN_RED_LED, HIGH); }
}

// --------------------------------------------------
// Implementaciones (stubs) de los KATAS
// Cada función devuelve true si completó su rutina normal,
// false si abortó por condición de seguridad (ej: línea detectada)
// Implementador: complete la sección marcada con TODO
// --------------------------------------------------

// ---------- BÁSICAS ----------

/**
 * Burst Frontal
 * - Principio: aceleración máxima hacia adelante por BURST_DURATION_MS.
 * - Requisitos: sensor_middle == TRUE y no line detected.
 */
bool kata_burst_frontal() {
  // 1) Pre-checks
  read_all_sensors();
  if (line_sensor_left || line_sensor_right) return false; // seguridad

  // 2) Inicio del burst: máxima potencia
  unsigned long t0 = millis();
  while (millis() - t0 < BURST_DURATION_MS) {
    // TODO: ajustar potencia y monitorear sensores
    set_motors(POWER_HIGH, POWER_HIGH);
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) { stop_movement(); return false; }
    if (!sensor_middle) { /* si el sensor pierde contacto, podemos optar por parar o continuar según diseño */ }
    // NOTE: permite micro-ajustes: p. ej. reducir potencia tras primer contacto
  }

  // 3) Mantenimiento de empuje breve (opcional)
  // TODO: decidir si mantener potencia, reducirla o entrar en modo 'pressure' según sensores

  stop_movement();
  return true;
}

/**
 * Sweep (barrido lateral)
 * - Principio: girar para escanear; si detecta lateral => atacar.
 */
bool kata_sweep() {
  // Pseudocódigo:
  // - Gira suavemente a la izquierda durante SWEEP_ANGLE_MS leyendo sensores laterales.
  // - Si detecta, interrumpir y ejecutar un ataque dirigido.
  // - Repetir en sentido contrario si no hay detección.
  // TODO: Completar la ejecución de giro controlado (usar set_motors con valores opuestos)
  unsigned long t0 = millis();
  while (millis() - t0 < SWEEP_ANGLE_MS) {
    // giro a la izquierda lento (ejemplo: -POWER_LOW vs POWER_LOW)
    set_motors(-POWER_LOW, POWER_LOW);
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) { stop_movement(); return false; }
    if (sensor_left || sensor_right || sensor_middle) {
      // detectado: detener sweep y decidir ataque
      stop_movement();
      // Por simplicidad: invocar burst frontal si sensor_middle
      if (sensor_middle && kata_is_enabled("burst_frontal")) kata_burst_frontal();
      else {
        // si lateral detectado, acercar con flank_push si está habilitado
        if (kata_is_enabled("flank_push")) kata_flank_push();
      }
      return true;
    }
  }
  // giro inverso (opcional)
  t0 = millis();
  while (millis() - t0 < SWEEP_ANGLE_MS) {
    set_motors(POWER_LOW, -POWER_LOW);
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) { stop_movement(); return false; }
    if (sensor_left || sensor_right || sensor_middle) {
      stop_movement();
      if (sensor_middle && kata_is_enabled("burst_frontal")) kata_burst_frontal();
      else { if (kata_is_enabled("flank_push")) kata_flank_push(); }
      return true;
    }
  }
  stop_movement();
  return true;
}

/**
 * Retreat & Ram (retroceso + embestida)
 * - Principio: crear espacio retrocediendo y atacar con ángulo.
 */
bool kata_retreat_and_ram() {
  // 1) Retroceso
  unsigned long t0 = millis();
  while (millis() - t0 < RETREAT_MS) {
    set_motors(-POWER_MED, -POWER_MED);
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) { stop_movement(); return false; }
  }
  stop_movement();
  delay(50);

  // 2) Giro leve (ajustar signo según detección izquierda/derecha)
  if (sensor_left) {
    // girar a la izquierda para reentrar con ángulo
    set_motors(-POWER_LOW, POWER_LOW);
    delay(120); // ajustar
  } else if (sensor_right) {
    set_motors(POWER_LOW, -POWER_LOW);
    delay(120);
  }

  // 3) Embestida potente
  set_motors(POWER_HIGH, POWER_HIGH);
  delay(BURST_DURATION_MS);
  stop_movement();

  return true;
}

// ---------- INTERMEDIAS ----------

/**
 * Flank Push
 * - Principio: posicionamiento lateral + push
 */
bool kata_flank_push() {
  // - Girar hacia el flanco detectado
  if (sensor_left) {
    // aproximación por izquierda: girar y avanzar diagonal
    set_motors(-POWER_LOW, POWER_MED);
    delay(150);
    // avanzar y empujar
    set_motors(POWER_HIGH, POWER_HIGH);
    delay(300);
  } else if (sensor_right) {
    set_motors(POWER_MED, -POWER_LOW);
    delay(150);
    set_motors(POWER_HIGH, POWER_HIGH);
    delay(300);
  } else {
    // fallback: simple burst frontal
    return kata_burst_frontal();
  }

  stop_movement();
  return true;
}

/**
 * Spiral Drive
 * - Principio: circundar con radio decreciente y strike al final
 */
bool kata_spiral_drive() {
  // Pseudocontrol de espiral (simplificado):
  uint16_t radius_step = POWER_MED;
  for (int step = 0; step < 4; ++step) {
    // aplicar diferencial para girar con radio decreciente
    set_motors(radius_step, radius_step/2); // ejemplo: radio->reduce
    delay(SPIRAL_STEP_MS);
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) { stop_movement(); return false; }
    if (sensor_middle) {
      // atacar cuando el objetivo quede centrado
      set_motors(POWER_HIGH, POWER_HIGH);
      delay(BURST_DURATION_MS);
      stop_movement();
      return true;
    }
    radius_step = max((int)POWER_LOW, (int)radius_step - 20);
  }
  stop_movement();
  return true;
}

/**
 * Stop-Wait-Strike
 * - Principio: detenerse, esperar la reacción del rival y atacar
 */
bool kata_stop_wait_strike() {
  // 1) Detenerse
  stop_movement();
  unsigned long t0 = millis();
  bool saw_movement = false;

  // 2) Espera breve y monitor
  while (millis() - t0 < WAIT_STRIKE_MS) {
    read_all_sensors();
    if (line_sensor_left || line_sensor_right) return false;
    // detectar movimiento del rival: si sensor central pasa de 0->1 o valor lateral cambia
    if (sensor_middle || sensor_left || sensor_right) {
      saw_movement = true;
      break;
    }
  }

  // 3) Si hubo reacción, strike
  if (saw_movement) {
    set_motors(POWER_HIGH, POWER_HIGH);
    delay(BURST_DURATION_MS);
    stop_movement();
    return true;
  }

  // 4) Si no hubo reacción -> volver a búsqueda
  return true;
}

// ---------- AVANZADAS ----------

/**
 * Torque Burst (control adaptativo de par)
 * - Requiere telemetría (encoders o estimación)
 * - Implementador: si tienes encoders, usar RPM para control; si no, usar heurística por tiempo y reacción.
 */
bool kata_torque_burst() {
  // TODO:
  // Pseudocódigo:
  // - Iniciar empuje con POWER_HIGH.
  // - Medir indicador de patinaje (si dispones RPM o caida de velocidad).
  // - Si detectas patinaje en una rueda, reducir la potencia de esa rueda y aumentar en la otra.
  // - Mantener hasta éxito o timeout.
  // Nota: sin encoders, puedes estimar por tiempo o por cambio de detección de posición relativa del rival.
  return false; // por defecto: no implementada
}

/**
 * Fake-Out + Pounce
 * - Finta corta seguida por ataque rápido
 */
bool kata_fakeout_pounce() {
  // 1) Finta (micro-burst)
  set_motors(POWER_LOW, POWER_LOW);
  delay(70);
  stop_movement();
  delay(60);
  // 2) Monitor reacción
  read_all_sensors();
  if (sensor_middle || sensor_left || sensor_right) {
    // 3) Pounce
    set_motors(POWER_HIGH, POWER_HIGH);
    delay(250);
    stop_movement();
    return true;
  }
  return true;
}

/**
 * Adaptive Pressure
 * - Ajusta agresividad por telemetría de combate (match history)
 */
bool kata_adaptive_pressure() {
  // TODO:
  // - Mantener counters (push_count, success_count, total_time)
  // - Si success_count / push_count > threshold => incrementar POWER base
  // - Si battery_low or temp_high => reducir agresividad
  // Requiere infraestructura de telemetría (variables globales)
  return false;
}

/**
 * Cooperative Rim Lift (experimental; revisar reglamento)
 * - Acción mecánica de alto riesgo (solo implementar si reglamento lo permite)
 */
bool kata_cooperative_rim_lift() {
  // TODO: diseño mecanico y control fino; no implementar sin pruebas
  return false;
}


void execute_strategy() {
  read_all_sensors();
  if (sensor_middle) {
    kata_burst_frontal();
  } else {
    set_motors(120, 120);
  }
}
