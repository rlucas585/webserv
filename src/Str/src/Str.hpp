#ifndef STR_HPP
#define STR_HPP

#include <iostream>
#include <string>
#include <stdexcept>

class Str {
	public:
		class Split {
			public:
				Split(const char *start);
				Split(const char *start, const char *delim);
				~Split(void);
				Split(Split const& src);
				Split	&operator=(Split const& rhs);
				Str		next(void);
				template <typename Container>
					Container	collect(void) {
						Container		c;
						Str				item;

						while (item = this->next()) {
							c.push_back(item);
						}
						return c;
					}

			private:
				const char 	*_remainder;
				const char	*_delimiter;

				Split(void) {}

				const char *	_findFirstNotOf(const char *s, const char *reject) const;
		};

	public:
		class iterator {
			public:
				iterator(const char *start, size_t len, size_t maxLen);
				~iterator(void);
				iterator(iterator const& src);
				iterator	&operator=(iterator const& rhs);
				iterator	&operator++();
				iterator	operator++(int);
				iterator	&operator--();
				iterator	operator--(int);
				bool		operator==(const iterator &rhs) const;
				bool		operator!=(const iterator &rhs) const;
				const char	&operator*(void) const;

			private:
				const char *_p;
				size_t		_len;
				size_t		_maxLen;
		};

	public:
		Str(void);
		Str(const char *data);

		Str(std::string const& src);
		Str		&operator=(std::string const& str);
		static Str	newSlice(std::string const& str);
		static Str	newSliceWithLength(std::string const& str, size_t len);
		static Str	newSliceWithOffset(std::string const& str, size_t offset);
		static Str	newSliceWithLengthAndOffset(std::string const& str, size_t len, size_t offset);
		operator	std::string() const;
		std::string	toString(void) const;

		static char	emptyBuffer[1];

		~Str(void);
		Str(Str const& src) { *this = src; }
		Str		&operator=(Str const& rhs);
		Str		&operator=(const char *data);

		static Str	newSlice(const char *data);
		static Str	newSliceWithLength(const char *data, size_t len);
		static Str	newSliceWithOffset(const char *data, size_t offset);
		static Str	newSliceWithOffset(Str const& src, size_t offset);
		static Str	newSliceWithLengthAndOffset(const char *data, size_t len, size_t offset);

		size_t		length(void) const;
		bool		isInitialized(void) const;
		iterator	begin(void) const;
		iterator	end(void) const;

		Split		split(const char *delim = " ");

		bool		operator==(Str const& rhs) const;
		bool		operator!=(Str const& rhs) const;
		bool		operator<(Str const& rhs) const;
		bool		operator<=(Str const& rhs) const;
		bool		operator>(Str const& rhs) const;
		bool		operator>=(Str const& rhs) const;

		operator	bool() const;

	private:
		const char *		_data;
		size_t					_len;

		bool		_throwIfUninitialized(void) const;

		Str(const char *data, size_t len);
		Str(const char *data, size_t len, size_t offset);
};

bool		operator==(std::string lhs, Str const& rhs);
bool		operator!=(std::string lhs, Str const& rhs);
bool		operator<(std::string lhs, Str const& rhs);
bool		operator<=(std::string lhs, Str const& rhs);
bool		operator>(std::string lhs, Str const& rhs);
bool		operator>=(std::string lhs, Str const& rhs);

std::ostream&		operator<<(std::ostream &o, Str const& str);

#endif
