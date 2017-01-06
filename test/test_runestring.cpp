#include <peelo/text/runestring.hpp>
#include <cassert>

using peelo::rune;
using peelo::runestring;

int main()
{
  assert(runestring().length() == 0);
  assert(runestring().empty());
  assert(runestring().blank());
  assert(runestring("\t ").blank());
  assert(runestring("a").front() == rune('a'));
  assert(runestring("abc").back() == rune('c'));
  assert(runestring("\xc3\xb6h\xc3\xb6").at(1) == rune('h'));
  assert(runestring("\xc3\xb6h\xc3\xb6")[1] == rune('h'));
  assert(runestring().assign("foo").equals("foo"));
  assert(runestring("a\xc3\xa4").equals_icase("A\xc3\x84"));
  assert(runestring().assign("foo").compare("foo") == 0);
  assert(runestring("a\xc3\xa4").compare_icase("A\xc3\x84") == 0);
  assert(runestring("a").concat("b") == "ab");
  assert(runestring("\t a\r\n ").trim() == "a");
  assert(runestring("abc").substr(1) == "bc");
  assert(runestring("abc").substr(1, 2) == "bc");
  assert(runestring("abc").substr(1, 5) == "bc");
  assert(runestring("abc").substr(1, 1) == "b");
  assert(runestring("\xc3\xa4").to_upper() == "\xc3\x84");
  assert(runestring("\xc3\x84").to_lower() == "\xc3\xa4");
  assert(runestring("a\xc3\x84").utf8() == "a\xc3\x84");

  assert(runestring("abc").find("bc") == 1);
  assert(runestring("abcdef").find("de", 3) == 3);
  assert(runestring("abc").find("d") == runestring::npos);
  assert(runestring("abc").find(rune('c')) == 2);
  assert(runestring("abc").find(rune('d')) == runestring::npos);

  assert(runestring("a").lines().size() == 1);
  assert(runestring("a\n\nb\r\nc\rd").lines().size() == 5);
  assert(runestring("a").words().size() == 1);
  assert(runestring("\ta  b\t c").words().size() == 3);

  return 0;
}
