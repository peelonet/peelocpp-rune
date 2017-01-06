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
#include <peelo/text/runestring.hpp>
#include <algorithm>
#include <stdexcept>

namespace peelo
{
  bool utf8_encode(char*, std::size_t&, rune::value_type);
  std::size_t utf8_decode_size(char);

  template<class CharT, class Traits>
  static bool utf8_decode(std::basic_ios<CharT, Traits>&, rune::value_type&);

  const runestring::size_type runestring::npos(-1);

  runestring::runestring()
    : m_offset(0)
    , m_length(0)
    , m_runes(nullptr)
    , m_counter(nullptr) {}

  runestring::runestring(const runestring& that)
    : m_offset(that.m_offset)
    , m_length(that.m_length)
    , m_runes(that.m_runes)
    , m_counter(that.m_counter)
  {
    if (m_counter)
    {
      ++(*m_counter);
    }
  }

  runestring::runestring(size_type count, const_reference r)
    : m_offset(0)
    , m_length(count)
    , m_runes(count ? new value_type[count] : nullptr)
    , m_counter(count ? new size_type(1) : nullptr)
  {
    for (size_type i = 0; i < m_length; ++i)
    {
      m_runes[m_offset + i] = r;
    }
  }

  runestring::runestring(const_pointer s, size_type count)
    : m_offset(0)
    , m_length(count)
    , m_runes(count ? new value_type[count] : nullptr)
    , m_counter(count ? new size_type(1) : nullptr)
  {
    if (count)
    {
      std::copy(s, s + count, m_runes + m_offset);
    }
  }

  runestring::runestring(const char* input)
    : m_offset(0)
    , m_length(0)
    , m_runes(nullptr)
    , m_counter(nullptr)
  {
    if (input)
    {
      for (const char* p = input; *p;)
      {
        const size_type size = utf8_decode_size(*p);

        if (size == 0)
        {
          break;
        }
        ++m_length;
        p += size;
      }
      if (m_length)
      {
        size_type index = 0;

        m_runes = new value_type[m_length];
        m_counter = new size_type(1);
        for (const char* p = input; *p;)
        {
          const size_type size = utf8_decode_size(*p);
          value_type::value_type result;
          bool is_valid = true;

          if (size == 0)
          {
            break;
          }
          switch (size)
          {
            case 1:
              result = *p;
              break;

            case 2:
              result = *p & 0x1f;
              break;

            case 3:
              result = *p & 0x0f;
              break;

            case 4:
              result = *p & 0x07;
              break;

            case 5:
              result = *p & 0x03;
              break;

            case 6:
              result = *p & 0x01;
              break;
          }
          for (size_type i = 1; i < size; ++i)
          {
            if ((p[i] & 0xc0) == 0x80)
            {
              result = (result << 6) | (p[i] & 0x3f);
            } else {
              is_valid = false;
              break;
            }
          }
          if (is_valid)
          {
            m_runes[m_offset + index++] = result;
            p += size;
          } else {
            break;
          }
        }
      }
    }
  }

  runestring::~runestring()
  {
    if (m_counter && !--(*m_counter))
    {
      delete[] m_runes;
      delete m_counter;
    }
  }

  bool runestring::blank() const
  {
    if (!m_length)
    {
      return true;
    }
    for (size_type i = 0; i < m_length; ++i)
    {
      if (!m_runes[m_offset + i].is_space())
      {
        return false;
      }
    }

    return true;
  }

  runestring::const_reference runestring::front() const
  {
    if (!m_length)
    {
      throw std::out_of_range("string is empty");
    }

    return m_runes[m_offset];
  }

  runestring::const_reference runestring::back() const
  {
    if (!m_length)
    {
      throw std::out_of_range("string is empty");
    }

    return m_runes[m_offset + m_length - 1];
  }

  runestring::const_reference runestring::at(size_type pos) const
  {
    if (m_length && pos < m_length)
    {
      return m_runes[m_offset + pos];
    }

    throw std::out_of_range("index out of bounds");
  }

  runestring::iterator runestring::begin() const
  {
    iterator i;

    if (m_runes)
    {
      i.m_pointer = m_runes + m_offset;
    }

    return i;
  }

