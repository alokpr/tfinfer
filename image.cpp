#include "image.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

G_DEFINE_QUARK(img - error, img_error)
#define IMG_ERROR (img_error_quark())

enum TensorDim {
  kTensorDimBatch,
  kTensorDimHeight,
  kTensorDimWidth,
  kTensorDimChannels,
  kTensorDimCount
};

struct _Image {
  cv::Mat impl;
};

static cv::Scalar to_color(int model, int klass) {
  // TODO: Implement a scheme.
  switch (klass) {
    case 0:
      return cv::Scalar(0, 0, 255);
    case 1:
      return cv::Scalar(0, 255, 0);
    case 2:
      return cv::Scalar(255, 0, 0);
    default:
      return cv::Scalar(255, 255, 0);
  }
}

void image_free(Image* image) { delete image; }

Image* image_new() { return new _Image; }

Image* image_from_file(const char* filepath, GError** error) {
  g_assert_nonnull(filepath);

  g_autoptr(Image) image = image_new();
  try {
    image->impl = cv::imread(filepath, cv::IMREAD_COLOR);
  } catch (cv::Exception& e) {
    *error = g_error_new_literal(IMG_ERROR, e.code, e.what());
    return NULL;
  }
  return static_cast<Image*>(g_steal_pointer(&image));
}

Image* image_from_tensor(const TfLiteTensor* tensor, GError** error) {
  g_assert_nonnull(tensor);

  if (TfLiteTensorNumDims(tensor) != kTensorDimCount) {
    return NULL;
  }
  if (TfLiteTensorDim(tensor, kTensorDimBatch) != 1) {
    return NULL;
  }
  if (TfLiteTensorDim(tensor, kTensorDimChannels) != 3) {
    return NULL;
  }

  int height = TfLiteTensorDim(tensor, kTensorDimHeight);
  if (height <= 0) {
    return NULL;
  }

  int width = TfLiteTensorDim(tensor, kTensorDimWidth);
  if (width <= 0) {
    return NULL;
  }

  int data_type = -1;
  switch (TfLiteTensorType(tensor)) {
    case kTfLiteUInt8:
      data_type = CV_8UC3;
      break;
    default:
      // TODO: Support kTfLiteFloat32 with CV_32FC3.
      return NULL;
  }

  void* data = TfLiteTensorData(tensor);
  if (!data) {
    return NULL;
  }
  // TODO: Verify TfLiteTensorByteSize.

  Image* image = image_new();
  image->impl = cv::Mat(height, width, data_type, data);
  return image;
}

Image* image_clone(const Image* image) {
  g_assert_nonnull(image);
  Image* clone = image_new();
  clone->impl = image->impl.clone();
  return clone;
}

void image_write(const Image* img, const char* filepath) {
  g_assert_nonnull(img);
  cv::imwrite(filepath, img->impl);
}

void image_size(const Image* img, int* w, int* h) {
  g_assert_nonnull(img);
  cv::Size size = img->impl.size();
  *w = size.width;
  *h = size.height;
}

void image_copy(const Image* src, Image* dst) {
  g_assert_nonnull(src);
  g_assert_nonnull(dst);
  src->impl.copyTo(dst->impl);
}

void image_convert(const Image* src, double alpha, Image* dst) {
  g_assert_nonnull(src);
  g_assert_nonnull(dst);
  src->impl.convertTo(dst->impl, dst->impl.type(), alpha);
}

void image_resize(const Image* src, int w, int h, Image* dst) {
  g_assert_nonnull(src);
  g_assert_nonnull(dst);
  cv::resize(src->impl, dst->impl, cv::Size(w, h));
}

void image_swap_rb(const Image* src, Image* dst) {
  g_assert_nonnull(src);
  g_assert_nonnull(dst);
  cv::cvtColor(src->impl, dst->impl, cv::COLOR_BGR2RGB);
}

void image_draw_roi(Image* img, int model, int klass, int score, int x, int y,
                    int w, int h) {
  cv::rectangle(img->impl, cv::Point(x, y), cv::Point(x + w, y + h),
                to_color(model, klass));
}
