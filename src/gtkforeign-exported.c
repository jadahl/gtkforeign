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

#include <glib.h>

#include "gtkforeign-exported.h"

enum
{
  PROP_0,
  PROP_FOREIGN,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

typedef struct _GtkForeignExportedPrivate
{
  GtkForeign *foreign;
} GtkForeignExportedPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GtkForeignExported, gtk_foreign_exported,
			    G_TYPE_OBJECT);

GtkForeignHandle *
gtk_foreign_exported_get_handle (GtkForeignExported *exported)
{
  return GTK_FOREIGN_EXPORTED_GET_CLASS (exported)->get_handle (exported);
}

void
gtk_foreign_exported_destroy (GtkForeignExported *exported)
{
  g_object_unref (exported);
}

GtkForeign *
gtk_foreign_exported_get_foreign (GtkForeignExported *exported)
{
  GtkForeignExportedPrivate *priv =
    gtk_foreign_exported_get_instance_private (exported);

  return priv->foreign;
}

static void
gtk_foreign_exported_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  GtkForeignExported *exported = GTK_FOREIGN_EXPORTED (object);
  GtkForeignExportedPrivate *priv =
    gtk_foreign_exported_get_instance_private (exported);

  switch (prop_id)
    {
    case PROP_FOREIGN:
      priv->foreign = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_foreign_exported_class_init (GtkForeignExportedClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = gtk_foreign_exported_set_property;

  obj_properties[PROP_FOREIGN] =
    g_param_spec_pointer ("foreign", "Foreign", "Foreign",
                          G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (gobject_class,
                                     N_PROPERTIES,
                                     obj_properties);
}

static void
gtk_foreign_exported_init (GtkForeignExported *exported)
{
}