  runestring::iterator runestring::end() const
  {
    iterator i;

    if (m_runes)
    {
      i.m_pointer = m_runes + m_offset + m_length;
    }

    return i;
  }

  runestring::reverse_iterator runestring::rbegin() const
  {
    return reverse_iterator(begin());
  }

  runestring::reverse_iterator runestring::rend() const
  {
    return reverse_iterator(end());
  }

  runestring& runestring::assign(const runestring& that)
  {
    if (m_runes != that.m_runes)
    {
      if (m_counter && !--(*m_counter))
      {
        delete[] m_runes;
        delete m_counter;
      }
      m_runes = that.m_runes;
      if ((m_counter = that.m_counter))
      {
        ++(*m_counter);
      }
    }
    m_offset = that.m_offset;
    m_length = that.m_length;

    return *this;
  }

  bool runestring::equals(const runestring& that) const
  {
    if (m_runes == that.m_runes)
    {
      return m_offset == that.m_offset && m_length == that.m_length;
    }
    else if (m_length != that.m_length)
    {
      return false;
    }
    for (size_type i = 0; i < m_length; ++i)
    {
      if (m_runes[m_offset + i] != that.m_runes[that.m_offset + i])
      {
        return false;
      }
    }

    return true;
  }

  bool runestring::equals_icase(const runestring& that) const
  {
    if (m_runes == that.m_runes)
    {
      return m_offset == that.m_offset && m_length == that.m_length;
    }
    else if (m_length != that.m_length)
    {
      return false;
    }
    for (size_type i = 0; i < m_length; ++i)
    {
      if (!m_runes[m_offset + i].equals_icase(that.m_runes[that.m_offset + i]))
      {
        return false;
      }
    }

    return true;
  }

  int runestring::compare(const runestring& that) const
  {
    if (m_runes != that.m_runes || m_offset != that.m_offset)
    {
      const size_type n = std::min(m_length, that.m_length);

      for (size_type i = 0; i < n; ++i)
      {
        const_reference a = m_runes[m_offset + i];
        const_reference b = that.m_runes[that.m_offset + i];

        if (a > b)
        {
          return 1;
        }
        else if (a < b)
        {
          return -1;
        }
      }
    }
    if (m_length > that.m_length)
    {
      return 1;
    }
    else if (m_length < that.m_length)
    {
      return -1;
    } else {
      return 0;
    }
  }

  int runestring::compare_icase(const runestring& that) const
  {
    if (m_runes != that.m_runes || m_offset != that.m_offset)
    {
      const size_type n = std::min(m_length, that.m_length);

      for (size_type i = 0; i < n; ++i)
      {
        const value_type a = m_runes[m_offset + i].to_lower();
        const value_type b = that.m_runes[that.m_offset + i].to_lower();

        if (a > b)
        {
          return 1;
        }
        else if (a < b)
        {
          return -1;
        }
      }
    }
    if (m_length > that.m_length)
    {
      return 1;
    }
    else if (m_length < that.m_length)
    {
      return -1;
    } else {
      return 0;
    }
  }

  runestring runestring::concat(const runestring& that) const
  {
    if (!m_length)
    {
      return that;
    }
    else if (!that.m_length)
    {
      return *this;
    } else {
      runestring result;

      result.m_length = m_length + that.m_length;
      result.m_runes = new value_type[result.m_length];
      result.m_counter = new size_type(1);
      std::copy(
        m_runes + m_offset,
        m_runes + m_offset + m_length,
        result.m_runes
      );
      std::copy(
        that.m_runes + that.m_offset,
        that.m_runes + that.m_offset + that.m_length,
        result.m_runes + m_length
      );

      return result;
    }
  }

  runestring runestring::concat(const_reference r) const
  {
    runestring result;

    result.m_length = m_length + 1;
    result.m_runes = new value_type[result.m_length];
    result.m_counter = new size_type(1);
    if (m_length)
    {
      std::copy(
        m_runes + m_offset,
        m_runes + m_offset + m_length,
        result.m_runes
      );
    }
    result.m_runes[m_length] = r;

    return result;
  }

