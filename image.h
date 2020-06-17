#ifndef TFINFER_IMAGE_H_
#define TFINFER_IMAGE_H_

#include <glib.h>
#include <tensorflow/lite/c/c_api.h>

G_BEGIN_DECLS

typedef struct _Image Image;

void image_free(Image* image);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(Image, image_free);

Image* image_new();
Image* image_from_file(const char* filepath, GError** error);
Image* image_from_tensor(const TfLiteTensor* tensor, GError** error);
Image* image_clone(const Image* image);

void image_write(const Image* img, const char* filepath);
void image_size(const Image* img, int* w, int* h);

void image_copy(const Image* src, Image* dst);
void image_convert(const Image* src, double alpha, Image* dst);
void image_resize(const Image* src, int w, int h, Image* dst);
void image_swap_rb(const Image* src, Image* dst);

G_END_DECLS

#endif  // TFINFER_IMAGE_H_
