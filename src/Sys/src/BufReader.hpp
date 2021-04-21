#ifndef BUFREADER_HPP
#define BUFREADER_HPP

#include "../../Option/src/optional.hpp"
#include "../../Result/src/result.hpp"
#include "../../Str/src/Str.hpp"
#include "FileDesc.hpp"

#ifndef STRING_LIMIT
#define STRING_LIMIT 1024
#endif

template <typename R>
Utils::RwResult read_until(R& reader, char delimiter, std::string& buf,
                           size_t read_limit = STRING_LIMIT);

// R must be some sort of Reader, such as a file, or a socket.
// They must have the read(void* buf, size_t len) member function.
template <typename R>
class BufReader {
  public:
    typedef Utils::result<Str, std::string> FillResult;

  public:
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
            Str::newSliceWithLengthAndOffset(buffer.get(), capacity - position, position));
    }

    void consume(size_t amount) { position = Utils::min(position + amount, capacity); }

    Utils::RwResult read_until(char delimiter, std::string& buf, size_t read_limit = STRING_LIMIT) {
        return ::read_until(*this, delimiter, buf, read_limit);
    }

    Utils::RwResult read_line(std::string& buf, size_t read_limit = STRING_LIMIT) {
        return ::read_until(*this, '\n', buf, read_limit);
    }

  private:
    R inner;
    Utils::unique_ptr<char> buffer;
    size_t position;
    size_t capacity;
    size_t buffer_size;

    BufReader(void);
    BufReader(size_t capacity, R new_inner)
        : inner(new_inner), buffer(new char[capacity]), position(0), capacity(0),
          buffer_size(capacity) {}

    static const size_t DEFAULT_BUF_SIZE = 1024;
};

// R should be a BufReader or similar
template <typename R>
Utils::RwResult read_until(R& reader, char delimiter, std::string& buf, size_t read_limit) {
    size_t bytes_read = 0;
    while (true) {
        bool done;
        size_t used;

        // First, fill the buffer (may or may not require read, see fill_buf impl)
        typename R::FillResult fill_res = reader.fill_buf();
        if (fill_res.is_err()) {
            return Utils::RwResult::Err(fill_res.unwrap_err());
        }
        // Str slice points to unread data in buffer
        Str available = fill_res.unwrap();
        // next is slice to same data as in available, but from the next delimiter
        Utils::optional<Str> next_opt = available.strchr(delimiter);
        if (next_opt.has_value()) {
            Str next = next_opt.unwrap();
            size_t length = next.raw() - available.raw();
            Str extension = Str::newSliceWithLength(available.raw(), length + 1);
            if (buf.size() + length + 1 > read_limit) {
                return Utils::RwResult::Err(std::string("Over read_limit"));
            }
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

#endif