  runestring runestring::trim() const
  {
    size_type i, j;

    for (i = 0; i < m_length; ++i)
    {
      if (!m_runes[m_offset + i].is_space())
      {
        break;
      }
    }
    for (j = m_length; j > 0; --j)
    {
      if (!m_runes[m_offset + j - 1].is_space())
      {
        break;
      }
    }
    if (i == 0 && j == m_length)
    {
      return *this;
    }

    return substr(i, j - i);
  }

  runestring runestring::substr(size_type pos, size_type count) const
  {
    runestring result;

    if (pos >= m_length)
    {
      return result;
    }
    else if (count == npos)
    {
      count = m_length - pos;
    }
    else if (count + pos > m_length)
    {
      count = m_length - pos;
    }
    result.m_offset = m_offset + pos;
    result.m_length = count;
    result.m_runes = m_runes;
    if ((result.m_counter = m_counter))
    {
      ++(*m_counter);
    }

    return result;
  }

  runestring runestring::to_lower() const
  {
    runestring result;

    if (m_length)
    {
      result.m_length = m_length;
      result.m_runes = new value_type[m_length];
      result.m_counter = new size_type(1);
      for (size_type i = 0; i < m_length; ++i)
      {
        result.m_runes[i] = m_runes[m_offset + i].to_lower();
      }
    }

    return result;
  }

  runestring runestring::to_upper() const
  {
    runestring result;

    if (m_length)
    {
      result.m_length = m_length;
      result.m_runes = new value_type[m_length];
      result.m_counter = new size_type(1);
      for (size_type i = 0; i < m_length; ++i)
      {
        result.m_runes[i] = m_runes[m_offset + i].to_upper();
      }
    }

    return result;
  }

  std::string runestring::utf8() const
  {
    std::string result;
    char buffer[5];

    for (size_type i = 0; i < m_length; ++i)
    {
      std::size_t size;

      if (utf8_encode(buffer, size, m_runes[m_offset + i]))
      {
        buffer[size] = 0;
        result += buffer;
      }
    }

    return result;
  }

  static std::string::size_type utf16_size(const runestring& str)
  {
    const runestring::size_type length = str.length();
    std::string::size_type size = 0;

    for (runestring::size_type i = 0; i < length; ++i)
    {
      if (str[i].code() > 0xffff)
      {
        size += 4;
      } else {
        size += 2;
      }
    }

    return size;
  }

  std::string runestring::utf16_be() const
  {
    std::string result;

    result.reserve(utf16_size(*this));
    for (size_type i = 0; i < m_length; ++i)
    {
      const rune::value_type c = m_runes[m_offset + i].code();

      if (c > 0xffff)
      {
        const rune::value_type plane = c >> 16;
        const rune::value_type high = (c & 0xff00) >> 8;

        result += static_cast<char>((plane >> 2) & 0xd8);
        result += static_cast<char>(((plane & 0x3) << 6) + (high >> 2));
        result += static_cast<char>((high & 0x2) + 0xdc);
        result += static_cast<char>(c & 0xff);
      } else {
        result += static_cast<char>((c & 0xff00) >> 8);
        result += static_cast<char>(c & 0xff);
      }
    }

    return result;
  }

  std::string runestring::utf16_le() const
  {
    std::string result;

    result.reserve(utf16_size(*this));
    for (size_type i = 0; i < m_length; ++i)
    {
      const rune::value_type c = m_runes[m_offset + i].code();

      if (c > 0xffff)
      {
        const rune::value_type plane = c >> 16;
        const rune::value_type high = (c & 0xff00) >> 8;

        result += static_cast<char>(((plane & 0x3) << 6) + (high >> 2));
        result += static_cast<char>((plane >> 2) & 0xd8);
        result += static_cast<char>(c & 0xff);
        result += static_cast<char>((high & 0x2) + 0xdc);
      } else {
        result += static_cast<char>(c & 0xff);
        result += static_cast<char>((c & 0xff00) >> 8);
      }
    }

    return result;
  }

  std::string runestring::utf32_be() const
  {
    std::string result;

    result.reserve(m_length * 4);
    for (size_type i = 0; i < m_length; ++i)
    {
      const rune::value_type c = m_runes[m_offset + i].code();

      result += static_cast<char>((c & 0xff000000) >> 24);
      result += static_cast<char>((c & 0xff0000) >> 16);
      result += static_cast<char>((c & 0xff00) >> 8);
      result += static_cast<char>(c & 0xff);
    }

    return result;
  }

