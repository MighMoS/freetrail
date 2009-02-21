INCLUDES = $(LIBXML_CFLAGS)
AM_CXXFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter

bin_PROGRAMS=freetrail
freetrail_SOURCES=journey.cc \
		location.cc \
		  main.cc \
		  party.cc \
		  runners.cc \
		  ui.cc \
		  world.cc

freetrail_LDADD = $(LIBXML_LIBS)
