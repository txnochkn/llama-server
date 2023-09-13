#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <string>
#include <thread>
#include <future>
#include <gtk/gtk.h>

namespace dialogs {
  #ifdef DIALOGS_CPP

  bool dialog_status;
  server_info_t server_info_buffer;
  GtkWidget* server_info_ip_entry;
  GtkWidget* server_info_port_entry;

  #endif

  server_info_t request_server_info(GtkWindow* parent, server_info_t old_info);
  void request_server_info_response(GtkDialog* dialog, gint response_id, gpointer user_data);
  server_info_t await_server_info_input();
}

#endif