  std::string runestring::utf32_le() const
  {
    std::string result;

    result.reserve(m_length * 4);
    for (size_type i = 0; i < m_length; ++i)
    {
      const rune::value_type c = m_runes[m_offset + i].code();

      result += static_cast<char>(c & 0xff);
      result += static_cast<char>((c & 0xff00) >> 8);
      result += static_cast<char>((c & 0xff0000) >> 16);
      result += static_cast<char>((c & 0xff000000) >> 24);
    }

    return result;
  }

  runestring::size_type runestring::find(const runestring& str,
                                         size_type pos) const
  {
    if (!str.m_length)
    {
      return pos;
    }
    else if (str.m_length > m_length)
    {
      return npos;
    }
    for (size_type i = pos; i < m_length; ++i)
    {
      bool found = true;

      if (i + str.m_length > m_length)
      {
        return npos;
      }
      for (size_type j = 0; j < str.m_length; ++j)
      {
        if (m_runes[m_offset + i + j] != str.m_runes[str.m_offset + j])
        {
          found = false;
          break;
        }
      }
      if (found)
      {
        return i;
      }
    }

    return npos;
  }

  runestring::size_type runestring::find(const_pointer s,
                                         size_type pos,
                                         size_type count) const
  {
    if (!count)
    {
      return pos;
    }
    else if (count > m_length)
    {
      return npos;
    }
    for (size_type i = pos; i < m_length; ++i)
    {
      bool found = true;

      if (i + count > m_length)
      {
        return npos;
      }
      for (size_type j = 0; j < count; ++j)
      {
        if (m_runes[m_offset + i + j] != s[j])
        {
          found = false;
          break;
        }
      }
      if (found)
      {
        return i;
      }
    }

    return npos;
  }

  runestring::size_type runestring::find(const_reference needle,
                                         size_type pos) const
  {
    while (pos < m_length)
    {
      if (m_runes[m_offset + pos] == needle)
      {
        return pos;
      }
      ++pos;
    }

    return npos;
  }

  runestring::size_type runestring::rfind(const runestring& str,
                                          size_type pos) const
  {
    if (pos == npos)
    {
      pos = m_length;
    }
    else if (pos > m_length || str.m_length > m_length)
    {
      return npos;
    }
    for (size_type i = m_length; i > 0; --i)
    {
      if (i > str.m_length)
      {
        bool found = true;

        for (size_type j = 0; j < str.m_length; ++j)
        {
          if (m_runes[m_offset + i - str.m_length + j - 1] != str.m_runes[str.m_offset + j])
          {
            found = false;
            break;
          }
        }
        if (found)
        {
          return i - str.m_length - 1;
        }
      }
    }

    return npos;
  }

  runestring::size_type runestring::rfind(const_pointer s,
                                          size_type pos,
                                          size_type count) const
  {
    if (pos == npos)
    {
      pos = m_length;
    }
    else if (pos > m_length || count > m_length)
    {
      return npos;
    }
    for (size_type i = m_length; i > 0; --i)
    {
      if (i > count)
      {
        bool found = true;

        for (size_type j = 0; j < count; ++j)
        {
          if (m_runes[m_offset + i - count + j - 1] != s[j])
          {
            found = false;
            break;
          }
        }
        if (found)
        {
          return i - count - 1;
        }
      }
    }

    return npos;
  }

  runestring::size_type runestring::rfind(const_reference needle,
                                          size_type pos) const
  {
    if (pos == npos)
    {
      pos = m_length;
    }
    else if (pos > m_length)
    {
      return npos;
    }
    for (size_type i = pos; i > 0; --i)
    {
      if (m_runes[m_offset + i - 1] == needle)
      {
        return i - 1;
      }
    }

    return npos;
  }

  std::vector<runestring> runestring::lines() const
  {
    std::vector<runestring> result;
    size_type begin = 0;
    size_type end = 0;

    for (size_type i = 0; i < m_length; ++i)
    {
      const_reference r = m_runes[m_offset + i];

      if (i + 1 < m_length
          && r.equals('\r')
          && m_runes[m_offset + i + 1].equals('\n'))
      {
        result.push_back(substr(begin, end - begin));
        begin = end = i + 2;
        ++i;
      }
      else if (r.equals('\n') || r.equals('\r'))
      {
        result.push_back(substr(begin, end - begin));
        begin = end = i + 1;
      } else {
        ++end;
      }
    }
    if (end - begin > 0)
    {
      result.push_back(substr(begin, end - begin));
    }

    return result;
  }

