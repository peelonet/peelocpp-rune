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
