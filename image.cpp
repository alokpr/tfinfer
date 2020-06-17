#include "image.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int cv_type(TfLiteType tf_type) {
  switch (tf_type) {
    case kTfLiteUInt8:
      return CV_8UC3;
    case kTfLiteFloat32:
      return CV_32FC3;
    default:
      g_assert_not_reached();
      return -1;
  }
}

void image_read(const char* filepath, TfLiteTensor* tensor) {
  g_assert_nonnull(filepath);
  g_assert_nonnull(tensor);

  // Number of tensor dimensions must be 4 - batch, height, width, channels.
  g_assert_cmpint(4, ==, TfLiteTensorNumDims(tensor));
  // Batch size must be 1.
  g_assert_cmpint(1, ==, TfLiteTensorDim(tensor, 0));
  // Number of channels must be 3 - RGB.
  g_assert_cmpint(3, ==, TfLiteTensorDim(tensor, 3));

  int height = TfLiteTensorDim(tensor, 1);
  int width = TfLiteTensorDim(tensor, 2);
  g_assert_cmpint(0, <, height);
  g_assert_cmpint(0, <, width);

  int data_type = cv_type(TfLiteTensorType(tensor));
  cv::Mat dst(height, width, data_type, TfLiteTensorData(tensor));
  g_assert(!dst.empty());
  g_assert_cmpint(dst.total() * dst.elemSize(), ==,
                  TfLiteTensorByteSize(tensor));

  cv::Mat src = cv::imread(filepath, cv::IMREAD_COLOR);
  g_assert(!src.empty());
  cv::resize(src, src, cv::Size(width, height));
  cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
  if (data_type != CV_8UC3) {
    g_assert_cmpint(CV_32FC3, ==, data_type);
    src.convertTo(dst, CV_32FC3, 1 / 255.0);
  }
}
