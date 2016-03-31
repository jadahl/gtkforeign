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

#ifndef GTKFOREIGN_IMPORTED_WAYLAND_H
#define GTKFOREIGN_IMPORTED_WAYLAND_H

#include "gtkforeign-imported.h"

#include "xdg-foreign-unstable-v1-client-protocol.h"

#define GTK_FOREIGN_TYPE_IMPORTED_WAYLAND (gtk_foreign_imported_wayland_get_type ())
G_DECLARE_FINAL_TYPE (GtkForeignImportedWayland,
                      gtk_foreign_imported_wayland,
                      GTK_FOREIGN, IMPORTED_WAYLAND,
                      GtkForeignImported);

GtkForeignImported * gtk_foreign_imported_wayland_new (GtkForeign              *foreign,
                                                       struct zxdg_imported_v1 *xdg_imported);

#endif /* GTKFOREIGN_IMPORTED_WAYLAND_H */
