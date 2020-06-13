#include <stdio.h>

#include "tf_api.h"

#define MODEL_FILE "/usr/local/share/tfinfer/data/model.pb"

int main() {
  g_info("tensorFlow version: %s", TF_Version());

  g_autoptr(GError) error = NULL;
  g_autoptr(TF_Buffer) graph_def = tf_buffer_from_file(MODEL_FILE, &error);
  if (!graph_def) {
    g_error("failed to read model file: %s", error->message);
    return error->code;
  }

  return 0;
}
