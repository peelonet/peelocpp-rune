/*
 * Copyright (c) 2016, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PEELO_TEXT_RUNESTRING_HPP_GUARD
#define PEELO_TEXT_RUNESTRING_HPP_GUARD

#include <peelo/text/rune.hpp>
#include <vector>

namespace peelo
{
  /**
   * Implementation of Unicode string that consists from runes.
   *
   * <h2>Memory model</h2>
   *
   * Rune string is implemented with copy-on-write memory model, meaning that
   * several instances of rune string might share the container of runes, which
   * might cause issues on threaded applications.
   */
  class runestring
  {
  public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef rune value_type;
    typedef rune& reference;
    typedef const rune& const_reference;
    typedef rune* pointer;
    typedef const rune* const_pointer;
    struct iterator;
    typedef iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    /**
     * Special value equal to maximum value representable by the
     * <code>size_type</code> type. Usually meaning as "no position",
     * it's value returned by search methods of rune string class
     * when it doesn't find the value being looked for.
     */
    static const size_type npos;

    /**
     * Constructs empty string.
     */
    explicit runestring();

    /**
     * Constructs copy of existing rune string.
     */
    runestring(const runestring& that);

    /**
     * Constructs rune string which contains <i>count</i> number of given rune.
     */
    runestring(size_type count, const_reference r);

    /**
     * Constructs rune string by copying <i>count</i> number of runes from
     * given array pointer.
     */
    runestring(const_pointer s, size_type count);

    /**
     * Constructs rune string from C type char array literal. <b>Note:</b> The
     * input is expected to be UTF-8 encoded.
     */
    runestring(const char* input);

    /**
     * Destructor.
     */
    virtual ~runestring();

    /**
     * Returns <code>true</code> if the string is not empty.
     */
    inline operator bool() const
    {
      return !!m_length;
    }

    /**
     * Returns <code>true</code> if the string is empty.
     */
    inline bool empty() const
    {
      return !m_length;
    }

    /**
     * Returns <code>true</code> if the string is either empty or contains only
     * whitespace characters.
     */
    bool blank() const;

    /**
     * Returns length of the string.
     */
    inline size_type length() const
    {
      return m_length;
    }

    /**
     * Returns reference to the first character in the string.
     *
     * \throw std::out_of_range If the string is empty
     */
    const_reference front() const;

    /**
     * Returns reference to the last character in the string.
     *
     * \throw std::out_of_range If the string is empty
     */
    const_reference back() const;

    /**
     * Returns reference to character from given index.
     *
     * \throw std::out_of_range If the given index is out of string limits
     */
    const_reference at(size_type pos) const;

    /**
     * Returns reference to character from given index. No boundary testing is
     * performed.
     */
    inline const_reference operator[](size_type pos) const
    {
      return m_runes[m_offset + pos];
    }

    /**
     * Returns iterator that points to the beginning of the string.
     */
    iterator begin() const;

    /**
     * Returns iterator that points past contents of the string.
     */
    iterator end() const;

    /**
     * Returns iterator that traverses through contents of the string in
     * reverse.
     */
    reverse_iterator rbegin() const;

    /**
     * Returns reverse iterator that points past contents of the string.
     */
    reverse_iterator rend() const;

    /**
     * Replaces contents of rune string with contents from another rune string.
     */
    runestring& assign(const runestring& that);

    /**
     * Assignment operator.
     */
    inline runestring& operator=(const runestring& that)
    {
      return assign(that);
    }

    /**
     * Tests whether contents of the rune string are equal with contents of
     * another rune string.
     */
    bool equals(const runestring& that) const;

    /**
     * Tests whether contents of the rune string are equal with contents of
     * another rune string, ignoring character case.
     */
    bool equals_icase(const runestring& that) const;

    /**
     * Equality testing operator.
     */
    inline bool operator==(const runestring& that) const
    {
      return equals(that);
    }

    /**
     * Non-equality testing operator.
     */
    inline bool operator!=(const runestring& that) const
    {
      return !equals(that);
    }

    /**
     * Compares contents of rune string against contents of another rune
     * string.
     */
    int compare(const runestring& that) const;

    /**
     * Compares contents of rune string against contents of another rune
     * string, ignoring character case.
     */
    int compare_icase(const runestring& that) const;

    /**
     * Comparison operator.
     */
    inline bool operator<(const runestring& that) const
    {
      return compare(that) < 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator>(const runestring& that) const
    {
      return compare(that) > 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator<=(const runestring& that) const
    {
      return compare(that) <= 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator>=(const runestring& that) const
    {
      return compare(that) >= 0;
    }

    /**
     * Concatenates contents of two rune strings and returns result.
     */
    runestring concat(const runestring& that) const;

    /**
     * Concatenates given rune at the end of the rune string and returns
     * result.
     */
    runestring concat(const_reference r) const;

    /**
     * Concatenation operator.
     */
    inline runestring operator+(const runestring& that) const
    {
      return concat(that);
    }

    /**
     * Concatenation operator.
     */
    inline runestring operator+(const_reference r) const
    {
      return concat(r);
    }

    /**
     * Strips whitespace from beginning and end of the rune string and returns
     * result.
     */
    runestring trim() const;

    /**
     * Returns substring beginning from given position with given length. If
     * length if omitted, the substring will continue to the end of the
     * original source string.
     */
    runestring substr(size_type pos = 0, size_type count = npos) const;

    /**
     * Converts rune string to lower case and returns result.
     */
    runestring to_lower() const;

    /**
     * Converts rune string to upper case and returns result.
     */
    runestring to_upper() const;

    /**
     * Encodes string with UTF-8 character encoding and returns it as byte
     * string.
     */
    std::string utf8() const;

    /**
     * Encodes string with UTF-16BE character encoding and returns it as byte
     * string.
     */
    std::string utf16_be() const;

    /**
     * Encodes string with UTF-16LE character encoding and returns it as byte
     * string.
     */
    std::string utf16_le() const;

    /**
     * Encodes string with UTF-32BE character encoding and returns it as byte
     * string.
     */
    std::string utf32_be() const;

    /**
     * Encodes string with UTF-32LE character encoding and returns it as byte
     * string.
     */
    std::string utf32_le() const;

    size_type find(const runestring& str, size_type pos = 0) const;

    size_type find(const_pointer s, size_type pos, size_type count) const;

    size_type find(const_reference needle, size_type pos = 0) const;

    size_type rfind(const runestring& str, size_type pos = npos) const;

    size_type rfind(const_pointer s, size_type pos, size_type count) const;

    size_type rfind(const_reference needle, size_type pos = npos) const;

    /**
     * Extracts all lines from the rune string and returns them in a vector of
     * substrings. All possible new line combinations are supported by this
     * method.
     */
    std::vector<runestring> lines() const;

    /**
     * Extracts all whitespace separated words from the rune string and returns
     * them in a vector of substrings.
     */
    std::vector<runestring> words() const;

  private:
    size_type m_offset;
    size_type m_length;
    pointer m_runes;
    size_type* m_counter;
  };

  /**
   * Iterator class that is capable of traversing through contents of rune
   * string.
   */
  struct runestring::iterator : public std::iterator<
    std::random_access_iterator_tag,
    value_type,
    difference_type,
    const_pointer,
    const_reference
  >
  {
  public:
    iterator();

    iterator(const iterator& that);

    iterator& operator=(const iterator& that);

    inline const_reference operator*() const
    {
      return *m_pointer;
    }

    inline const_pointer operator->() const
    {
      return m_pointer;
    }

    iterator& operator++();

    iterator operator++(int);

    iterator& operator--();

    iterator operator--(int);

    inline bool operator==(const_iterator& that) const
    {
      return m_pointer == that.m_pointer;
    }

    inline bool operator!=(const_iterator& that) const
    {
      return m_pointer != that.m_pointer;
    }

    inline bool operator<(const_iterator& that) const
    {
      return m_pointer < that.m_pointer;
    }

    inline bool operator>(const_iterator& that) const
    {
      return m_pointer > that.m_pointer;
    }

    inline bool operator<=(const_iterator& that) const
    {
      return m_pointer <= that.m_pointer;
    }

    inline bool operator>=(const_iterator& that) const
    {
      return m_pointer >= that.m_pointer;
    }

    inline const_reference operator[](size_type n) const
    {
      return m_pointer[n];
    }

    iterator operator+(size_type n) const;

    iterator operator-(size_type n) const;

    iterator& operator+=(size_type n);

    iterator& operator-=(size_type n);

    difference_type operator-(const iterator& that) const;

  private:
    pointer m_pointer;
    friend class runestring;
  };

  std::ostream& operator<<(std::ostream&, const runestring&);

  std::istream& getline(std::istream&, runestring&);
}

#endif /* !PEELO_TEXT_RUNESTRING_HPP_GUARD */
