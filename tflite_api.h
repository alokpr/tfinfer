#ifndef TFINFER_TF_API_H_
#define TFINFER_TF_API_H_

#include <glib.h>
#include <tensorflow/lite/c/c_api.h>

G_DEFINE_AUTOPTR_CLEANUP_FUNC(TfLiteInterpreter, TfLiteInterpreterDelete);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(TfLiteInterpreterOptions,
                              TfLiteInterpreterOptionsDelete);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(TfLiteModel, TfLiteModelDelete);

#endif  // TFINFER_TF_API_H_
