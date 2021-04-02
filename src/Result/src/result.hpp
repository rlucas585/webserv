/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   result.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/01 21:15:23 by rlucas        #+#    #+#                 */
/*   Updated: 2021/04/02 17:38:48 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESULT_HPP
#define RESULT_HPP

#include "../../Traits/src/type_traits.hpp"
#include "../../Utils/src/Utils.hpp"

namespace Utils {

// E must be convertible to std::string for Error printing.
// Would be better with traits to test whether E has an << overload to an
// ostream.
template <typename T, typename E> class result {
    // Ok and Err would be preferred, but naming-conflicts are unavoidable
    enum Status {
        Error,
        Okay,
    };

    typedef typename meta::remove_const<T>::type stored_type;
    typedef typename meta::remove_const<E>::type error_type;

    union {
        error_type error;
        stored_type payload;
    };
    Status stat;

  public:
    ~result(void) { this->destroy(); }
    result(T const& data) : payload(data), stat(Okay) {}
    result(E const& err) : error(err), stat(Error) {}
    result(result const& other) {
        if (other.stat == Error)
            new (&error) error_type(other.error);
        else
            new (&payload) stored_type(other.payload);
    }
    result& operator=(result const& rhs) {
        if (rhs.stat == Error)
            new (&error) error_type(rhs.error);
        else
            new (&payload) stored_type(rhs.payload);
    }
    static result Ok(T const& data) { return result(data); }
    static result Err(E const& err) { return result(err); }

    bool is_ok(void) const { return stat == Okay; }
    bool is_err(void) const { return stat == Error; }
    stored_type unwrap(void) { return this->expect(); }
    stored_type expect(const char* errmsg = "Called unwrap on an Err value: ") {
        stored_type ret = payload;
        if (stat == Error) {
            throw Utils::runtime_error(std::string(errmsg) + "\"" + error + "\"");
        }
        this->set_as_default_error(); // Value is "removed", so type now err's if unwrap called
        return ret;
    }
    error_type unwrap_err(void) { return this->expect_err(); }
    error_type expect_err(const char* errmsg = "Called unwrap_err on an Ok value.") {
        error_type ret = error;
        if (stat == Okay) {
            throw Utils::runtime_error(errmsg);
        }
        this->set_as_default_error();
        return ret;
    }

  private:
    result(void);

    void destroy(void) {
        if (stat == Error)
            error.~error_type();
        else
            payload.~stored_type();
    }
    void set_as_default_error(void) {
        if (stat == Error)
            error.~error_type();
        else
            payload.~stored_type();
        new (&error) error_type();
        stat = Error;
    }
};

} // namespace Utils

#endif
