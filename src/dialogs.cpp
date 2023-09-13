#define DIALOGS_CPP
#include <dialogs.hpp>

namespace dialogs {
    server_info_t request_server_info(GtkWindow* parent, server_info_t old_info) {
        // Mark dialog as running
        dialog_status = true;

        server_info_buffer.port = old_info.port;
        server_info_buffer.host = old_info.host;

        // Create dialog
        GtkWidget* dialog = gtk_dialog_new_with_buttons(
            "Enter server info",
            parent,
            GTK_DIALOG_MODAL,
            "Ok",
            GTK_RESPONSE_OK,
            "Cancell",
            GTK_RESPONSE_CANCEL,
            NULL
       );
        GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // Create input fields for IP and Port
        GtkWidget* ip_label = gtk_label_new("Host:");
        GtkWidget* ip_entry = gtk_entry_new();
        GtkWidget* port_label = gtk_label_new("Port:");
        GtkWidget* port_entry = gtk_entry_new();

        // Add labels and entry widgets to the content area
        gtk_container_add(GTK_CONTAINER(content_area), ip_label);
        gtk_container_add(GTK_CONTAINER(content_area), ip_entry);
        gtk_container_add(GTK_CONTAINER(content_area), port_label);
        gtk_container_add(GTK_CONTAINER(content_area), port_entry);

        // Set properties for the IP and Port entry widgets
        gtk_entry_set_max_length(GTK_ENTRY(ip_entry), 15); // Adjust the maximum IP length as needed
        gtk_entry_set_max_length(GTK_ENTRY(port_entry), 5); // Adjust the maximum port length as needed

        server_info_ip_entry = ip_entry;
        server_info_port_entry = port_entry;

        g_signal_connect(dialog, "response", G_CALLBACK(request_server_info_response), NULL);

        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));

        std::future<server_info_t> awaited_result = std::async(std::launch::async, await_server_info_input);
        server_info_t result = awaited_result.get();

        gtk_widget_destroy(dialog);
        return result;
    }
    void request_server_info_response(GtkDialog* dialog, gint response_id, gpointer user_data) {
        try {
            switch (response_id) {
            case GTK_RESPONSE_OK: {
                int port = std::stoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(server_info_port_entry))));
                std::string host = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(server_info_ip_entry)));

                server_info_buffer.host = host;
                server_info_buffer.port = port;

                dialog_status = false;
                
                break;
            }
            case GTK_RESPONSE_CANCEL:

                dialog_status = false;
                break;
            }
        }
        catch(const std::exception& e) {
            dialog_status = false;
        }
        
    }
    server_info_t await_server_info_input() {
        while (dialog_status) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        return server_info_buffer;
    }
}