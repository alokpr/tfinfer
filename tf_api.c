#include "tf_api.h"

G_DEFINE_QUARK(tf-error, tf_error)
#define TF_ERROR (tf_error_quark())

static void buffer_free(void* data, size_t length) { g_free(data); }

TF_Buffer* tf_buffer_from_file(const gchar* filepath, GError** error) {
  gchar* contents = NULL;
  gsize length = 0;
  if (!g_file_get_contents(filepath, &contents, &length, error)) {
    return NULL;
  }

  TF_Buffer* buffer = TF_NewBuffer();
  buffer->data = contents;
  buffer->length = length;
  buffer->data_deallocator = buffer_free;
  return buffer;
}

TF_Graph* tf_graph_from_file(const gchar* filepath, GError** error) {
  g_autoptr(TF_Buffer) graph_def = tf_buffer_from_file(filepath, error);
  if (!graph_def) return NULL;

  g_autoptr(TF_Status) status = TF_NewStatus();
  g_autoptr(TF_ImportGraphDefOptions) options = TF_NewImportGraphDefOptions();
  g_autoptr(TF_Graph) graph = TF_NewGraph();
  TF_GraphImportGraphDef(graph, graph_def, options, status);

  TF_Code status_code = TF_GetCode(status);
  if (status_code != TF_OK) {
    *error = g_error_new_literal(TF_ERROR, status_code, TF_Message(status));
    return NULL;
  }
  return g_steal_pointer(&graph);
}
