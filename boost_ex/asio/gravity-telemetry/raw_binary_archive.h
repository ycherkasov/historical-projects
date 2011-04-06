/* 
 * File:   raw_binary_archive.h
 * Author: straight
 *
 * Created on 24 Март 2011 г., 11:51
 */

#ifndef RAW_BINARY_ARCHIVE_H
#define	RAW_BINARY_ARCHIVE_H
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

class raw_binary_oarchive :
public binary_oarchive_impl<raw_binary_oarchive, std::ostream::char_type,
        std::ostream::traits_type> {

    typedef raw_binary_oarchive derived_t;
    typedef binary_oarchive_impl<raw_binary_oarchive, std::ostream::char_type,
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
    }

    void save_override(const boost::archive::class_name_type & t, int) { }

    void save_override(const boost::archive::library_version_type & t, int) { }

    void save_override(const boost::archive::version_type & t, int) { }

    void save_override(const boost::archive::class_id_type & t, int) { }

    void save_override(const boost::archive::class_id_reference_type & t, int) { }

    void save_override(const boost::archive::class_id_optional_type & t, int) { }

    void save_override(const boost::archive::object_id_type & t, int) { }

    void save_override(const boost::archive::object_reference_type & t, int) { }

    void save_override(const boost::archive::tracking_type & t, int) { }


public:

    raw_binary_oarchive(std::ostream & os, unsigned flags = 0) :
    base_t(os, flags | boost::archive::no_header) { }

    raw_binary_oarchive(std::streambuf & bsb, unsigned int flags = 0) :
    base_t(bsb, flags | boost::archive::no_header) { }
};

class raw_binary_iarchive 
    : public binary_iarchive_impl<raw_binary_iarchive, std::istream::char_type,
        std::istream::traits_type> {

    typedef raw_binary_iarchive derived_t;
    typedef binary_iarchive_impl<raw_binary_iarchive,std::istream::char_type,
        std::istream::traits_type> base_t;

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

    void load_override(const boost::archive::class_name_type& t, int) { }

    void load_override(const boost::archive::library_version_type& t, int) { }

    void load_override(const boost::archive::version_type& t, int) { }

    void load_override(const boost::archive::class_id_type& t, int) { }

    void load_override(const boost::archive::class_id_reference_type& t, int) { }

    void load_override(const boost::archive::class_id_optional_type& t, int) { }

    void load_override(const boost::archive::object_id_type& t, int) { }

    void load_override(const boost::archive::object_reference_type& t, int) { }

    void load_override(const boost::archive::tracking_type& t, int) { }

    void load_override(boost::archive::class_name_type& t, int) { }

    void load_override(boost::archive::library_version_type& t, int) { }

    void load_override(boost::archive::version_type& t, int) { }

    void load_override(boost::archive::class_id_type& t, int) { }

    void load_override(boost::archive::class_id_reference_type& t, int) { }

    void load_override(boost::archive::class_id_optional_type& t, int) { }

    void load_override(boost::archive::object_id_type& t, int) { }

    void load_override(boost::archive::object_reference_type& t, int) { }

    void load_override(boost::archive::tracking_type& t, int) { }

public:

    raw_binary_iarchive(std::istream & is, unsigned int flags = 0) :
    base_t(is, flags | boost::archive::no_header) { }

    raw_binary_iarchive(std::streambuf & bsb, unsigned int flags = 0) :
    base_t(bsb, flags | boost::archive::no_header) { }
};


#endif	/* RAW_BINARY_ARCHIVE_H */

