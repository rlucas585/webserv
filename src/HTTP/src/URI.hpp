#ifndef URI_HPP
#define URI_HPP

#include "Slice.hpp"
#include "State.hpp"
#include "optional.hpp"
#include "result.hpp"

#include <map>

namespace http {
class URI {
  public:
    typedef Utils::result<URI, State> Result;

    enum e_Scheme {
        HTTP,
    };

    class Scheme {
      public:
        Scheme(void);
        Scheme(e_Scheme method);
        ~Scheme(void);
        Scheme(Scheme const& src);
        Scheme& operator=(Scheme const& rhs);

        operator e_Scheme() const;
        operator const char*() const;

      private:
        e_Scheme inner;

        static const char* enum_strings[];
    };

    static Result parse_uri(Slice uri_slice);

    URI(void);
    ~URI(void);
    URI(URI const& other);
    URI& operator=(URI const& rhs);

    Scheme const& scheme(void) const;
    Utils::optional<std::string> const& authority(void) const;
    std::string const& path(void) const;
    Utils::optional<std::string> const& query(void) const;
    Utils::optional<std::string> const& fragment(void) const;

    friend std::ostream& operator<<(std::ostream& o, URI const& uri);

    std::string to_string(void) const;

  private:
    Scheme scheme_;
    Utils::optional<std::string> authority_;
    std::string path_;
    Utils::optional<std::string> query_;
    Utils::optional<std::string> fragment_;

    URI(Scheme scheme, Utils::optional<std::string> authority, std::string path,
        Utils::optional<std::string> query, Utils::optional<std::string> fragment);

    static std::map<const Slice, URI::Scheme> create_scheme_map(void);
    static const std::map<const Slice, URI::Scheme> valid_schemes;

    static bool authority_is_present(Slice uri_slice_no_scheme);

    static const size_t URI_SIZE_LIMIT;
};

std::ostream& operator<<(std::ostream& o, URI::Scheme const& scheme);
std::ostream& operator<<(std::ostream& o, URI const& uri);

} // namespace http

#endif
