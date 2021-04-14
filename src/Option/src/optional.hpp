/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   optional.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/01 17:16:36 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 19:14:52 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include "../../AlignedStorage/src/aligned_storage.hpp"
#include "../../Traits/src/type_traits.hpp"
#include "../../Utils/src/Utils.hpp"

namespace Utils {

struct nullopt_t {
    explicit nullopt_t(int) {}
};

const nullopt_t nullopt(0);

// Glaring inefficiencies in optional due to the lack of move
// semantics in C++98, thus requiring copy semantics
template <typename T>
class optional {
    typedef typename meta::remove_const<T>::type stored_type;
    typedef Utils::aligned_storage<stored_type> storage_type;

    bool engaged;
    storage_type storage;

  public:
    optional(void) : engaged(false) {}
    ~optional(void) { this->reset(); }
    optional(struct nullopt_t) : engaged(false) {}
    optional(stored_type const& data) : engaged(false) { this->construct(data); }
    optional(optional const& other) : engaged(false) {
        if (other.engaged) {
            this->construct(other.value());
        }
    }
    optional& operator=(optional const& rhs) {
        if (this == &rhs) {
            return *this;
        }
        this->reset();
        if (rhs.engaged) {
            this->set(rhs.value());
        }
        return *this;
    }
    optional& operator=(struct nullopt_t) {
        this->reset();
        return *this;
    }
    void set(stored_type const& data) { this->construct(data); }
    bool has_value(void) const { return engaged; }
    stored_type& value(void) {
        if (!engaged)
            throw Utils::runtime_error("Bad optional access");
        return *reinterpret_cast<stored_type*>(storage.address());
    }
    stored_type const& value(void) const {
        if (!engaged)
            throw Utils::runtime_error("Bad optional access");
        return *reinterpret_cast<stored_type const*>(storage.address());
    }
    stored_type unwrap(void) { return this->expect(); }
    stored_type expect(const char* errmsg = "Bad optional access") {
        if (!engaged)
            throw Utils::runtime_error(errmsg);
        stored_type ret = this->value();
        this->reset(); // "Remove" the stored value
        return ret;
    }
    stored_type value_or(stored_type const& default_value) const {
        if (engaged)
            return this->value();
        return default_value;
    }
    void swap(optional& other) {
        if (!other.engaged && !engaged)
            return;
        else if (engaged && !other.engaged) {
            other.set(this->value());
            this->reset();
        } else if (!engaged && other.engaged) {
            this->set(other.value());
            other.reset();
        } else {
            stored_type tmp_payload = other.value();
            other.set(this->value());
            this->set(tmp_payload);
        }
    }
    void reset(void) {
        if (engaged)
            this->get_ptr()->stored_type::~stored_type();
        engaged = false;
    }
    stored_type& operator*() { return *reinterpret_cast<stored_type*>(storage.address()); }
    stored_type const& operator*() const {
        return *reinterpret_cast<stored_type*>(storage.address());
    }
    stored_type* operator->() { return reinterpret_cast<stored_type*>(storage.address()); }
    const stored_type* operator->() const {
        return reinterpret_cast<stored_type*>(storage.address());
    }

  private:
    void construct(stored_type const& data) {
        if (engaged)
            this->reset();
        new (storage.address()) stored_type(data);
        engaged = true;
    }
    template <class Expr>
    void construct(Expr const& expr, void const*) {
        if (engaged)
            this->reset();
        new (storage.address()) stored_type(expr);
        engaged = true;
    }
    stored_type const* get_ptr(void) const {
        return reinterpret_cast<stored_type const*>(storage.address());
    }
    stored_type* get_ptr(void) { return reinterpret_cast<stored_type*>(storage.address()); }
};

template <typename T>
optional<T> make_optional(T const& data) {
    return optional<T>(data);
}
} // namespace Utils

#endif
