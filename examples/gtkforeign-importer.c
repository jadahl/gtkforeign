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

typedef struct _GtkForeignImporter
{
  GtkWindow *window;
  GtkWidget *label;
  GtkWidget *button;

  GtkForeign *foreign;
  GtkForeignImported *imported;
} GtkForeignImporter;

enum
{
  TARGET_IMPORTER,
};

static GtkTargetEntry target_table[] = {
  { "application/x-xdg-foreign-handle", GTK_TARGET_OTHER_APP, TARGET_IMPORTER },
};

static void
label_drag_data_received  (GtkWidget          *widget,
                           GdkDragContext     *context,
                           gint                x,
                           gint                y,
                           GtkSelectionData   *selection_data,
                           guint               info,
                           guint               time,
                           GtkForeignImporter *importer)
{
  if (gtk_selection_data_get_length (selection_data) >= 0 &&
      gtk_selection_data_get_format (selection_data) == 8)
    {
      gchar *handle_str;
      GtkForeignHandle *handle;
      GtkForeignImported *imported;
      GdkWindow *gdk_window;

      handle_str = (gchar *) gtk_selection_data_get_data (selection_data);
      handle = gtk_foreign_handle_deserialize (handle_str);
      imported = gtk_foreign_import_window (importer->foreign, handle);

      gdk_window = gtk_widget_get_window (GTK_WIDGET (importer->window));

      gtk_foreign_imported_set_parent_of (imported, gdk_window);
      gdk_window_set_modal_hint (gdk_window, TRUE);

      gtk_container_remove (GTK_CONTAINER (importer->window), importer->label);
      gtk_container_add (GTK_CONTAINER (importer->window), importer->button);

      importer->imported = imported;

      gtk_drag_finish (context, TRUE, FALSE, time);
      return;
    }

  gtk_drag_finish (context, TRUE, TRUE, time);
}

static gboolean
button_press_event (GtkWidget          *widget,
                    GdkEvent           *event,
                    GtkForeignImporter *importer)
{
  GdkWindow *gdk_window;

  gdk_window = gtk_widget_get_window (GTK_WIDGET (importer->window));
  gdk_window_set_modal_hint (gdk_window, FALSE);

  gtk_foreign_imported_destroy (importer->imported);
  importer->imported = NULL;

  gtk_container_remove (GTK_CONTAINER (importer->window), importer->button);
  gtk_container_add (GTK_CONTAINER (importer->window), importer->label);

  return TRUE;
}

int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *label;
  GtkWidget *button;
  GtkForeignImporter importer;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "GtkForeign Importer");
  gtk_window_set_default_size (GTK_WINDOW (window),
                               640, 480);
  g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);

  label = gtk_label_new ("Drop here to import");
  gtk_drag_dest_set (label,
		     GTK_DEST_DEFAULT_ALL,
		     target_table,
                     sizeof (target_table) / sizeof (*target_table),
		     GDK_ACTION_MOVE);
  g_signal_connect (label, "drag_data_received",
		    G_CALLBACK (label_drag_data_received),
                    &importer);

  button = gtk_button_new_with_label ("Click here to disassociated");
  g_signal_connect (button, "button-press-event",
                    G_CALLBACK (button_press_event),
                    &importer);

  importer = (GtkForeignImporter) {
    .window = GTK_WINDOW (window),
    .label = g_object_ref (label),
    .button = g_object_ref (button),
    .foreign = gtk_foreign_new (gdk_display_get_default()),
  };

  gtk_container_add (GTK_CONTAINER (window), label);

  gtk_widget_show (label);
  gtk_widget_show (button);
  gtk_widget_show (window);

  gtk_main ();

  g_object_unref (importer.label);
  g_object_unref (importer.button);

  return 0;
}
