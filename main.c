#include "config.h"
#include "tf_api.h"

int main() {
  g_info("tensorFlow version: %s", TF_Version());
  g_info("model file: %s", MODEL_FILE);

  g_autoptr(GError) error = NULL;
  g_autoptr(TF_Graph) graph = tf_graph_from_file(MODEL_FILE, &error);
  if (!graph) {
    g_error("failed to load graph: %s", error->message);
    return EXIT_FAILURE;
  }

  g_autoptr(TF_Session) session = tf_session_new(graph, &error);
  if (!session) {
    g_error("failed to create session: %s", error->message);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