  std::vector<runestring> runestring::words() const
  {
    std::vector<runestring> result;
    size_type begin = 0;
    size_type end = 0;

    for (size_type i = 0; i < m_length; ++i)
    {
      const_reference r = m_runes[m_offset + i];

      if (r.is_space())
      {
        if (end - begin > 0)
        {
          result.push_back(substr(begin, end - begin));
        }
        begin = end = i + 1;
      } else {
        ++end;
      }
    }
    if (end - begin > 0)
    {
      result.push_back(substr(begin, end - begin));
    }

    return result;
  }

  runestring::iterator::iterator()
    : m_pointer(nullptr) {}

  runestring::iterator::iterator(const iterator& that)
    : m_pointer(that.m_pointer) {}

  runestring::iterator& runestring::iterator::operator=(const iterator& that)
  {
    m_pointer = that.m_pointer;

    return *this;
  }

  runestring::iterator& runestring::iterator::operator++()
  {
    ++m_pointer;

    return *this;
  }

  runestring::iterator runestring::iterator::operator++(int)
  {
    iterator return_value(*this);

    ++m_pointer;

    return return_value;
  }

  runestring::iterator& runestring::iterator::operator--()
  {
    --m_pointer;

    return *this;
  }

  runestring::iterator runestring::iterator::operator--(int)
  {
    iterator return_value(*this);

    --m_pointer;

    return return_value;
  }

  runestring::iterator runestring::iterator::operator+(size_type n) const
  {
    iterator return_value;

    return_value.m_pointer = m_pointer + n;

    return return_value;
  }

  runestring::iterator runestring::iterator::operator-(size_type n) const
  {
    iterator return_value;

    return_value.m_pointer = m_pointer - n;

    return return_value;
  }

  runestring::iterator& runestring::iterator::operator+=(size_type n)
  {
    m_pointer += n;

    return *this;
  }

  runestring::iterator& runestring::iterator::operator-=(size_type n)
  {
    m_pointer -= n;

    return *this;
  }

  runestring::difference_type runestring::iterator::operator-(const iterator& that) const
  {
    return m_pointer - that.m_pointer;
  }

  std::ostream& operator<<(std::ostream& os, const runestring& str)
  {
    os << str.utf8();

    return os;
  }

  std::istream& getline(std::istream& is, runestring& str)
  {
    std::istream::sentry sentry(is);

    if (sentry)
    {
      rune::value_type code;
      std::vector<rune> result;

      do
      {
        if (utf8_decode(is, code))
        {
          if (code == '\n')
          {
            break;
          } else {
            result.push_back(rune(code));
          }
        }
      }
      while (is.good());
      //str.assign();
    }

    return is;
  }

  template<class CharT, class Traits>
  static bool utf8_decode(std::basic_ios<CharT, Traits>& is,
                          rune::value_type& result)
  {
    int c = is.rdbuf()->sbumpc();
    std::size_t size;

    if (c == Traits::eof())
    {
      return false;
    }
    switch ((size = utf8_decode_size(c)))
    {
      case 0:
        is.setstate(std::basic_ios<CharT, Traits>::failbit);
        return false;

      case 1:
        result = c;
        break;

      case 2:
        result = c & 0x1f;
        break;

      case 3:
        result = c & 0x0f;
        break;

      case 4:
        result = c & 0x07;
        break;

      case 5:
        result = c & 0x03;
        break;

      case 6:
        result = c & 0x01;
        break;
    }
    for (std::size_t i = 1; i < size; ++i)
    {
      if ((c = is.rdbuf()->sbumpc()) == Traits::eof())
      {
        return false;
      }
      else if ((c & 0xc0) != 0x80)
      {
        is.setstate(std::basic_ios<CharT, Traits>::failbit);

        return false;
      }
      result = (result << 6) | (c & 0x3f);
    }

    return true;
  }
}
