INCLUDES = $(GTK_CFLAGS)
LDADD = $(GTK_LIBS)

bin_PROGRAMS = quicksynergy

quicksynergy_SOURCES = synergy_config.c callbacks.c main.c
quicksynergy_LDADD = @LIBINTL@ $(LDADD)

noinst_HEADERS = synergy_config.h callbacks.h

localedir = $(datadir)/locale

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
EXTRA_DIST = intl.h
