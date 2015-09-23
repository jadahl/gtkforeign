NULL=
lib_LTLIBRARIES = libgtkforeign.la

EXTRA_DIST = \
	protocol/xdg-foreign.xml		\
	protocol/xdg-shell.xml			\
	$(NULL)

BUILT_SOURCES =					\
	src/xdg-foreign-client-protocol.h	\
	src/xdg-foreign-protocol.c		\
	src/xdg-shell-client-protocol.h		\
	src/xdg-shell-protocol.c		\
	$(NULL)

DISTCLEANFILES =				\
	$(BUILT_SOURCES)			\
	$(NULL)

libgtkforeignincludedir = $(includedir)/gtkforeign

libgtkforeigninclude_HEADERS =			\
	src/gtkforeign.h			\
	$(NULL)

nodist_libgtkforeign_la_SOURCES =		\
	$(BUILT_SOURCES)			\
	$(NULL)

libgtkforeign_la_SOURCES =			\
	src/gtkforeign.c			\
	src/gtkforeign-exported.c		\
	src/gtkforeign-exported.h		\
	src/gtkforeign-exported-wayland.c	\
	src/gtkforeign-exported-wayland.h	\
	src/gtkforeign-impl.c			\
	src/gtkforeign-impl.h			\
	src/gtkforeign-impl-wayland.c		\
	src/gtkforeign-impl-wayland.h		\
	src/gtkforeign-private.h		\
	$(NULL)

libgtkforeign_la_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)
libgtkforeign_la_LIBADD = $(GTK_LIBS)

src/%-protocol.c : $(srcdir)/protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@
	$(AM_V_GEN)$(SED) -i -e 's/WL_EXPORT //' $@

src/%-client-protocol.h : $(srcdir)/protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) client-header < $< > $@

noinst_PROGRAMS =				\
	gtkforeign-exporter			\
	gtkforeign-importer			\
	$(NULL)

gtkforeign_exporter_LDADD = libgtkforeign.la

gtkforeign_exporter_SOURCES =			\
	examples/gtkforeign-exporter.c		\
	$(NULL)

gtkforeign_exporter_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)

gtkforeign_importer_LDADD = libgtkforeign.la

gtkforeign_importer_SOURCES =			\
	examples/gtkforeign-importer.c		\
	$(NULL)

gtkforeign_importer_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)
