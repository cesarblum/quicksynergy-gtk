# inline logo

LOGO = qslogo.png

LOGO_PAIR = qslogo $(srcdir)/qslogo.png

qslogo.h:
	gdk-pixbuf-csource --raw --extern --build-list $(LOGO_PAIR) > qslogo.h

EXTRA_DIST = qslogo.png
noinst_DATA = qslogo.h
CLEANFILES = $(noinst_DATA)
