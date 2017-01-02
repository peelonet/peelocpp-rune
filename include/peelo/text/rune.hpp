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
#ifndef PEELO_TEXT_RUNE_HPP_GUARD
#define PEELO_TEXT_RUNE_HPP_GUARD

#include <cstdint>
#include <string>
#include <iostream>

namespace peelo
{
  /**
   * The Rune class represents single 32-bit Unicode character.
   */
  class rune
  {
  public:
    typedef std::uint32_t value_type;

    /** Minimum value for rune (zero). */
    static const rune min;
    /** Maximum value for rune (0x10ffff). */
    static const rune max;

    /**
     * Constructs rune which code point is zero.
     */
    rune();

    /**
     * Constructs rune from given code point.
     *
     * \throws std::out_of_range If given code point is too large
     */
    explicit rune(value_type code);

    /**
     * Copy constructor.
     */
    rune(const rune& that);

    /**
     * Returns <code>true</code> if rune value is zero.
     */
    inline bool operator!() const
    {
      return !m_code;
    }

    /**
     * Returns the Unicode code point which this class represents.
     */
    inline value_type code() const
    {
      return m_code;
    }

    /**
     * Casting operator that casts the rune into integer.
     */
    inline operator int() const
    {
      return m_code;
    }

    /**
     * Assigns code point from another rune into this one.
     */
    rune& assign(const rune& that);

    /**
     * Replaces code point of the rune with another code point.
     *
     * \throws std::out_of_range If given code point is too large
     */
    rune& assign(value_type code);

    /**
     * Assignment operator.
     */
    inline rune& operator=(const rune& that)
    {
      return assign(that);
    }

    /**
     * Assignment operator.
     *
     * \throws std::out_of_range If given code point is too large
     */
    inline rune& operator=(value_type code)
    {
      return assign(code);
    }

    /**
     * Tests whether two runes are equal or not.
     */
    bool equals(const rune& that) const;

    /**
     * Tests whether rune's code point is equal with given code point.
     */
    bool equals(value_type code) const;

    /**
     * Tests whether two runes are equal, ignoring character case.
     */
    bool equals_icase(const rune& that) const;

    /**
     * Tests whether rune's code point is equal with given code point, ignoring
     * character case.
     */
    bool equals_icase(value_type code) const;

    /**
     * Equality testing operator.
     */
    inline bool operator==(const rune& that) const
    {
      return equals(that);
    }

    /**
     * Equality testing operator.
     */
    inline bool operator==(value_type code) const
    {
      return equals(code);
    }

    /**
     * Non-equality testing operator.
     */
    inline bool operator!=(const rune& that) const
    {
      return !equals(that);
    }

    /**
     * Non-equality testing operator.
     */
    inline bool operator!=(value_type code) const
    {
      return !equals(code);
    }

    int compare(const rune& that) const;

    int compare(value_type code) const;

    int compare_icase(const rune& that) const;

    int compare_icase(value_type code) const;

