#ifndef BUFREADER_HPP
#define BUFREADER_HPP

#include "optional.hpp"
#include "result.hpp"
#include "Slice.hpp"
#include "FileDesc.hpp"

#include "TcpStream.hpp"

// Prototypes
template <typename R>
Utils::RwResult read_until(R& reader, char delimiter, std::string& buf);
template <typename R>
Utils::RwResult read_char(R& reader, char& c);

// R must be some sort of Reader, such as a file, or a socket.
// They must have the read(void* buf, size_t len) member function.
template <typename R>
class BufReader {
  public:
    typedef Utils::result<Slice, std::string> FillResult;

  public:
    BufReader(void)
        : inner(R()), buffer(new char[DEFAULT_BUF_SIZE]), position(0), capacity(0),
          buffer_size(DEFAULT_BUF_SIZE) {}
    ~BufReader(void) {}
    BufReader(BufReader const& other) { *this = other; }
    BufReader& operator=(BufReader const& rhs) {
        if (this == &rhs) {
            return *this;
        }
        BufReader& mutable_ref = const_cast<BufReader&>(rhs);

        inner = rhs.inner;
        buffer = mutable_ref.buffer;
        position = rhs.position;
        capacity = rhs.capacity;
        buffer_size = rhs.buffer_size;
        return *this;
    }

    static BufReader init(R new_inner) { return BufReader(DEFAULT_BUF_SIZE, new_inner); }
    static BufReader with_capacity(size_t capacity, R new_inner) {
        return BufReader(capacity, new_inner);
    }

    void reset(R new_inner) {
        inner = new_inner;
        position = 0;
        capacity = 0;
    }

    size_t get_position(void) const { return position; }

    size_t get_buffer_size(void) const { return buffer_size; }

    size_t get_capacity(void) const { return capacity; }

    FillResult fill_buf(void) {
        if (position >= capacity) {
            Utils::RwResult read_res = inner.read(buffer.get(), buffer_size);
            if (read_res.is_err()) {
                return FillResult::Err(read_res.unwrap_err());
            }
            capacity = read_res.unwrap();
            position = 0;
        }
        return FillResult::Ok(
            Slice::newSliceWithLengthAndOffset(buffer.get(), capacity - position, position));
    }

    bool eof(char delim = '\n') {
        return eof_impl(
            delim,
            meta::is_same<typename meta::type_enable<meta::is_stream<R>::value, R>::type, R>());
    }

    void consume(size_t amount) { position = Utils::min(position + amount, capacity); }

    Utils::RwResult read_until(char delimiter, std::string& buf) {
        return ::read_until(*this, delimiter, buf);
    }

    Utils::RwResult read_line(std::string& buf) { return ::read_until(*this, '\n', buf); }

    Utils::RwResult read_char(char& c) { return ::read_char(*this, c); }

    Utils::RwResult read_remaining(std::string& buf) {
        if (position >= capacity) {
            return Utils::RwResult::Ok(0);
        }
        Slice remaining =
            Slice::newSliceWithLengthAndOffset(buffer.get(), capacity - position, position);
        size_t bytes_read = remaining.length();
        append_slice_to_string(buf, remaining);

        return Utils::RwResult::Ok(bytes_read);
    }

    R& as_inner(void) { return inner; }
    R const& as_inner(void) const { return inner; }

  private:
    // implementation for a stream
    bool eof_impl(char delim, meta::true_type) {
        if (capacity == 0) // No read has occurred
            return false;
        if (capacity == buffer_size) // Read filled buffer completely, more available
            return false;

        Slice remaining =
            Slice::newSliceWithLengthAndOffset(buffer.get(), capacity - position, position);
        Utils::optional<Slice> next_opt = remaining.strchr(delim);
        if (next_opt.has_value())
            return false;
        else
            return true;
    }

    // implementation for a file
    bool eof_impl(char delim, meta::false_type) {
        (void)delim;
        if (position < capacity)
            return false;

        if (capacity == buffer_size)
            return false;

        FillResult fill_res = fill_buf();

        if (fill_res.is_err()) // If failure to read, return eof() == true
            return true;

        Slice available = fill_res.unwrap();
        if (available.length() == 0)
            return true;
        return false;
    }

  private:
    R inner;
    Utils::array_unique_ptr<char> buffer;
    size_t position;
    size_t capacity;
    size_t buffer_size;

    BufReader(size_t capacity, R new_inner)
        : inner(new_inner), buffer(new char[capacity]), position(0), capacity(0),
          buffer_size(capacity) {
        Utils::memset(reinterpret_cast<void*>(&*buffer), '\0', capacity);
    }

    static const size_t DEFAULT_BUF_SIZE = 1024;
};

// R should be a BufReader or similar
template <typename R>
Utils::RwResult read_until(R& reader, char delimiter, std::string& buf) {
    size_t bytes_read = 0;
    while (true) {
        bool done;
        size_t used;

        // First, fill the buffer (may or may not require read, see fill_buf impl)
        typename R::FillResult fill_res = reader.fill_buf();
        if (fill_res.is_err()) {
            return Utils::RwResult::Err(fill_res.unwrap_err());
        }
        // Slice slice points to unread data in buffer
        Slice available = fill_res.unwrap();
        // next is slice to same data as in available, but from the next delimiter
        Utils::optional<Slice> next_opt = available.strchr(delimiter);
        if (next_opt.has_value()) {
            Slice next = next_opt.unwrap();
            size_t length = next.raw() - available.raw();
            Slice extension = Slice::newSliceWithLength(available.raw(), length + 1);
            append_slice_to_string(buf, extension);
            done = true;
            used = length + 1;
        } else {
            append_slice_to_string(buf, available);
            done = false;
            used = available.length();
        }
        reader.consume(used); // Move buffer position along by 'used' bytes
        bytes_read += used;
        // Return once we've read until delimiter, or when EOF is reached
        if (done || used == 0) {
            return Utils::RwResult::Ok(bytes_read);
        }
    }
}

template <typename R>
Utils::RwResult read_char(R& reader, char& c) {
    typename R::FillResult fill_res = reader.fill_buf();
    if (fill_res.is_err()) {
        return Utils::RwResult::Err(fill_res.unwrap_err());
    }
    Slice available = fill_res.unwrap();
    if (available.length() == 0)
        return Utils::RwResult::Ok(0);
    c = available.front().unwrap(); // TODO Could add front() to Slice.
    reader.consume(1);
    return Utils::RwResult::Ok(1);
}

#endif
