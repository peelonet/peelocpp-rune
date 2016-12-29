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
#include <peelo/text/rune.hpp>
#include <stdexcept>

namespace peelo
{
  bool utf8_encode(char*, std::size_t&, rune::value_type);

  static const rune::value_type max_code_point = 0x10ffff;

  const rune rune::min(0);
  const rune rune::max(max_code_point);

  rune::rune()
    : m_code(0) {}

  rune::rune(value_type code)
    : m_code(code)
  {
    if (code > max_code_point)
    {
      throw std::out_of_range("code point too large");
    }
  }

  rune::rune(const rune& that)
    : m_code(that.m_code) {}

  rune& rune::assign(const rune& that)
  {
    m_code = that.m_code;

    return *this;
  }

  rune& rune::assign(value_type code)
  {
    if (code > max_code_point)
    {
      throw std::out_of_range("code point too large");
    }
    m_code = code;

    return *this;
  }

  bool rune::equals(const rune& that) const
  {
    return equals(that.m_code);
  }

  bool rune::equals(value_type code) const
  {
    return m_code == code;
  }

  bool rune::equals_icase(const rune& that) const
  {
    return equals_icase(that.m_code);
  }

  bool rune::equals_icase(value_type code) const
  {
    const value_type a = to_lower(m_code);
    const value_type b = to_lower(code);

    return a == b;
  }

  int rune::compare(const rune& that) const
  {
    return compare(that.m_code);
  }

  int rune::compare(value_type code) const
  {
    return m_code > code ? 1 : m_code < code ? -1 : 0;
  }

  int rune::compare_icase(const rune& that) const
  {
    return compare_icase(that.m_code);
  }

  int rune::compare_icase(value_type code) const
  {
    const value_type a = to_lower(m_code);
    const value_type b = to_lower(code);

    return a > b ? 1 : a < b ? -1 : 0;
  }

  rune::value_type rune::to_lower(value_type c)
  {
    if (c >= 'A' && c <= 'Z')
    {
      return c + 32;
    }
    if (c >= 0x00C0)
    {
      if ((c >= 0x00C0 && c <= 0x00D6) || (c >= 0x00D8 && c <= 0x00DE))
      {
        return c + 32;
      }
      else if ((c >= 0x0100 && c < 0x0138) || (c > 0x0149 && c < 0x0178))
      {
        if (c == 0x0130)
        {
          return 0x0069;
        }
        else if ((c & 1) == 0)
        {
          return c + 1;
        }
      }
      else if (c == 0x0178)
      {
        return 0x00FF;
      }
      else if ((c >= 0x0139 && c < 0x0149) || (c > 0x0178 && c < 0x017F))
      {
        if (c & 1)
        {
          return c + 1;
        }
      }
      else if (c >= 0x0200 && c <= 0x0217)
      {
        if ((c & 1) == 0)
        {
          return c + 1;
        }
      }
      else if ((c >= 0x0401 && c <= 0x040C) || (c >= 0x040E && c <= 0x040F))
      {
        return c + 80;
      }
      else if (c >= 0x410 && c <= 0x042F)
      {
        return c + 32;
      }
      else if (c >= 0x0460 && c <= 0x047F)
      {
        if ((c & 1) == 0)
        {
          return c + 1;
        }
      }
      else if (c >= 0x0531 && c <= 0x0556)
      {
        return c + 48;
      }
      else if (c >= 0x10A0 && c <= 0x10C5)
      {
        return c + 48;
      }
      else if (c >= 0xFF21 && c <= 0xFF3A)
      {
        return c + 32;
      }
    }

    return c;
  }

  rune rune::to_lower() const
  {
    return rune(to_lower(m_code));
  }

  rune::value_type rune::to_upper(value_type c)
  {
    if (c >= 'a' && c <= 'z')
    {
        return c - 32;
    }
    if (c >= 0x00E0)
    {
      if ((c >= 0x00E0 && c <= 0x00F6) || (c >= 0x00F8 && c <= 0x00FE))
      {
        return c - 32;
      }
      else if (c == 0x00FF)
      {
        return 0x0178;
      }
      else if ((c >= 0x0100 && c < 0x0138) || (c > 0x0149 && c < 0x0178))
      {
        if (c == 0x0131)
        {
          return 0x0049;
        }
        else if (c & 1)
        {
          return c - 1;
        }
      }
      else if ((c >= 0x0139 && c < 0x0149) || (c > 0x0178 && c < 0x017F))
      {
        if ((c & 1) == 0)
        {
          return c - 1;
        }
      }
      else if (c == 0x017F)
      {
        return 0x0053;
      }
      else if (c >= 0x0200 && c <= 0x0217)
      {
        if (c & 1)
        {
          return c - 1;
        }
      }
      else if (c >= 0x0430 && c <= 0x044F)
      {
        return c - 32;
      }
      else if ((c >= 0x0451 && c <= 0x045C) || (c >= 0x045E && c <= 0x045F))
      {
        return c - 80;
      }
      else if (c >= 0x0460 && c <= 0x047F)
      {
        if (c & 1)
        {
          return c - 1;
        }
      }
      else if (c >= 0x0561 && c < 0x0587)
      {
        return c - 48;
      }
      else if (c >= 0xFF41 && c <= 0xFF5A)
      {
        return c - 32;
      }
    }

    return c;
  }

  rune rune::to_upper() const
  {
    return rune(to_upper(m_code));
  }

  std::string rune::utf8() const
  {
    char buffer[4];
    std::size_t size;

    if (utf8_encode(buffer, size, m_code))
    {
      buffer[size] = 0;

      return buffer;
    }

    return std::string();
  }

  rune& rune::operator++()
  {
    if (m_code == max_code_point)
    {
      m_code = 0;
    } else {
      ++m_code;
    }

    return *this;
  }

  rune rune::operator++(int)
  {
    const rune return_value(*this);

    if (m_code == max_code_point)
    {
      m_code = 0;
    } else {
      ++m_code;
    }

    return return_value;
  }

  rune& rune::operator--()
  {
    if (m_code == 0)
    {
      m_code = max_code_point;
    } else {
      --m_code;
    }

    return *this;
  }

  rune rune::operator--(int)
  {
    const rune return_value(*this);

    if (m_code == 0)
    {
      m_code = max_code_point;
    } else {
      --m_code;
    }

    return return_value;
  }

  std::ostream& operator<<(std::ostream& os, const class rune& rune)
  {
    os << rune.utf8();

    return os;
  }
}
