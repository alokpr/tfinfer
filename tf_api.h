#ifndef TFINFER_TF_API_H_
#define TFINFER_TF_API_H_

#include <glib.h>
#include <tensorflow/c/c_api.h>

G_DEFINE_AUTOPTR_CLEANUP_FUNC(TF_Buffer, TF_DeleteBuffer);

TF_Buffer* tf_buffer_from_file(const gchar* filepath, GError** error);

#endif  // TFINFER_TF_API_H_
