AC_PREREQ([2.63])
AC_INIT([gtkforeign], [0.1], [jadahl@gmail.com])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.13 foreign no-define tar-ustar no-dist-gzip dist-xz subdir-objects])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])
AC_USE_SYSTEM_EXTENSIONS
AC_SYS_LARGEFILE

AC_PROG_CC
AM_PROG_CC_C_O

GETTEXT_PACKAGE=gtkforeign
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
		   [The prefix for our gettext translation domains.])

AX_APPEND_COMPILE_FLAGS([-Wall -Wstrict-prototypes -Werror=missing-prototypes \
		-Werror=implicit-function-declaration \
		-Werror=pointer-arith -Werror=init-self -Werror=format=2 \
		-Werror=format-security \
		-Werror=missing-include-dirs -Werror=aggregate-return \
		-Werror=declaration-after-statement])

AC_CHECK_FUNCS(posix_fallocate)

# Initialize libtool
LT_PREREQ([2.2.4])
LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(GTK, gtk+-3.0 wayland-server wayland-client wayland-protocols)

AC_PATH_PROG([WAYLAND_SCANNER],[wayland-scanner],[no])
AS_IF([test "x$WAYLAND_SCANNER" = "xno"],
  AC_MSG_ERROR([Could not find wayland-scanner in your PATH, required for parsing wayland extension protocols]))
AC_SUBST([WAYLAND_SCANNER])

AC_SUBST(WAYLAND_PROTOCOLS_DATADIR, `$PKG_CONFIG --variable=pkgdatadir wayland-protocols`)

AC_CONFIG_FILES([
Makefile
])
AC_OUTPUT
