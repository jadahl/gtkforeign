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

#ifndef GTKFOREIGN_H
#define GTKFOREIGN_H

#include <gdk/gdk.h>

typedef struct _GtkForeign GtkForeign;
typedef struct _GtkForeignExported GtkForeignExported;
typedef struct _GtkForeignImported GtkForeignImported;
typedef struct _GtkForeignHandle GtkForeignHandle;

GtkForeign *         gtk_foreign_new                    (GdkDisplay         *display);

void                 gtk_foreign_free                   (GtkForeign         *foreign);

GtkForeignExported * gtk_foreign_export_window          (GtkForeign         *foreign,
                                                         GdkWindow          *window);

GtkForeignImported * gtk_foreign_import_window          (GtkForeign         *foreign,
                                                         GtkForeignHandle   *handle);

GtkForeignHandle *   gtk_foreign_exported_get_handle    (GtkForeignExported *exported);

void                 gtk_foreign_exported_destroy       (GtkForeignExported *exported);

void                 gtk_foreign_imported_set_parent_of (GtkForeignImported *imported,
                                                         GdkWindow          *window);

void                 gtk_foreign_imported_destroy       (GtkForeignImported *imported);

GtkForeignHandle *   gtk_foreign_handle_deserialize     (const gchar        *handle_str);

gchar *              gtk_foreign_handle_serialize       (GtkForeignHandle   *handle);

void                 gtk_foreign_handle_free            (GtkForeignHandle   *handle);

#endif /* GTKFOREIGN_H */
