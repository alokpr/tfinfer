#include "config.h"
#include "image.h"
#include "tflite_api.h"

static void error_cb(void* user_data, const char* format, va_list args) {
  g_error(format, args);
}

/*
static void tensor_info(const TfLiteTensor* tensor) {
  int ndims = TfLiteTensorNumDims(tensor);
  g_info("name=%s type=%d dims=%d bytes=%lu", TfLiteTensorName(tensor),
         TfLiteTensorType(tensor), ndims, TfLiteTensorByteSize(tensor));
  for (int i = 0; i < ndims; ++i) {
    g_info("dim %d: %d", i, TfLiteTensorDim(tensor, i));
  }
}
*/

enum OutputTensor {
  kOutputTensorLocations,
  kOutputTensorClasses,
  kOutputTensorScores,
  kOutputTensorDetections
};

gboolean load_input(const Image* input_img, TfLiteInterpreter* interpreter,
                    GError** error) {
  g_assert_cmpint(1, ==, TfLiteInterpreterGetInputTensorCount(interpreter));

  TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(interpreter, 0);
  g_autoptr(Image) output_img = image_from_tensor(input_tensor, error);
  if (!output_img) return FALSE;

  int width, height;
  image_size(output_img, &width, &height);
  image_resize(input_img, width, height, output_img);
  image_swap_rb(output_img, output_img);
  return TRUE;
}

void print_output(TfLiteInterpreter* interpreter) {
  g_assert_cmpint(4, ==, TfLiteInterpreterGetOutputTensorCount(interpreter));

  const TfLiteTensor* dtn_tensor =
      TfLiteInterpreterGetOutputTensor(interpreter, kOutputTensorDetections);
  const int num_detections = (int)((float*)TfLiteTensorData(dtn_tensor))[0];
  g_info("num detections: %d", num_detections);

  const TfLiteTensor* loc_tensor =
      TfLiteInterpreterGetOutputTensor(interpreter, kOutputTensorLocations);
  const float* locations = (const float*)TfLiteTensorData(loc_tensor);

  const TfLiteTensor* cls_tensor =
      TfLiteInterpreterGetOutputTensor(interpreter, kOutputTensorClasses);
  const float* classes = (const float*)TfLiteTensorData(cls_tensor);

  const TfLiteTensor* scr_tensor =
      TfLiteInterpreterGetOutputTensor(interpreter, kOutputTensorScores);
  const float* scores = (const float*)TfLiteTensorData(scr_tensor);

  for (int i = 0; i < num_detections; ++i) {
    g_info("%d: class=%d score=%f location=[%f %f %f %f]", i, (int)classes[i],
           scores[i], locations[4 * i], locations[4 * i + 1],
           locations[4 * i + 2], locations[4 * i + 3]);
  }
}

int main() {
  g_info("tensorflow lite version: %s", TfLiteVersion());

  g_autoptr(TfLiteModel) model = TfLiteModelCreateFromFile(TFLITE_MODEL_FILE);
  if (!model) {
    g_error("TfLiteModelCreateFromFile failed: %s", TFLITE_MODEL_FILE);
    return EXIT_FAILURE;
  }
  g_info("model file: %s", TFLITE_MODEL_FILE);

  g_autoptr(TfLiteInterpreterOptions) options =
      TfLiteInterpreterOptionsCreate();
  TfLiteInterpreterOptionsSetErrorReporter(options, error_cb, NULL);

  g_autoptr(TfLiteInterpreter) interpreter =
      TfLiteInterpreterCreate(model, options);
  if (!interpreter) {
    g_error("TfLiteInterpreterCreate failed");
    return EXIT_FAILURE;
  }

  TfLiteStatus status = TfLiteInterpreterAllocateTensors(interpreter);
  if (status != kTfLiteOk) {
    g_error("TfLiteInterpreterAllocateTensors failed: %d", status);
    return EXIT_FAILURE;
  }

  g_autoptr(GError) error = NULL;
  g_autoptr(Image) input_image = image_from_file(INPUT_FILE, &error);
  if (!input_image) {
    g_error("image_from_file failed: %s", error->message);
  }
  if (!load_input(input_image, interpreter, &error)) {
    g_error("load_input failed: %s", error->message);
  }

  status = TfLiteInterpreterInvoke(interpreter);
  if (status != kTfLiteOk) {
    g_error("TfLiteInterpreterInvoke failed: %d", status);
    return EXIT_FAILURE;
  }

  print_output(interpreter);
  return EXIT_SUCCESS;
}
