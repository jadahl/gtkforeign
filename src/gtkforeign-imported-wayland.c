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
#include "gtkforeign-imported.h"
#include "gtkforeign-imported-wayland.h"
#include "xdg-foreign-client-protocol.h"

struct _GtkForeignImportedWayland
{
  GtkForeignImported parent;

  struct _xdg_imported *xdg_imported;
  GtkForeignHandle *handle;
};

G_DEFINE_TYPE (GtkForeignImportedWayland,
               gtk_foreign_imported_wayland,
               GTK_FOREIGN_TYPE_IMPORTED);

static void
gtk_foreign_imported_wayland_set_parent_of (GtkForeignImported *imported,
                                            GdkWindow          *window)
{
  GtkForeignImportedWayland *imported_wayland =
    GTK_FOREIGN_IMPORTED_WAYLAND (imported);
  struct xdg_surface *xdg_surface;
  
  xdg_surface = gdk_wayland_window_get_xdg_surface (window);

  if (!xdg_surface)
    {
      g_warning ("Trying to set imported parent of non-xdg_surface window");
      return;
    }

  _xdg_imported_set_parent_of (imported_wayland->xdg_imported,
                               xdg_surface);
}

static void
gtk_foreign_imported_wayland_handle_destroyed (void                 *data,
                                               struct _xdg_imported *_xdg_imported)
{
  g_warning ("xdg_imported destroyed\n");
}

static const struct _xdg_imported_listener gtk_foreign_xdg_imported_listener = {
  gtk_foreign_imported_wayland_handle_destroyed,
};

GtkForeignImported *
gtk_foreign_imported_wayland_new (GtkForeign *foreign,
                                  struct _xdg_imported *xdg_imported)
{
  GdkDisplay *display;
  GtkForeignImportedWayland *imported_wayland;
  struct wl_display *wl_display;

  imported_wayland = g_object_new (GTK_FOREIGN_TYPE_IMPORTED_WAYLAND,
                                   "foreign", foreign,
                                   NULL);
  imported_wayland->xdg_imported = xdg_imported;


  _xdg_imported_add_listener (xdg_imported,
                              &gtk_foreign_xdg_imported_listener,
                              imported_wayland);

  display = gtk_foreign_get_gdk_display (foreign);
  wl_display = gdk_wayland_display_get_wl_display (display);
  wl_display_roundtrip (wl_display);

  return GTK_FOREIGN_IMPORTED (imported_wayland);
}

static void
gtk_foreign_imported_wayland_finalize (GObject *gobject)
{
  GtkForeignImportedWayland *imported_wayland =
    GTK_FOREIGN_IMPORTED_WAYLAND (gobject);

  _xdg_imported_destroy (imported_wayland->xdg_imported);

  G_OBJECT_CLASS (gtk_foreign_imported_wayland_parent_class)->finalize (gobject);
}

static void
gtk_foreign_imported_wayland_class_init (GtkForeignImportedWaylandClass *klass)
{
  GtkForeignImportedClass *imported_class = GTK_FOREIGN_IMPORTED_CLASS (klass);
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  imported_class->set_parent_of = gtk_foreign_imported_wayland_set_parent_of;

  gobject_class->finalize = gtk_foreign_imported_wayland_finalize;
}

static void
gtk_foreign_imported_wayland_init (GtkForeignImportedWayland *imported_wayland)
{
}
