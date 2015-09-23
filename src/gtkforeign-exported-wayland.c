/*
 * Copyright (C) 2015 Red Hat Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by:
 *     Jonas Ådahl <jadahl@gmail.com>
 */

#include "config.h"

#include <gdk/gdkwayland.h>

#include "gtkforeign-private.h"
#include "gtkforeign-exported.h"
#include "gtkforeign-exported-wayland.h"
#include "xdg-foreign-client-protocol.h"

struct _GtkForeignExportedWayland
{
  GtkForeignExported parent;

  struct _xdg_exported *xdg_exported;
  GtkForeignHandle *handle;
};

G_DEFINE_TYPE (GtkForeignExportedWayland,
               gtk_foreign_exported_wayland,
               GTK_FOREIGN_TYPE_EXPORTED);

static GtkForeignHandle *
gtk_foreign_exported_wayland_get_handle (GtkForeignExported *exported)
{
  GtkForeignExportedWayland *exported_wayland =
    GTK_FOREIGN_EXPORTED_WAYLAND (exported);

  return exported_wayland->handle;
}

static void
gtk_foreign_exported_wayland_handle_handle (void                 *data,
                                            struct _xdg_exported *_xdg_exported,
                                            const char           *handle)
{
  GtkForeignExportedWayland *exported_wayland = data;

  exported_wayland->handle = gtk_foreign_handle_deserialize (handle);
}

static const struct _xdg_exported_listener gtk_foreign_xdg_exported_listener = {
  gtk_foreign_exported_wayland_handle_handle,
};

GtkForeignExported *
gtk_foreign_exported_wayland_new (GtkForeign *foreign,
                                  struct _xdg_exported *xdg_exported)
{
  GdkDisplay *display;
  GtkForeignExportedWayland *exported_wayland;
  struct wl_display *wl_display;

  exported_wayland = g_object_new (GTK_FOREIGN_TYPE_EXPORTED_WAYLAND,
                                   "foreign", foreign,
                                   NULL);
  exported_wayland->xdg_exported = xdg_exported;


  _xdg_exported_add_listener (xdg_exported,
                              &gtk_foreign_xdg_exported_listener,
                              exported_wayland);

  display = gtk_foreign_get_gdk_display (foreign);
  wl_display = gdk_wayland_display_get_wl_display (display);
  wl_display_roundtrip (wl_display);

  return GTK_FOREIGN_EXPORTED (exported_wayland);
}

static void
gtk_foreign_exported_wayland_finalize (GObject *gobject)
{
  GtkForeignExportedWayland *exported_wayland =
    GTK_FOREIGN_EXPORTED_WAYLAND (gobject);

  _xdg_exported_destroy (exported_wayland->xdg_exported);

  G_OBJECT_CLASS (gtk_foreign_exported_wayland_parent_class)->finalize (gobject);
}

static void
gtk_foreign_exported_wayland_class_init (GtkForeignExportedWaylandClass *klass)
{
  GtkForeignExportedClass *exported_class = GTK_FOREIGN_EXPORTED_CLASS (klass);
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  exported_class->get_handle = gtk_foreign_exported_wayland_get_handle;

  gobject_class->finalize = gtk_foreign_exported_wayland_finalize;
}

static void
gtk_foreign_exported_wayland_init (GtkForeignExportedWayland *exported_wayland)
{
}
