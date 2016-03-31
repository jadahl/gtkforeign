NULL=
lib_LTLIBRARIES = libgtkforeign.la

BUILT_SOURCES =					\
	src/xdg-foreign-unstable-v1-client-protocol.h	\
	src/xdg-foreign-unstable-v1-protocol.c		\
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
	src/gtkforeign-imported.c		\
	src/gtkforeign-imported.h		\
	src/gtkforeign-imported-wayland.c	\
	src/gtkforeign-imported-wayland.h	\
	src/gtkforeign-private.h		\
	$(NULL)

libgtkforeign_la_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)
libgtkforeign_la_LIBADD = $(GTK_LIBS)

.SECONDEXPANSION:

define protostability
$(shell echo $1 | sed 's/.*\(\<unstable\>\|\<stable\>\).*/\1/')
endef

define protoname
$(shell echo $1 | sed 's/\([a-z\-]\+\)-[a-z]\+-v[0-9]\+/\1/')
endef

%-protocol.c : $(WAYLAND_PROTOCOLS_DATADIR)/$$(call protostability,$$*)/$$(call protoname,$$*)/$$*.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@
	$(AM_V_GEN)$(SED) -i -e 's/WL_EXPORT //' $@
%-client-protocol.h : $(WAYLAND_PROTOCOLS_DATADIR)/$$(call protostability,$$*)/$$(call protoname,$$*)/$$*.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) client-header < $< > $@

%-protocol.c : $(srcdir)/protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@
	$(AM_V_GEN)$(SED) -i -e 's/WL_EXPORT //' $@

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
