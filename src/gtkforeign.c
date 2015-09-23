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

#include "gtkforeign.h"
#include "gtkforeign-impl.h"
#include "gtkforeign-private.h"

struct _GtkForeign
{
  GdkDisplay *display;
  GtkForeignImpl *impl;
};

struct _GtkForeignHandle
{
  gchar *handle_str;
};

GtkForeign *
gtk_foreign_new (GdkDisplay *display)
{
  GtkForeign *foreign;

  foreign = g_new0 (GtkForeign, 1);

  foreign->display = display;
  foreign->impl = gtk_foreign_create_impl (foreign);

  return foreign;
}

void
gtk_foreign_free (GtkForeign *foreign)
{
  g_object_unref (foreign->impl);
  g_free (foreign);
}

GtkForeignExported *
gtk_foreign_export_window (GtkForeign *foreign,
                           GdkWindow  *window)
{
  return gtk_foreign_impl_export_window (foreign->impl, window);
}

GtkForeignImported *
gtk_foreign_import_window (GtkForeign       *foreign,
                           GtkForeignHandle *handle)
{
  return gtk_foreign_impl_import_window (foreign->impl, handle);
}

GdkDisplay *
gtk_foreign_get_gdk_display (GtkForeign *foreign)
{
  return foreign->display;
}

GtkForeignHandle *
gtk_foreign_handle_deserialize (const gchar *handle_str)
{
  GtkForeignHandle *handle;

  handle = g_new0 (GtkForeignHandle, 1);
  handle->handle_str = g_strdup (handle_str);

  return handle;
}

gchar *
gtk_foreign_handle_serialize (GtkForeignHandle *handle)
{
  return g_strdup (handle->handle_str);
}

void
gtk_foreign_handle_free (GtkForeignHandle *handle)
{
  g_free (handle->handle_str);
  g_free (handle);
}
