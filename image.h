#ifndef TFINFER_IMAGE_H_
#define TFINFER_IMAGE_H_

#include <glib.h>
#include <tensorflow/lite/c/c_api.h>

G_BEGIN_DECLS

void image_read(const char* filepath, TfLiteTensor* tensor);

G_END_DECLS

#endif  // TFINFER_IMAGE_H_
