#include <events.hpp>
#include <main.hpp>

gboolean on_entry_key_press(GtkWidget* entry, GdkEventKey* event, gpointer user_data) {
    if (event->keyval == GDK_KEY_Return && (event->state & GDK_SHIFT_MASK)) {
        // Shift+Enter detected, insert a newline character
        int pos = gtk_editable_get_position(GTK_EDITABLE(entry));
        gtk_entry_buffer_insert_text(gtk_entry_get_buffer(GTK_ENTRY(entry)), pos, "\n", 1);
        gtk_editable_set_position(GTK_EDITABLE(entry), pos+1);
        return TRUE;
    }
    else if (event->keyval == GDK_KEY_Return) {
        add_message_to_list_box(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entry))), username, user_icon_pixbuf);
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(entry)), "", 0);
    }
    return FALSE; // Allow default key press handling
}