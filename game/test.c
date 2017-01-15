#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkBuilder *gtkBuilder;
    GtkWidget *window;
    gtk_init(&argc, &argv);
    
    gtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder, "proj.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(gtkBuilder, "mywindow"));
    
    GtkLabel *label;
    label = GTK_LABEL(gtk_builder_get_object(gtkBuilder, "label1" ));
    gtk_label_set_label(label, "This is text");
    
    g_object_unref(G_OBJECT(gtkBuilder));
    gtk_widget_show(window);
    gtk_main();
    
    return 0;
}
