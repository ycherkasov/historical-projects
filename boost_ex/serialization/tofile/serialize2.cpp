#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <vector>

// include headers that implement a archive in simple text format
#include <boost/archive/binary_oarchive_impl.hpp>
#include <boost/archive/binary_iarchive_impl.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// include template definitions for base classes used.  Otherwise
// you'll get link failure with undefined symbols
#include <boost/archive/impl/basic_binary_oprimitive.ipp>
#include <boost/archive/impl/basic_binary_iprimitive.ipp>
#include <boost/archive/impl/basic_binary_oarchive.ipp>
#include <boost/archive/impl/basic_binary_iarchive.ipp>

// включаем, чтобы сериализация работала с векторами
#include <boost/serialization/vector.hpp>
// включаем, чтобы нормально проходила сериализация XML
#include <boost/serialization/nvp.hpp>

#include <boost/utility/enable_if.hpp>

using namespace boost::archive;

enum {
    e_IdentityRequest = 0x00,
    e_IdentityResponse = 0x01,

    e_TelemetryRequest = 0x10,
    e_TelemetryResponse = 0x11
};

class gps_position {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        // то же, что и make_nvp, только имя параметра выводится в макросе
        ar & BOOST_SERIALIZATION_NVP(degrees);
        ar & BOOST_SERIALIZATION_NVP(minutes);
        ar & BOOST_SERIALIZATION_NVP(seconds);
    }
    int degrees;
    int minutes;
    float seconds;
public:

    gps_position() {
    };

    gps_position(int d, int m, float s) :
    degrees(d), minutes(m), seconds(s) {
    }
};

struct TelemetryRequest {
    static const int telemetryRequestPacketSize = 4;
    static const uint8_t e_typeID = e_TelemetryRequest;

    TelemetryRequest()
    : addr(0x01)
    , packet_id(e_TelemetryRequest)
    , size(telemetryRequestPacketSize) {
    }

    //friend std::ostream & operator<<(std::ostream &os, const TelemetryRequest & gp);

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

};

//std::ostream & operator<<(std::ostream &os, const TelemetryRequest& t) {
//    return os << t.addr << t.packet_id << t.size << t.crc8;
//}

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
    , nextPointDistance() {
    }

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

    enum E_Get_Control_Mode {
        e_get_control_unknown,
        e_get_control_mode_futaba,
        e_get_control_mode_manual,
        e_get_control_mode_auto,
    };

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

    std::string get_debug_info() {
        char str[BUFSIZ];
        snprintf(str, BUFSIZ, "TelemetryResponse: addr:%d, packet_id:%d, size:%d, state:%x"
                ", activeFPointId:%d, Deviation:%d, nextPointDistance:%d"
                , addr, packet_id, size, currentState
                , activeFPointId, fdDeviation, nextPointDistance);
        return str;
    }

};
using namespace boost::archive;

class fast_binary_oarchive :
public binary_oarchive_impl<
fast_binary_oarchive,
std::ostream::char_type,
std::ostream::traits_type
> {
    typedef fast_binary_oarchive derived_t;
    typedef binary_oarchive_impl<fast_binary_oarchive, std::ostream::char_type,
    std::ostream::traits_type> base_t;
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class boost::archive::detail::interface_oarchive<derived_t>;
    friend class basic_binary_oarchive<derived_t>;
    friend class basic_binary_oprimitive<
    derived_t,
    std::ostream::char_type,
    std::ostream::traits_type
    >;
    friend class boost::archive::save_access;
#endif
    // add base class to the places considered when matching
    // save function to a specific set of arguments.  Note, this didn't
    // work on my MSVC 7.0 system using
    // binary_oarchive_impl<derived_t>::load_override;
    // so we use the sure-fire method below.  This failed to work as well

    template<class T>
    void save_override(T & t, BOOST_PFTO int) {
        base_t::save_override(t, 0);
        //t.serialize(*this, 0);
    }
    void save_override(const boost::archive::class_name_type & t, int) {}
    void save_override(const boost::archive::library_version_type & t, int) {}
    void save_override(const boost::archive::version_type & t, int) {}
    void save_override(const boost::archive::class_id_type & t, int) {}
    void save_override(const boost::archive::class_id_reference_type & t, int) {}
    void save_override(const boost::archive::class_id_optional_type & t, int) {}
    void save_override(const boost::archive::object_id_type & t, int) {}
    void save_override(const boost::archive::object_reference_type & t, int) {}
    void save_override(const boost::archive::tracking_type & t, int) {}


public:

    fast_binary_oarchive(std::ostream & os, unsigned flags = 0) :
    base_t(os, flags | boost::archive::no_header) {
    }

    fast_binary_oarchive(std::streambuf & bsb, unsigned int flags = 0) :
    base_t(bsb, flags | boost::archive::no_header) {
    }
};

