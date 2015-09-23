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

#include "gtkforeign-impl.h"
#include "gtkforeign-impl-wayland.h"
#include "gtkforeign-private.h"

typedef struct _GtkForeignImplPrivate
{
  GtkForeign *foreign;
} GtkForeignImplPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GtkForeignImpl, gtk_foreign_impl, G_TYPE_OBJECT);

GtkForeignImpl *
gtk_foreign_create_impl (GtkForeign *foreign)
{
  GtkForeignImpl *impl = NULL;
  GtkForeignImplPrivate *priv;

  if (GDK_IS_WAYLAND_DISPLAY (gtk_foreign_get_gdk_display (foreign)))
    impl = g_object_new (GTK_FOREIGN_TYPE_IMPL_WAYLAND, NULL);

  if (!impl)
    {
      g_error ("Unsupported display type\n");
      return NULL;
    }

  priv = gtk_foreign_impl_get_instance_private (impl);
  priv->foreign = foreign;

  return impl;
}

GtkForeignExported *
gtk_foreign_impl_export_window (GtkForeignImpl   *impl,
                                GdkWindow        *window)
{
  return GTK_FOREIGN_IMPL_GET_CLASS (impl)->export_window (impl, window);
}

GtkForeignImported *
gtk_foreign_impl_import_window (GtkForeignImpl   *impl,
                                GtkForeignHandle *handle)
{
  return GTK_FOREIGN_IMPL_GET_CLASS (impl)->import_window (impl, handle);
}

GtkForeign *
gtk_foreign_impl_get_foreign (GtkForeignImpl *impl)
{
  GtkForeignImplPrivate *priv = gtk_foreign_impl_get_instance_private (impl);

  return priv->foreign;
}

static void
gtk_foreign_impl_class_init (GtkForeignImplClass *klass)
{
}

static void
gtk_foreign_impl_init (GtkForeignImpl *impl)
{
}
