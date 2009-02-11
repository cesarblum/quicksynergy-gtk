INCLUDES = $(GTK_CFLAGS)
LDADD = $(GTK_LIBS)

bin_PROGRAMS = quicksynergy

quicksynergy_SOURCES = proc.c synergy_config.c callbacks.c main.c screen_entry.c
quicksynergy_LDADD = @LIBINTL@ $(LDADD)

noinst_HEADERS = proc.h synergy_config.h callbacks.h screen_entry.h

localedir = $(datadir)/locale

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@
EXTRA_DIST = intl.h
