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

  return 0;
}
