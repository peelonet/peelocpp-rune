ADD_LIBRARY(
  peelocpp_text
  rune.cpp
  runestring.cpp
  utf8.cpp
)
INSTALL(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/peelo
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp"
)
INSTALL(
  TARGETS peelocpp_text
  ARCHIVE
  DESTINATION lib
)
