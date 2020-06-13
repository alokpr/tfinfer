#ifndef TFINFER_TFBUFFER_H_
#define TFINFER_TFBUFFER_H_

#include <glib.h>
#include <tensorflow/c/c_api.h>

TF_Buffer* tf_buffer_from_file(const gchar* filepath, GError** error);

G_DEFINE_AUTOPTR_CLEANUP_FUNC(TF_Buffer, TF_DeleteBuffer);

#endif  // TFINFER_TFBUFFER_H_
