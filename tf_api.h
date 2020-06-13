#ifndef TFINFER_TF_API_H_
#define TFINFER_TF_API_H_

#include <glib.h>
#include <tensorflow/c/c_api.h>

G_DEFINE_AUTOPTR_CLEANUP_FUNC(TF_Buffer, TF_DeleteBuffer);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(TF_Graph, TF_DeleteGraph);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(TF_ImportGraphDefOptions,
                              TF_DeleteImportGraphDefOptions);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(TF_Status, TF_DeleteStatus);

TF_Buffer* tf_buffer_from_file(const gchar* filepath, GError** error);
TF_Graph* tf_graph_from_file(const gchar* filepath, GError** error);

#endif  // TFINFER_TF_API_H_
