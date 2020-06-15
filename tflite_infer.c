#include "config.h"
#include "tflite_api.h"

int main() {
  g_info("tensorflow lite version: %s", TfLiteVersion());

  g_autoptr(TfLiteModel) model = TfLiteModelCreateFromFile(TFLITE_MODEL_FILE);
  if (!model) {
    g_error("TfLiteModelCreateFromFile failed: %s", TFLITE_MODEL_FILE);
    return EXIT_FAILURE;
  }

  g_autoptr(TfLiteInterpreter) interpreter =
      TfLiteInterpreterCreate(model, NULL);
  if (!interpreter) {
    g_error("TfLiteInterpreterCreate failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
