INCLUDES = $(LIBXML_INCLUDES) $(LIBCPPTEST_INCLUDES)
AM_CXXFLAGS = -Wall -Wextra -I$(top_srcdir)/src \
              $(LIBXML_CFLAGS) $(LIBCPPTEST_CFLAGS)

TESTS = test_party
check_PROGRAMS=test_party

test_party_SOURCES = \
                     test_party.cc
test_party_LDADD = \
                   $(top_srcdir)/src/libfreetrail.a \
                   $(LIBXML_LIBS) $(LIBCPPTEST_LIBS)

