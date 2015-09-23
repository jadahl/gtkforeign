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

#include <gtk/gtk.h>
#include <gtkforeign/gtkforeign.h>
#include <string.h>

typedef struct _GtkForeignExporter
{
  GtkWindow *window;
  GtkForeign *foreign;

  GtkForeignExported *pending_exported;
  GList *exported_list;
} GtkForeignExporter;

enum
{
  TARGET_IMPORTER,
  TARGET_ROOTWIN,
};

static GtkTargetEntry target_table[] = {
  { "application/x-xdg-foreign-handle", GTK_TARGET_OTHER_APP, TARGET_IMPORTER },
  { "application/x-rootwindow-drop", 0, TARGET_ROOTWIN }
};

static void
source_drag_begin (GtkWidget          *widget,
                   GdkDragContext     *context,
                   GtkForeignExporter *exporter)
{
  GdkWindow *gdk_window;
  GtkForeignExported *exported;

  gdk_window = gtk_widget_get_window (GTK_WIDGET (exporter->window));
  exported = gtk_foreign_export_window (exporter->foreign, gdk_window);
  if (!exported)
    {
      g_warning ("Failed to export window\n");
      return;
    }

  exporter->pending_exported = exported;
}

static void
source_drag_end (GtkWidget          *widget,
                 GdkDragContext     *context,
                 GtkForeignExporter *exporter)
{
  if (exporter->pending_exported)
    {
      gtk_foreign_exported_destroy (exporter->pending_exported);
      exporter->pending_exported = NULL;
    }
}

static void
source_drag_data_delete (GtkWidget          *widget,
                         GdkDragContext     *context,
                         GtkForeignExporter *exporter)
{
  exporter->exported_list = g_list_append (exporter->exported_list,
                                           exporter->pending_exported);
  exporter->pending_exported = NULL;
}

static void
source_drag_data_get  (GtkWidget          *widget,
		       GdkDragContext     *context,
		       GtkSelectionData   *selection_data,
		       guint               info,
		       guint               time,
		       gpointer            data)
{
  GtkForeignExporter *exporter = data;
  GtkForeignHandle *handle;
  gchar *handle_str;

  if (info != TARGET_IMPORTER)
    return;

  handle = gtk_foreign_exported_get_handle (exporter->pending_exported);

  handle_str = gtk_foreign_handle_serialize (handle);
  gtk_selection_data_set (selection_data,
                          gtk_selection_data_get_target (selection_data),
                          8, (guchar *) handle_str, strlen (handle_str));
  g_free (handle_str);
}

int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkForeignExporter exporter;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "GtkForeign Exporter");
  gtk_window_set_default_size (GTK_WINDOW (window),
                               800, 600);
  g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);

  exporter = (GtkForeignExporter) {
    .window = GTK_WINDOW (window),
    .foreign = gtk_foreign_new (gdk_display_get_default()),
  };

  button = gtk_button_new_with_label ("Drag to importer to export\n");
  gtk_drag_source_set (button, GDK_BUTTON1_MASK,
		       target_table,
                       sizeof (target_table) / sizeof (*target_table),
                       GDK_ACTION_MOVE);
  g_signal_connect (button, "drag_begin",
                    G_CALLBACK (source_drag_begin),
                    &exporter);
  g_signal_connect (button, "drag_data_get",
                    G_CALLBACK (source_drag_data_get),
                    &exporter);
  g_signal_connect (button, "drag_data_delete",
                    G_CALLBACK (source_drag_data_delete),
                    &exporter);
  g_signal_connect (button, "drag_end",
                    G_CALLBACK (source_drag_end),
                    &exporter);

  gtk_container_add (GTK_CONTAINER (window), button);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
