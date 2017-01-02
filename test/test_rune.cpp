#include <peelo/text/rune.hpp>
#include <cassert>

int main()
{
  peelo::rune auml(0x00e4);

  assert(auml.equals(0x00e4));
  assert(auml.equals_icase(0x00c4));
  assert(auml.compare('a') > 0);
  assert(auml.compare(0x00e4) == 0);
  assert(auml.compare(0x00f6) < 0);
  assert(auml.compare_icase('A') > 0);
  assert(auml.compare_icase(0x00c4) == 0);
  assert(auml.compare_icase(0x00f6) < 0);

  assert(peelo::rune::to_upper(0x00e4) == 0x00c4);
  assert(peelo::rune::to_lower(0x00c4) == 0x00e4);

  assert(peelo::rune::is_alnum('5'));
  assert(peelo::rune::is_alpha('a'));
  assert(peelo::rune::is_ascii('s'));
  assert(peelo::rune::is_blank(' '));
  assert(peelo::rune::is_cntrl('\t'));
  assert(peelo::rune::is_digit('9'));
  assert(peelo::rune::is_graph('a'));
  assert(peelo::rune::is_lower('a'));
  assert(peelo::rune::is_number('5'));
  assert(peelo::rune::is_print(' '));
  assert(peelo::rune::is_punct('-'));
  assert(peelo::rune::is_space('\t'));
  assert(peelo::rune::is_upper('A'));
  assert(peelo::rune::is_word('a'));
  assert(peelo::rune::is_xdigit('a'));

  auml = peelo::rune::min;
  --auml;
  assert(auml == peelo::rune::max);

  auml = peelo::rune::max;
  ++auml;
  assert(auml == peelo::rune::min);

  assert(peelo::rune(0x00e4).utf8() == "\xc3\xa4");
  assert(peelo::rune(0x00f6).utf8() == "\xc3\xb6");
  assert(peelo::rune(0x5047).utf8() == "\xe5\x81\x87");

  return 0;
}
