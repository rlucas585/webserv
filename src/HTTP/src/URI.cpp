#include "URI.hpp"

namespace http {

const size_t URI::URI_SIZE_LIMIT = 2048;

URI::Scheme::Scheme(void) : inner(HTTP) {}

URI::Scheme::Scheme(e_Scheme state) : inner(state) {}

URI::Scheme::~Scheme(void) {}

URI::Scheme::Scheme(Scheme const& src) : inner(src.inner) {}

URI::Scheme& URI::Scheme::operator=(Scheme const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    inner = rhs.inner;
    return *this;
}

const char* URI::Scheme::enum_strings[] = {
    "http://",
};

URI::Scheme::operator e_Scheme() const { return inner; }

URI::Scheme::operator const char*() const { return enum_strings[inner]; }

URI::URI(void) : scheme_(HTTP), path_("/") {}

URI::~URI(void) {}

URI::URI(Scheme scheme, Utils::optional<std::string> authority, std::string path,
         Utils::optional<std::string> query, Utils::optional<std::string> fragment)
    : scheme_(scheme), authority_(authority), path_(path), query_(query), fragment_(fragment) {}

URI::URI(URI const& other) { *this = other; }

// TODO optimise with "move semantics"
URI& URI::operator=(URI const& rhs) {
    if (&rhs == this) {
        return *this;
    }
    scheme_ = rhs.scheme_;
    authority_ = rhs.authority_;
    path_ = rhs.path_;
    query_ = rhs.query_;
    fragment_ = rhs.fragment_;
    return *this;
}

// Function should ideally be broken into smaller parts
URI::Result URI::parse_uri(Slice uri_slice) {
    Scheme scheme = HTTP;
    Utils::optional<std::string> authority = Utils::nullopt;
    std::string path("/");
    Utils::optional<std::string> query = Utils::nullopt;
    Utils::optional<std::string> fragment = Utils::nullopt;
    Slice path_and_query;

    if (uri_slice.length() > URI_SIZE_LIMIT) {
        return Result::Err(URITooLong_414);
    }
    Utils::optional<Slice> after_scheme = uri_slice.find("://");
    if (after_scheme.has_value()) {
        Slice slice_from_divider = after_scheme.unwrap();
        Slice scheme_string =
            Slice::newSliceWithLength(uri_slice.raw(), slice_from_divider.raw() - uri_slice.raw());
        std::map<const Slice, URI::Scheme>::const_iterator search =
            valid_schemes.find(scheme_string);

        if (search == valid_schemes.end() || slice_from_divider.length() <= 3) {
            return Result::Err(BadRequest_400);
        }
        scheme = search->second;
        Slice after_scheme_slice = Slice::newSliceWithOffset(slice_from_divider, 3);
        if (authority_is_present(after_scheme_slice)) {
            Slice::Split iter = after_scheme_slice.split("/");
            Slice authority_slice = iter.next();
            authority = Utils::make_optional(authority_slice.toString());
            if (iter.is_complete()) {
                return URI(scheme, authority, path, query, fragment);
            }
            path_and_query =
                Slice::newSliceWithOffset(after_scheme_slice, authority_slice.length());
        } else {
            path_and_query = after_scheme_slice;
        }
    } else {
        if (uri_slice.front().unwrap() != '/') {
            return Result::Err(BadRequest_400);
        }
        path_and_query = uri_slice;
    }
    Slice::Split iter = path_and_query.split("?");
    if (iter.count_remaining() > 2) {
        return Result::Err(BadRequest_400);
    }
    path = iter.next().toString();
    if (!iter.is_complete()) {
        Slice query_and_fragment_slice = iter.next();
        Slice::Split iter2 = query_and_fragment_slice.split("#");
        if (iter2.count_remaining() > 2) {
            return Result::Err(BadRequest_400);
        }
        query = Utils::make_optional(iter2.next().toString());
        if (!iter2.is_complete()) {
            fragment = Utils::make_optional(iter2.next().toString());
        }
    }
    return URI(scheme, authority, path, query, fragment);
}

URI::Scheme const& URI::scheme(void) const { return scheme_; }

Utils::optional<std::string const*> URI::authority(void) const {
    if (authority_.has_value()) {
        return Utils::make_optional(&*authority_);
    }
    return Utils::nullopt;
}

std::string const& URI::path(void) const { return path_; }

Utils::optional<std::string const*> URI::query(void) const {
    if (query_.has_value()) {
        return Utils::make_optional(&*query_);
    }
    return Utils::nullopt;
}

Utils::optional<std::string const*> URI::fragment(void) const {
    if (fragment_.has_value()) {
        return Utils::make_optional(&*fragment_);
    }
    return Utils::nullopt;
}

std::string URI::to_string(void) const {
    std::string output;

    output += static_cast<const char*>(scheme_);
    if (authority_.has_value()) {
        output += *authority_;
    }
    output += path_;
    if (query_.has_value()) {
        output += "?";
        output += *query_;
    }
    if (fragment_.has_value()) {
        output += "#";
        output += *fragment_;
    }
    return output;
}

std::map<const Slice, URI::Scheme> URI::create_scheme_map(void) {
    std::map<const Slice, URI::Scheme> m;
    m["http"] = HTTP;
    return m;
}

const std::map<const Slice, URI::Scheme> URI::valid_schemes = URI::create_scheme_map();

bool URI::authority_is_present(Slice uri_slice_no_scheme) {
    if (uri_slice_no_scheme.front().unwrap() != '/') {
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& o, URI::Scheme const& scheme) {
    o << static_cast<const char*>(scheme);
    return o;
}

std::ostream& operator<<(std::ostream& o, URI const& uri) {
    o << uri.scheme_;
    if (uri.authority_.has_value()) {
        o << *uri.authority_;
    }
    o << uri.path_;
    if (uri.query_.has_value()) {
        o << "?";
        o << *uri.query_;
    }
    if (uri.fragment_.has_value()) {
        o << "#";
        o << *uri.fragment_;
    }
    return o;
}

} // namespace http
