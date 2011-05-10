/* 
 * File:   packets.h
 * Author: ycherkasov
 *
 * Created on April 26, 2011, 2:58 PM
 */

#ifndef PACKETS_H
#define	PACKETS_H

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>

struct base {
    uint8_t x;
    base():x(2){}
    virtual ~base(){}

    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int file_version) {
        ar & BOOST_SERIALIZATION_NVP(x);
    }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(base)

struct derived_one : public base {
    uint8_t y;
    derived_one():y(11){}

    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int file_version) {
        boost::serialization::base_object<base>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(base);
        ar & BOOST_SERIALIZATION_NVP(y);
    }
};

BOOST_CLASS_EXPORT(base);

struct derived_two : public base {
    uint8_t z;

    derived_two():z(22){}

    template<class Archive>
    inline void serialize(Archive & ar, const unsigned int file_version) {
        boost::serialization::base_object<base>(*this);
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(base);
        ar & BOOST_SERIALIZATION_NVP(z);
    }
};

BOOST_CLASS_EXPORT(derived_one);
BOOST_CLASS_EXPORT(derived_two);

#endif	/* PACKETS_H */

