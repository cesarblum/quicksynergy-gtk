AC_INIT([QuickSynergy], [0.9.0], [cesarbs@gmail.com])

AM_INIT_AUTOMAKE(quicksynergy, 0.9.0)

AC_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR([src/main.c])

AC_PROG_CC

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

AC_C_CONST
AC_TYPE_PID_T

AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gethostname mkdir])

PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.10.0])
PKG_CHECK_MODULES([GTK], [glib-2.0 >= 2.16.0])

AM_GNU_GETTEXT_VERSION([0.16])
AM_GNU_GETTEXT([external])

AC_CONFIG_FILES([
    Makefile
    po/Makefile.in
    src/logo/Makefile
    src/Makefile])

AC_OUTPUT
