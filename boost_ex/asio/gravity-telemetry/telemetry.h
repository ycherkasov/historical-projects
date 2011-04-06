/* 
 * File:   telemetry.h
 * Author: straight
 *
 * Created on 24 Март 2011 г., 11:23
 */

#ifndef TELEMETRY_H
#define	TELEMETRY_H
#include <iostream>
#include <string>

// include headers that implement a archive in simple text format
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "simple_log_archive.h"


using namespace boost::archive;

enum {
    e_IdentityRequest = 0x00,
    e_IdentityResponse = 0x01,

    e_TelemetryRequest = 0x10,
    e_TelemetryResponse = 0x11
};

struct TelemetryRequest {
    static const int telemetryRequestPacketSize = 4;
    static const uint8_t e_typeID = e_TelemetryRequest;

    TelemetryRequest()
    : addr(0x01)
    , packet_id(e_TelemetryRequest)
    , size(telemetryRequestPacketSize) { }

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(addr);
        ar & BOOST_SERIALIZATION_NVP(packet_id);
        ar & BOOST_SERIALIZATION_NVP(size);
        ar & BOOST_SERIALIZATION_NVP(crc8);
    }
    /** @brief UAV address (should be same with request) */
    uint8_t addr;

    /** @brief 0x11 */
    uint8_t packet_id;

    /** @brief 0x11 */
    uint8_t size;

    uint8_t crc8;

    void print_debug_info(std::ostream& where) {
        simple_log_archive log(where);
        log << *this;
        where << std::endl;
    }

};

struct TelemetryResponse {
    static const int telemetryResponsePacketSize = 49;
    static const uint8_t e_typeID = e_TelemetryResponse;

    TelemetryResponse()
    : addr(0x01)
    , packet_id(e_TelemetryResponse)
    , size(telemetryResponsePacketSize)
    , currentState()
    , roll()
    , pitch()
    , course()
    , kiasSpeed()
    , baroHeight()
    , verticalSpeed()
    , usonicAltmHeight()
    , trackAngle()
    , trackSpeed()
    , lattitude()
    , longitude()
    , snsHeight()
    , engineRPM()
    , engineTemperature1()
    , engineTemperature2()
    , aileronChannel()
    , elevatorChannel()
    , rudderChannel()
    , throttleChannel()
    , flapsChannel()
    , activeFPointId()
    , fdDeviation()
    , nextPointDistance() { }

    enum E_ControlState {
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
    };

    /** @brief UAV address (should be same with request) */
    uint8_t addr;

    /** @brief 0x11 */
    uint8_t packet_id;

    /** @brief 0x11 */
    uint8_t size;

    /** @brief Current UAV state
     * see chapter 3.1 Table 6 UAV protocol specification */
    uint8_t currentState;

    /** @brief Heel * 0.01 degree */
    int16_t roll;

    /** @brief pitch * 0.01 degree */
    int16_t pitch;

    /** @brief Course relative to real North * 0.01 degree */
    uint16_t course;

    /** @brief KIAS (Knots Indicated Air Speed) * 0.01 m/s */
    uint16_t kiasSpeed;

    /** @brief Barometric height * 1 m */
    int16_t baroHeight;

    /** @brief Rate of descent (vertical speed) * 0.01 m/s */
    int16_t verticalSpeed;

    /** @brief Ultrasonic altimeter heoght * 0.01 m */
    int16_t usonicAltmHeight;

    /** @brief True course arrow relative to North * 0.01 degree */
    uint16_t trackAngle;

    /** @brief True course speed * 0.01 m/s */
    uint16_t trackSpeed;

    /** @brief Latitude * degrees */
    float lattitude;

    /** @brief Longitude * degrees */
    float longitude;

    /** @brief Height by SNS * 1 m */
    int16_t snsHeight;

    /** @brief Engine revolutions per minute */
    uint16_t engineRPM;

    /** @brief Engines 1&2 temperature * 1 celsius */
    int16_t engineTemperature1;
    int16_t engineTemperature2;

    /** @brief Aileron channel (-127...+127, negative is left heel) */
    int8_t aileronChannel;

    /** @brief Elevator (height) channel (-127...+127, negative is diving) */
    int8_t elevatorChannel;

    /** @brief Rudder (direction) channel (-127...+127, negative is left turn) */
    int8_t rudderChannel;

    /** @brief Throttle (-127 = OFF, 0 = 0%, 127 = 100%) */
    int8_t throttleChannel;

    /** @brief Wing flaps (0 = flight, 1 = flight up, 2 = landing ) */
    int8_t flapsChannel;

    /** @brief Flight task active pouint8_t e_typeID */
    uint8_t activeFPointId;

    /** @brief Azimuth deviation from flight direction * 1 m */
    int16_t fdDeviation;

    /** @brief Distance to the next point of flight task * 10 m */
    int16_t nextPointDistance;

    /** @brief CRC */
    uint8_t crc8;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(addr);
        ar & BOOST_SERIALIZATION_NVP(packet_id);
        ar & BOOST_SERIALIZATION_NVP(size);
        ar & BOOST_SERIALIZATION_NVP(currentState);
        ar & BOOST_SERIALIZATION_NVP(roll);
        ar & BOOST_SERIALIZATION_NVP(pitch);
        ar & BOOST_SERIALIZATION_NVP(course);
        ar & BOOST_SERIALIZATION_NVP(kiasSpeed);
        ar & BOOST_SERIALIZATION_NVP(baroHeight);
        ar & BOOST_SERIALIZATION_NVP(verticalSpeed);
        ar & BOOST_SERIALIZATION_NVP(usonicAltmHeight);
        ar & BOOST_SERIALIZATION_NVP(trackAngle);
        ar & BOOST_SERIALIZATION_NVP(trackSpeed);
        ar & BOOST_SERIALIZATION_NVP(lattitude);
        ar & BOOST_SERIALIZATION_NVP(longitude);
        ar & BOOST_SERIALIZATION_NVP(snsHeight);
        ar & BOOST_SERIALIZATION_NVP(engineRPM);
        ar & BOOST_SERIALIZATION_NVP(engineTemperature1);
        ar & BOOST_SERIALIZATION_NVP(engineTemperature2);
        ar & BOOST_SERIALIZATION_NVP(aileronChannel);
        ar & BOOST_SERIALIZATION_NVP(elevatorChannel);
        ar & BOOST_SERIALIZATION_NVP(rudderChannel);
        ar & BOOST_SERIALIZATION_NVP(throttleChannel);
        ar & BOOST_SERIALIZATION_NVP(flapsChannel);
        ar & BOOST_SERIALIZATION_NVP(activeFPointId);
        ar & BOOST_SERIALIZATION_NVP(fdDeviation);
        ar & BOOST_SERIALIZATION_NVP(nextPointDistance);
        ar & BOOST_SERIALIZATION_NVP(crc8);
    }

    void print_debug_info(std::ostream& where){
        simple_log_archive log(where);
        log << *this;
        where << std::endl;
    }

};


#endif	/* TELEMETRY_H */