    /**
     * Comparison operator.
     */
    inline bool operator<(const rune& that) const
    {
      return compare(that) < 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator<(value_type code) const
    {
      return compare(code) < 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator>(const rune& that) const
    {
      return compare(that) > 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator>(value_type code) const
    {
      return compare(code) > 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator<=(const rune& that) const
    {
      return compare(that) <= 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator<=(value_type code) const
    {
      return compare(code) <= 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator>=(const rune& that) const
    {
      return compare(that) >= 0;
    }

    /**
     * Comparison operator.
     */
    inline bool operator>=(value_type code) const
    {
      return compare(code) >= 0;
    }

    /**
     * Returns lower case equivalent of given code point if it's upper
     * case.
     */
    static value_type to_lower(value_type code);

    /**
     * Returns lower case equivalent if character is upper case.
     */
    rune to_lower() const;

    /**
     * Returns upper case equivalent of given code point if it's lower
     * case.
     */
    static value_type to_upper(value_type code);

    /**
     * Returns upper case equivalent if character is lower case.
     */
    rune to_upper() const;

    /**
     * Returns <code>true</code> if given code point is alphanumeric
     * character.
     */
    static bool is_alnum(value_type code);

    /**
     * Returns <code>true</code> if rune is alphanumeric character.
     */
    bool is_alnum() const;

    /**
     * Returns <code>true</code> if given code point is alphabetic
     * character.
     */
    static bool is_alpha(value_type code);

    /**
     * Returns <code>true</code> if rune is alphabetic character.
     */
    bool is_alpha() const;

    /**
     * Returns <code>true</code> if given code point is in ASCII range.
     */
    static bool is_ascii(value_type code);

    /**
     * Returns <code>true</code> if rune is in ASCII range.
     */
    bool is_ascii() const;

    /**
     * Returns <code>true</code> if given code point is a blank character. A
     * blank character is a space character used to separate words within a
     * line of text.
     */
    static bool is_blank(value_type code);

    /**
     * Returns <code>true</code> if rune is a blank character. A blank
     * character is a space character used to separate words within a line of
     * text.
     */
    bool is_blank() const;

    /**
     * Returns <code>true</code> if given code point is control sequence.
     */
    static bool is_cntrl(value_type code);

    /**
     * Returns <code>true</code> if rune is control sequence.
     */
    bool is_cntrl() const;

    /**
     * Returns <code>true</code> if given code point is ASCII decimal digit
     * character.
     */
    static bool is_digit(value_type code);

    /**
     * Returns <code>true</code> if rune is ASCII decimal digit character.
     */
    bool is_digit() const;

    /**
     * Returns <code>true</code> if given code point is any printing
     * character, except space and other locale specific space like
     * characters.
     */
    static bool is_graph(value_type code);

    /**
     * Returns <code>true</code> if rune is any printing character, except
     * space and other locale specific space like characters.
     */
    bool is_graph() const;

    /**
     * Returns <code>true</code> if given code point is lower case letter.
     */
    static bool is_lower(value_type code);

    /**
     * Returns <code>true</code> if rune is lower case letter.
     */
    bool is_lower() const;

    /**
     * Returns <code>true</code> if given code point is any number
     * character in any locale.
     */
    static bool is_number(value_type code);

    /**
     * Returns <code>true</code> if rune is any number character in any
     * locale.
     */
    bool is_number() const;

    /**
     * Returns <code>true</code> if given code point is any printable
     * character, including space.
     */
    static bool is_print(value_type code);

    /**
     * Returns <code>true</code> if rune is any printable character,
     * including space.
     */
    bool is_print() const;

    /**
     * Returns <code>true</code> if given code point is any punctuation
     * character, except space or alphanumeric character.
     */
    static bool is_punct(value_type code);

    /**
     * Returns <code>true</code> if rune is any punctuation character,
     * except space or alphanumeric character.
     */
    bool is_punct() const;

    /**
     * Returns <code>true</code> if given code point is whitespace
     * character.
     */
    static bool is_space(value_type code);

    /**
     * Returns <code>true</code> if rune is whitespace character.
     */
    bool is_space() const;

    /**
     * Returns <code>true</code> if given code point is upper case letter.
     */
    static bool is_upper(value_type code);

    /**
     * Returns <code>true</code> if rune is upper case letter.
     */
    bool is_upper() const;

    /**
     * Returns <code>true</code> if given code point is a word character.
     */
    static bool is_word(value_type code);

    /**
     * Returns <code>true</code> if rune is a word character.
     */
    bool is_word() const;

    /**
     * Returns <code>true</code> if given code point is hexadecimal
     * character.
     */
    static bool is_xdigit(value_type code);

    /**
     * Returns <code>true</code> if rune is hexadecimal character.
     */
    bool is_xdigit() const;

    /**
     * Encodes rune with UTF-8 character encoding and returns result.
     */
    std::string utf8() const;

    /**
     * Increments rune by one.
     */
    rune& operator++();

    /**
     * Increments rune by one.
     */
    rune operator++(int);

    /**
     * Decrements rune by one.
     */
    rune& operator--();

    /**
     * Decrements rune by one.
     */
    rune operator--(int);

  private:
    /** Unicode code point which this class represents. */
    value_type m_code;
  };

  /**
   * Converts the rune into UTF-8 and writes it into given stream.
   */
  std::ostream& operator<<(std::ostream&, const rune&);
}

#endif /* !PEELO_TEXT_RUNE_HPP_GUARD */
