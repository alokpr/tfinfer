#include "tf_api.h"

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
