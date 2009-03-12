INCLUDES = $(LIBXML_CFLAGS)
AM_CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter

bin_PROGRAMS=freetrail
freetraildir = src/
freetrail_SOURCES= \
                   common.cc \
                   journey.cc \
                   location.cc \
                   main.cc \
                   party.cc \
                   runners.cc \
                   ui.cc \
                   world.cc
freetrail_HEADERS = \
                    common.hh \
                    journey.hh \
                    location.hh \
                    main.hh \
                    party.hh \
                    runners.hh \
                    ui.hh \
                    world.hh

freetrail_LDADD = $(LIBXML_LIBS)
