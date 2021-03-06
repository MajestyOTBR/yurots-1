#ifndef BOOST_ARCHIVE_DETAIL_COMMON_IARCHIVE_HPP
#define BOOST_ARCHIVE_DETAIL_COMMON_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// common_iarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/archive/detail/basic_iarchive.hpp>
#include <boost/archive/detail/basic_pointer_iserializer.hpp>
#include <boost/archive/detail/interface_iarchive.hpp>
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/serialization/singleton.hpp>

namespace boost {
namespace archive {
namespace detail {

class extended_type_info;

// note: referred to as Curiously Recurring Template Patter (CRTP)
template<class Archive>
class common_iarchive : 
    public basic_iarchive,
    public interface_iarchive<Archive>
{
    friend class interface_iarchive<Archive>;
private:
    virtual void vload(version_type & t){
        * this->This() >> t; 
    }
    virtual void vload(object_id_type & t){
        * this->This() >> t;
    }
    virtual void vload(class_id_type & t){
        * this->This() >> t;
    }
    virtual void vload(class_id_optional_type & t){
        * this->This() >> t;
    }
    virtual void vload(tracking_type & t){
        * this->This() >> t;
    }
    virtual void vload(class_name_type &s){
        * this->This() >> s;
    }
protected:
    // default processing - invoke serialization library
    template<class T>
    void load_override(T & t, BOOST_PFTO int){
        archive::load(* this->This(), t);
    }
    // default implementations of functions which emit start/end tags for
    // archive types that require them.
    void load_start(const char * /*name*/){}
    void load_end(const char * /*name*/){}
    // default archive initialization
    common_iarchive(unsigned int flags = 0) : 
        basic_iarchive(flags),
        interface_iarchive<Archive>()
    {}
public:
    virtual const basic_pointer_iserializer * 
    find(const boost::serialization::extended_type_info & eti) const {
    	return static_cast<const basic_pointer_iserializer *>(
            archive_serializer_map<Archive>::find(eti)
        );
    }
};

} // namespace detail
} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_DETAIL_COMMON_IARCHIVE_HPP

