#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "include/logo_data.h"
#include "include/callbacks.h"
#include "include/config.h"
#include "include/proc.h"
#include "include/misc.h"

GtkWidget *main_window;
GtkWidget *notebook;
GtkWidget *hostname_entry;
pid_t pid;

int main(int argc, char **argv) {
	struct entry_data server_entries;
	GtkWidget *vbox;
	GtkWidget *vbox0;
	GtkWidget *hbox;
	GtkWidget *table;
	GtkWidget *page_label;
	GtkWidget *sep;
	GtkWidget *about_button;
	GtkWidget *start_button;
	GtkWidget *close_button;
	GtkWidget *image;
	GtkWidget *stock;
	GtkWidget *label;
	
	gtk_init(&argc, &argv);
	
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(main_window), "QuickSynergy");
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(main_window), 5);
	g_signal_connect(G_OBJECT(main_window), "delete_event",
		G_CALLBACK(app_quit), (gpointer) &server_entries);
	g_signal_connect(G_OBJECT(main_window), "destroy",
		G_CALLBACK(gtk_main_quit), (gpointer) &server_entries);
	
	vbox = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(main_window), vbox);
	
	notebook = gtk_notebook_new();
	
	table = gtk_table_new(3, 3, TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(table), 5);
	
	server_entries.above_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.above_entry), "Above");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.above_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.above_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.above_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Above");
	g_signal_connect(G_OBJECT(server_entries.above_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Above");
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.above_entry, 1, 2, 0, 1);
	
	server_entries.below_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.below_entry), "Below");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.below_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.below_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.below_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Below");
	g_signal_connect(G_OBJECT(server_entries.below_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Below");
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.below_entry, 1, 2, 2, 3);
	
	server_entries.left_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.left_entry), "Left");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.left_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.left_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.left_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Left");
	g_signal_connect(G_OBJECT(server_entries.left_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Left");
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.left_entry, 0, 1, 1, 2);
	
	server_entries.right_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(server_entries.right_entry), "Right");
	gtk_entry_set_alignment(GTK_ENTRY(server_entries.right_entry), 0.5);
	gtk_entry_set_width_chars(GTK_ENTRY(server_entries.right_entry), 10);
	g_signal_connect(G_OBJECT(server_entries.right_entry), "focus-in-event",
		G_CALLBACK(entry_focus_in_event), (gpointer) "Right");
	g_signal_connect(G_OBJECT(server_entries.right_entry), "focus-out-event",
		G_CALLBACK(entry_focus_out_event), (gpointer) "Right");
	gtk_table_attach_defaults(GTK_TABLE(table), server_entries.right_entry, 2, 3, 1, 2);
	
	image = gtk_image_new_from_pixbuf(
		gdk_pixbuf_new_from_xpm_data((const char **) logo_data));
	gtk_table_attach_defaults(GTK_TABLE(table), image, 1, 2, 1, 2);
	
	page_label = gtk_label_new("Server");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, page_label);
	
	vbox0 = gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox0), 5);
	
	label = gtk_label_new("Server hostname:");
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
	gtk_box_pack_start(GTK_BOX(vbox0), label, FALSE, FALSE, 0);
	
	hostname_entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox0), hostname_entry, FALSE, FALSE, 0);
	
	page_label = gtk_label_new("Client");
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox0, page_label);
	
	gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);
	
	sep = gtk_hseparator_new();
	
	gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 0);
	
	hbox =  gtk_hbox_new(TRUE, 0);
	
	about_button = gtk_button_new_with_label("About");
	stock = gtk_image_new_from_stock(GTK_STOCK_ABOUT, GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(about_button), stock);
	g_signal_connect(G_OBJECT(about_button), "clicked",
		G_CALLBACK(about_button_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), about_button, FALSE, FALSE, 0);
	
	start_button = gtk_button_new_with_label("Start");
	stock = gtk_image_new_from_stock(GTK_STOCK_EXECUTE, GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(start_button), stock);
	g_signal_connect(G_OBJECT(start_button), "clicked",
		G_CALLBACK(start_button_clicked), (gpointer) &server_entries);
	gtk_box_pack_start(GTK_BOX(hbox), start_button, FALSE, FALSE, 0);
	
	close_button = gtk_button_new_with_label("Close");
	stock = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image(GTK_BUTTON(close_button), stock);
	gtk_box_pack_start(GTK_BOX(hbox), close_button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(close_button), "clicked",
		G_CALLBACK(gtk_main_quit), (gpointer) &server_entries);
	
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	
	load_config(&server_entries);
	
	if((pid = get_process_by_name("synergys"))) {
		gtk_button_set_label(GTK_BUTTON(start_button), "Stop");
		stock = gtk_image_new_from_stock(GTK_STOCK_STOP, GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(start_button), stock);
		
		gtk_widget_set_sensitive(notebook, FALSE);
	}
	else if((pid = get_process_by_name("synergyc"))) {
		gtk_button_set_label(GTK_BUTTON(start_button), "Stop");
		stock = gtk_image_new_from_stock(GTK_STOCK_STOP, GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image(GTK_BUTTON(start_button), stock);
		
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
		
		gtk_widget_set_sensitive(notebook, FALSE);
	}
	
	gtk_widget_show_all(main_window);
	
	gtk_main();
	
	return 0;
}
