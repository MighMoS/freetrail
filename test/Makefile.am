INCLUDES = $(LIBCPPTEST_CFLAGS) $(LIBXML_CFLAGS)
AM_CXXFLAGS = -Wall -Wextra -I$(top_srcdir)/src

## The libraries all tests use
TEST_LIBS = $(top_builddir)/src/libfreetrail.a \
                $(LIBXML_LIBS) \
                $(LIBCPPTEST_LIBS)

TESTS = test_party test_map
check_PROGRAMS = test_party test_map

test_party_SOURCES = test_party.cc
test_party_LDADD = $(TEST_LIBS)

test_map_SOURCES = test_map.cc
test_map_LDADD = $(TEST_LIBS)
