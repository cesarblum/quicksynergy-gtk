# inline logo

LOGO = qslogo.png

LOGO_PAIR = qslogo $(srcdir)/qslogo.png

qslogo.h: $(LOGO)
	gdk-pixbuf-csource --raw --extern --build-list $(LOGO_PAIR) > qslogo.h

noinst_DATA = qslogo.h
CLEANFILES = $(noinst_DATA)
