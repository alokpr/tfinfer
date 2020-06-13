#include <stdio.h>

#include "tf_api.h"

#define MODEL_FILE "/usr/local/share/tfinfer/data/model.pb"

int main() {
  g_info("tensorFlow version: %s", TF_Version());

  g_autoptr(GError) error = NULL;
  g_autoptr(TF_Graph) graph = tf_graph_from_file(MODEL_FILE, &error);
  if (!graph) {
    g_error("failed to load graph: %s", error->message);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
