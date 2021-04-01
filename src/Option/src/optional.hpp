/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   optional.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/01 17:16:36 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/01 22:34:11 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include "../../Utils/src/Utils.hpp"
#include "../../Utils/src/type_traits.hpp"

namespace Utils {

struct nullopt_t {
    explicit nullopt_t(int) {}
};

const nullopt_t nullopt(0);

// Glaring inefficiencies in optional due to the lack of move
// semantics in C++98, thus requiring copy semantics
template <typename T> class optional {
    typedef typename remove_const<T>::type stored_type;
    struct empty_byte {};
    union {
        empty_byte empty;
        stored_type payload;
    };
    bool engaged;

  public:
    optional(void) : empty(), engaged(false) {}
    ~optional(void) { this->reset(); }
    optional(struct nullopt_t) : empty(), engaged(false) {}
    optional(T const& data) : empty(), engaged(false) { this->set(data); }
    optional(optional const& other) : empty(), engaged(false) {
        if (other.engaged) {
            this->set(other.payload);
        }
    }
    optional& operator=(optional const& rhs) {
        if (this == &rhs) {
            return *this;
        }
        this->reset();
        if (rhs.engaged) {
            this->set(rhs.payload);
        }
        return *this;
    }
    optional& operator=(struct nullopt_t) {
        this->reset();
        return *this;
    }
    void set(stored_type const& data) {
        if (engaged)
            payload = data;
        else
            new (&payload) stored_type(data);
        engaged = true;
    }
    bool has_value(void) const { return engaged; }
    stored_type& value(void) {
        if (!engaged)
            throw Utils::runtime_error("Bad optional access");
        return payload;
    }
    stored_type const& value(void) const {
        if (!engaged)
            throw Utils::runtime_error("Bad optional access");
        return payload;
    }
    stored_type unwrap(void) { return this->expect(); }
    stored_type expect(const char* errmsg = "Bad optional access") {
        stored_type ret = payload;
        if (!engaged)
            throw Utils::runtime_error(errmsg);
        this->reset(); // "Remove" the stored value
        return ret;
    }
    stored_type value_or(stored_type const& default_value) const {
        if (engaged)
            return payload;
        return default_value;
    }
    void swap(optional& other) {
        if (!other.engaged && !engaged)
            return;
        else if (engaged && !other.engaged) {
            other.set(payload);
            this->reset();
        } else if (!engaged && other.engaged) {
            this->set(other.payload);
            other.reset();
        } else {
            stored_type tmp_payload = other.payload;
            other.set(payload);
            this->set(tmp_payload);
        }
    }
    void reset(void) {
        if (engaged)
            payload.~stored_type();
        engaged = false;
    }
    stored_type& operator*() { return payload; }
    stored_type const& operator*() const { return payload; }
    stored_type* operator->() { return &payload; }
    const stored_type* operator->() const { return &payload; }
};

template <typename T> optional<T> make_optional(T const& data) { return optional<T>(data); }
} // namespace Utils

#endif
