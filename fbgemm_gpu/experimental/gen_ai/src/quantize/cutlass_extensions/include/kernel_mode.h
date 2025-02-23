/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <ATen/ATen.h>

namespace fbgemm_gpu {

enum class KernelMode { Small, Large, Default };

inline KernelMode get_kernel_mode(at::Tensor XQ, at::Tensor WQ) {
  auto M = XQ.size(0);
  auto K = XQ.size(1);
  auto N = WQ.size(0);
  // Use a large kernel if at least two shapes are large....
  bool use_large_kernel =
      ((M >= 2048 && K >= 2048) || (M >= 2048 && N >= 2048) ||
       (K >= 2048 && N >= 2048));
  if (M <= 128 || N <= 128) {
    return KernelMode::Small;
  } else if (use_large_kernel) {
    return KernelMode::Large;
  } else {
    return KernelMode::Default;
  }
}

inline KernelMode get_batched_kernel_mode(at::Tensor XQ, at::Tensor WQ) {
  auto B = XQ.size(0);
  auto M = XQ.size(1);
  auto K = XQ.size(2);
  auto N = WQ.size(1);
  auto BM = B * M;
  auto BN = B * N;
  auto BK = B * K;
  // Use a large kernel if at least two shapes are large....
  bool use_large_kernel =
      ((BM >= 2048 && BK >= 2048) || (BM >= 2048 && BK >= 2048) ||
       (BK >= 2048 && BN >= 2048));
  if (BM <= 128 || BN <= 128) {
    return KernelMode::Small;
  } else if (use_large_kernel) {
    return KernelMode::Large;
  } else {
    return KernelMode::Default;
  }
}

} // namespace fbgemm_gpu
