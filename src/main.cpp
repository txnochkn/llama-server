#include "events.cpp"
#include "dialogs.cpp"

void update_server_info(GtkWidget* label) {
    server_info_t new_server_info = dialogs::request_server_info(GTK_WINDOW(window), {server_info.host, server_info.port});
    server_info = {new_server_info.host, new_server_info.port};
    gtk_label_set_text(GTK_LABEL(label), ("Selected server: "+server_info.host+":"+std::to_string(server_info.port)).c_str());
}

void add_message_to_list_box(std::string message, std::string author, GdkPixbuf* pfp_pixbuf) {
    // Create containers
    GtkWidget* message_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget* message_pfp_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget* inner_message_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget* message_author_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget* message_content_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    
    gtk_container_set_border_width(GTK_CONTAINER(message_container), 20);
    gtk_widget_set_vexpand(message_pfp_container, TRUE);
    gtk_widget_set_valign(message_pfp_container, GTK_ALIGN_FILL);

    // Create widgets
    GtkWidget* pfp_image = gtk_image_new_from_pixbuf(pfp_pixbuf);
    GtkWidget* author_label = gtk_label_new((author+":").c_str());
    PangoAttrList* message_author_label_attr_list = pango_attr_list_from_string(
        "0 -1 weight bold, 0 -1 font-desc \"Sans 15\""
    );
    gtk_label_set_attributes(GTK_LABEL(author_label), message_author_label_attr_list);
    GtkWidget* message_content = gtk_label_new(message.c_str());
    // Add widgets and containers to their containers
    gtk_container_add(GTK_CONTAINER(message_author_container), message_pfp_container);
    gtk_container_add(GTK_CONTAINER(message_container), inner_message_container);
    gtk_container_add(GTK_CONTAINER(inner_message_container), message_author_container);
    gtk_container_add(GTK_CONTAINER(inner_message_container), message_content_container);

    gtk_container_add(GTK_CONTAINER(message_pfp_container), pfp_image);
    gtk_container_add(GTK_CONTAINER(message_author_container), author_label);
    gtk_container_add(GTK_CONTAINER(message_content_container), message_content);
    
    gtk_list_box_insert(GTK_LIST_BOX(message_list_box), message_container, -1);

    gtk_widget_show_all(window);
}

int main() {
    gtk_init(0,0);

    user_icon_pixbuf = gdk_pixbuf_new_from_file("user.png", NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "AI Client");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    // Create a vertical box to hold the widgets
    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget* version_label = gtk_label_new("Client version: 0.1a");
    gtk_container_add(GTK_CONTAINER(vbox), version_label);
    GtkWidget* server_info_label = gtk_label_new(("Selected server: "+server_info.host+":"+std::to_string(server_info.port)).c_str());
    gtk_container_add(GTK_CONTAINER(vbox), server_info_label);

    // Create a GtkScrolledWindow
    GtkWidget* scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledWindow, TRUE, TRUE, 0);

    // Create a GtkListBox
    message_list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(message_list_box), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), message_list_box);

    GtkWidget* bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_end(GTK_BOX(vbox), bottom_box, FALSE, FALSE, 0);
    gtk_widget_set_size_request(bottom_box, -1, 50);

    user_input_entry = gtk_entry_new();
    GtkWidget* user_input_entry_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(bottom_box), user_input_entry_scrolled_window);
    gtk_widget_set_hexpand(user_input_entry_scrolled_window, TRUE);
    gtk_widget_set_halign(user_input_entry_scrolled_window, GTK_ALIGN_FILL);
    gtk_container_add(GTK_CONTAINER(user_input_entry_scrolled_window), user_input_entry);

    GtkWidget* send_button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(bottom_box), send_button);
    gtk_button_set_label(GTK_BUTTON(send_button), "Send!");
    gtk_widget_set_halign(send_button, GTK_ALIGN_FILL);
    

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(user_input_entry, "key-press-event", G_CALLBACK(on_entry_key_press), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}