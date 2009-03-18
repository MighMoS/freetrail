INCLUDES = $(LIBXML_CFLAGS)
AM_CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -DFREETRAIL_MAPSDIR=\"$(mapsdir)\"

bin_PROGRAMS=freetrail
freetrail_SOURCES= \
                   common.cc \
                   location.cc \
                   main.cc \
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

freetrail_LDADD = $(LIBXML_LIBS)
