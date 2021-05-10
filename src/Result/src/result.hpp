/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   result.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/01 21:15:23 by rlucas        #+#    #+#                 */
/*   Updated: 2021/05/10 08:48:57 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESULT_HPP
#define RESULT_HPP

#include "../../Traits/src/type_traits.hpp"
#include "../../Utils/src/Utils.hpp"

#include <sys/types.h>

namespace Utils {

// E must be convertible to std::string for Error printing.
// Would be better with traits to test whether E has an << overload to an
// ostream.
template <typename T, typename E>
class result {
    // Ok and Err would be preferred, but naming-conflicts are unavoidable
    enum Status {
        Error,
        Okay,
    };

    typedef typename meta::remove_const<T>::type ok_type;
    typedef typename meta::remove_const<E>::type err_type;

    typedef typename meta::if_c<(sizeof(ok_type) > sizeof(err_type)), ok_type, err_type>::type
        larger_type;
    typedef meta::aligned_storage<larger_type> storage_type;

    Status stat;
    storage_type storage;

  public:
    ~result(void) { this->destroy(); }
    result(ok_type const& data) : stat(Okay) { this->construct(data); }
    result(err_type const& err) : stat(Error) { this->construct(err); }
    result(result const& other) {
        if (other.stat == Error)
            this->construct(other.err());
        else
            this->construct(other.ok());
    }
    result& operator=(result const& rhs) {
        this->destroy(); // XXX I worry that this may be called with an uninitialized result - Test
        if (rhs.stat == Error)
            this->construct(rhs.err());
        else
            this->construct(rhs.ok());
        return *this;
    }
    static result Ok(T const& data) { return result(data); }
    static result Err(E const& err) { return result(err); }

    bool is_ok(void) const { return stat == Okay; }
    bool is_err(void) const { return stat == Error; }
    ok_type unwrap(void) { return this->expect(); }
    ok_type expect(const char* errmsg = "Called unwrap on an Err value: ") {
        if (stat == Error) {
            throw Utils::runtime_error(
                std::string(errmsg).append("\"").append(this->err()).append("\""));
        }
        ok_type ret = this->ok();
        this->set_as_default_error(); // Value is "removed", so type now err's if unwrap called
        return ret;
    }
    err_type unwrap_err(void) { return this->expect_err(); }
    err_type expect_err(const char* errmsg = "Called unwrap_err on an Ok value.") {
        if (stat == Okay) {
            throw Utils::runtime_error(errmsg);
        }
        err_type ret = this->err();
        this->set_as_default_error();
        return ret;
    }
    ok_type const& ok(void) const { return *reinterpret_cast<ok_type const*>(storage.address()); }
    ok_type& ok(void) { return *reinterpret_cast<ok_type*>(storage.address()); }
    err_type const& err(void) const {
        return *reinterpret_cast<err_type const*>(storage.address());
    }
    err_type& err(void) { return *reinterpret_cast<err_type*>(storage.address()); }

    bool operator==(result<ok_type, err_type> const& other) const {
        if (stat != other.stat)
            return false;
        if (stat == Okay)
            return this->ok() == other.ok();
        else
            return this->err() == other.err();
    }
    bool operator!=(result<ok_type, err_type> const& other) const { return !(*this == other); }

  private:
    result(void);

    void construct(ok_type const& data) {
        new (storage.address()) ok_type(data);
        stat = Okay;
    }
    void construct(err_type const& err) {
        new (storage.address()) err_type(err);
        stat = Error;
    }
    template <class Expr>
    void construct(Expr const& expr, void const*) {
        if (stat == Error)
            new (storage.address()) err_type(expr);
        else
            new (storage.address()) ok_type(expr);
    }
    void destroy(void) {
        if (stat == Error) {
            this->get_err_ptr()->err_type::~err_type();
        } else {
            this->get_ok_ptr()->ok_type::~ok_type();
        }
    }
    void set_as_default_error(void) {
        this->destroy();
        new (storage.address()) err_type();
        stat = Error;
    }
    err_type const* get_err_ptr(void) const {
        return reinterpret_cast<err_type const*>(storage.address());
    }
    err_type* get_err_ptr(void) { return reinterpret_cast<err_type*>(storage.address()); }
    ok_type const* get_ok_ptr(void) const {
        return reinterpret_cast<ok_type const*>(storage.address());
    }
    ok_type* get_ok_ptr(void) { return reinterpret_cast<ok_type*>(storage.address()); }
};

// The following definitions are for reusable Result types that can be used
// across multiple classes.
typedef Utils::result<ssize_t, std::string> RwResult;

} // namespace Utils

#endif
