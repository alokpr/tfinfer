FROM ubuntu:18.04

RUN apt-get update && apt-get --no-install-recommends --yes install \
    build-essential \
    ca-certificates \
    curl \
    git \
    python3-dev \
    python3-pip \
    unzip \
    wget \
    zip \
    zlib1g-dev

# Some TF tools expect a "python" binary
RUN ln -s $(which python3) /usr/local/bin/python

RUN pip3 install numpy

ARG BAZEL_VERSION=2.0.0
RUN curl -sfL "https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/bazel-${BAZEL_VERSION}-installer-linux-x86_64.sh" -o /tmp/bazel-installer.sh \
    && chmod +x /tmp/bazel-installer.sh \
    && /tmp/bazel-installer.sh

ARG TF_VERSION=2.2.0
RUN curl -sfL https://github.com/tensorflow/tensorflow/archive/v${TF_VERSION}.tar.gz | tar -xzC /tmp
WORKDIR /tmp/tensorflow-${TF_VERSION}/
RUN bazel build \
    -c opt \
    --config=avx2_linux \
    --config=monolithic \
    --config=noaws \
    --config=nogcp \
    --config=nohdfs \
    --config=nonccl \
    //tensorflow/tools/lib_package:cheaders \
    //tensorflow/tools/lib_package:clib
RUN tar -xC /usr -f bazel-bin/tensorflow/tools/lib_package/cheaders.tar
RUN tar -xC /usr -f bazel-bin/tensorflow/tools/lib_package/clib.tar

RUN bazel build \
    -c opt \
    //tensorflow/lite/c:tensorflowlite_c
RUN mkdir -p /usr/include/tensorflow/lite/c/ \
    && cp tensorflow/lite/c/*.h /usr/include/tensorflow/lite/c/ \
    && cp bazel-bin/tensorflow/lite/c/libtensorflowlite_c.so /usr/lib/

RUN apt-get --no-install-recommends --yes install \
    ninja-build \
    python3-setuptools \
    python3-wheel
RUN pip3 install meson

RUN DEBIAN_FRONTEND=noninteractive apt-get --no-install-recommends --yes install \
    libglib2.0-dev \
    libopencv-dev
