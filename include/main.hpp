#ifndef MAIN_HPP
#define MAIN_HPP

#include <gtk/gtk.h>
#include <string>

struct server_info_t {
  std::string host;
  int port;  
};

// Global variables
server_info_t server_info = {"127.0.0.1", 5000};
GtkWidget* window;
GtkWidget* user_input_entry;
GtkWidget* message_list_box;
std::string username = "TechnoChicken";
GdkPixbuf* user_icon_pixbuf;

void add_message_to_list_box(std::string message, std::string author, GdkPixbuf* pfp_pixbuf);

#endif