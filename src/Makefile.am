INCLUDES = $(GTK_CFLAGS) -DPACKAGE_LOCALE_DIR=\"$(datadir)/locale\"
LDADD = $(GTK_LIBS)

bin_PROGRAMS = quicksynergy

quicksynergy_SOURCES = proc.c synergy_config.c callbacks.c main.c screen_entry.c
quicksynergy_LDADD = $(LDADD) $(INTLLIBS)

noinst_HEADERS = proc.h synergy_config.h callbacks.c logo_data.h screen_entry.h
