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
