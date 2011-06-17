/**
 * Included objects are accessed using the name of the .thrift file as a
 * prefix. i.e. shared.SharedObject
 */
include "shared.thrift"

namespace cpp telemetry_rpc
namespace java telemetry_rpc

typedef byte int8
typedef i16 int16

enum ControlState {
    e_idle = 0x01, // Ожидание готовности датчиков
    e_ready_for_engine_start = 0x02, // Готов к запуску
    e_engine_start = 0x11, // Запуск двигателя
    e_engine_warming = 0x12, // Прогрев двигателя
    e_engine_check = 0x12, // Проверка двигателя
    e_ready_for_takeoff = 0x21, // Готов к взлёту
    e_rolling = 0x31, // Разбег
    e_breaks_ground = 0x32, // Подъём
    e_climbing = 0x41, // Первоначальный набор
    e_starting = 0x42, // Разгон
    e_flaps_off = 0x43, // Уборка закрылок
    e_on_route = 0x51, // Маршрут
    e_airfield_route = 0x61, // Аэродромный маршрут
    e_barometric_glidepath = 0x71, // Глиссада по барометрическому высотомеру
    e_ultrasonic_glidepath = 0x72, // Глиссада по УЗВ
    e_gliding = 0x73, // Выравнивание
    e_landing_run = 0x81, // Пробег
    e_emergency = 0xA1, // Выпуск парашюта
    e_init_manual = 0xC0, // Инициализация ручного режима
    e_command_radioline_manual = 0xC1, // Ручной режим: КРЛ
    e_futaba_manual = 0xC2 // Ручной режим: Futaba
}


struct Telemetry {
  1: int8 uav_addr,
  2: ControlState state,
  3: i32 roll,
  4: i32 pitch,
  5: i32 course,
  6: i32 kiass_speed,
  7: i32 baro_height
  8: i32 vertical_speed,
  9: i32 usonic_altim_height,
  10: i32 track_angle,
  11: i32 track_speed,
  12: double lattitude,
  13: double longitude,
  14: i32 snsHeight,
  15: i32 engine_rpm,
  16: i32 engine_temp1,
  17: i32 engine_temp2,
  18: i32 aileron,
  19: i32 elevator,
  20: i32 rudder,
  21: i32 throttle,
  22: i32 flaps,
  23: i32 active_fligt_task_point,
  24: i32 deviation,
  25: i32 next_point_distance
}

exception TelemetryTimeout {
  1: string what
}


service UavTelemetry extends shared.SharedService {

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   */

   void ping(),

   Telemetry get_telemetry() throws (1:TelemetryTimeout ex),

   void set_pitch(1:i32 p),
   void set_roll(1:i32 p),
   void set_elevator(1:i32 p),
   void set_rudder(1:i32 p),

   oneway void save()
}

