AM_CPPFLAGS = $(LIBXML_CFLAGS)
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter \
              -DFREETRAIL_MAPSDIR=\"$(mapsdir)\" \
              $(CXXDEBUG)

bin_PROGRAMS=freetrail
freetrail_SOURCES= \
                   main.cc
noinst_LIBRARIES = \
                   libfreetrail.a
libfreetrail_a_SOURCES = \
                   common.cc \
                   location.cc \
                   party.cc \
                   parser.cc \
                   runners.cc \
                   ui.cc \
                   world.cc
noinst_HEADERS = \
                    common.hh \
                    location.hh \
                    parser.hh \
                    party.hh \
                    runners.hh \
                    ui.hh \
                    world.hh

freetrail_LDADD = libfreetrail.a $(LIBXML_LIBS)