class fast_binary_iarchive :
// don't derive from binary_oarchive !!!
public binary_iarchive_impl<
fast_binary_iarchive,
std::istream::char_type,
std::istream::traits_type
> {
    typedef fast_binary_iarchive derived_t;
    typedef binary_iarchive_impl<
    fast_binary_iarchive,
    std::istream::char_type,
    std::istream::traits_type
    > base_t;
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class boost::archive::detail::interface_iarchive<derived_t>;
    friend class basic_binary_iarchive<derived_t>;
    friend class basic_binary_iprimitive<
    derived_t,
    std::ostream::char_type,
    std::ostream::traits_type
    >;
    friend class boost::archive::load_access;
#endif
    // add base class to the places considered when matching
    // save function to a specific set of arguments.  Note, this didn't
    // work on my MSVC 7.0 system using
    // binary_oarchive_impl<derived_t>::load_override;
    // so we use the sure-fire method below.  This failed to work as well

    template<class T>
    void load_override(T & t, BOOST_PFTO int) {
        base_t::load_override(t, 0);
    }

    void load_override(const boost::archive::class_name_type & t, int) {}
    void load_override(const boost::archive::library_version_type & t, int) {}
    void load_override(const boost::archive::version_type & t, int) {}
    void load_override(const boost::archive::class_id_type & t, int) {}
    void load_override(const boost::archive::class_id_reference_type & t, int) {}
    void load_override(const boost::archive::class_id_optional_type & t, int) {}
    void load_override(const boost::archive::object_id_type & t, int) {}
    void load_override(const boost::archive::object_reference_type & t, int) {}
    void load_override(const boost::archive::tracking_type & t, int) {}

    void load_override( boost::archive::class_name_type & t, int) {}
    void load_override( boost::archive::library_version_type & t, int) {}
    void load_override( boost::archive::version_type & t, int) {}
    void load_override( boost::archive::class_id_type & t, int) {}
    void load_override( boost::archive::class_id_reference_type & t, int) {}
    void load_override( boost::archive::class_id_optional_type & t, int) {}
    void load_override( boost::archive::object_id_type & t, int) {}
    void load_override( boost::archive::object_reference_type & t, int) {}
    void load_override( boost::archive::tracking_type & t, int) {}

public:

    fast_binary_iarchive(std::istream & is, unsigned int flags = 0) :
    base_t(is, flags | boost::archive::no_header) {
    }

    fast_binary_iarchive(std::streambuf & bsb, unsigned int flags = 0) :
    base_t(bsb, flags | boost::archive::no_header) {
    }
};

int main(int argc, char* argv[]) {
    //std::ofstream ofs("filename");

    TelemetryRequest t;
    t.crc8 = 0xFF;

    using namespace boost::archive;
    //std::ios_base::openmode mode_txt = static_cast<std::ios_base::openmode>(0);
    std::ios_base::openmode mode_bin = std::ios::binary;
    std::string file("binary_arch.dump");
    //TestArch<text_iarchive, text_oarchive > ("text_arch.dump", mode_txt, t);
    //TestArch<binary_iarchive, binary_oarchive > ("binary_arch.dump", mode_bin, t);
    //TestArch<xml_iarchive, xml_oarchive > ("xml_arch.dump", mode_txt, t);

    int flag = boost::archive::no_header;
    { // Сериализуем
        std::ofstream ofs(file.c_str(), std::ios::out | std::ios::binary);
        //binary_oarchive oa(ofs, flag);
        fast_binary_oarchive oa(ofs, flag);
        // make_nvp создаёт пару имя-значение, которая отразится в XML
        // если не используем XML архив, то можно пару не создавать
        oa << t; //boost::serialization::make_nvp("Test_Object", t);
    }

    TelemetryRequest newg;
    { // Десериализуем
        std::ifstream ifs(file.c_str(), std::ios::in | std::ios::binary);
        binary_iarchive ia(ifs, flag);
        ia >> newg; //boost::serialization::make_nvp("Test_Object", newg);
    }

    return 0;
}

