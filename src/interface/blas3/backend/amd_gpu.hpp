/***************************************************************************
 *
 *  @license
 *  Copyright (C) Codeplay Software Limited
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  For your convenience, a copy of the License has been included in this
 *  repository.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  SYCL-BLAS: BLAS implementation using SYCL
 *
 *  @filename amd_gpu.hpp
 *
 **************************************************************************/
#ifndef SYCL_BLAS_GEMM_AMD_GPU_BACKEND_HPP
#define SYCL_BLAS_GEMM_AMD_GPU_BACKEND_HPP
#include "interface/gemm_launcher.h"

namespace blas {
namespace gemm {

namespace backend {
template <bool _t_a, bool _t_b, bool is_beta_zero, typename executor_t,
          typename container_0_t, typename container_1_t,
          typename container_2_t, typename element_t, typename index_t>
typename executor_t::policy_t::event_t _gemm(
    executor_t& ex, index_t _M, index_t _N, index_t _K, element_t _alpha,
    container_0_t _a, index_t _lda, container_1_t _b, index_t _ldb,
    element_t _beta, container_2_t _c, index_t _ldc, index_t batch_size) {
  /* Tall & Skinny matrices. */
  if (batch_size == 1 && ((_K > 8192 && _M <= 1024 && _N <= 1024) ||
                          (_K > 1024 && _M <= 256 && _N <= 256))) {
    if (_M <= 16 && _N > 32) {
      return blas::Gemm_Launcher<
          256, true, true, true, 64, Tile<1, 4, 16, 16>, _t_a, _t_b,
          static_cast<int>(gemm_memory_t::local),
          static_cast<int>(gemm_algorithm_t::tall_skinny),
          is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                               _b, _ldb, _beta, _c, _ldc, 1);
    } else if (_M > 64 && _N <= 32) {
      return blas::Gemm_Launcher<
          256, true, true, true, 64, Tile<4, 1, 16, 16>, _t_a, _t_b,
          static_cast<int>(gemm_memory_t::local),
          static_cast<int>(gemm_algorithm_t::tall_skinny),
          is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                               _b, _ldb, _beta, _c, _ldc, 1);
    } else if (_M <= 16 || _N <= 16) {
      return blas::Gemm_Launcher<
          256, true, true, true, 64, Tile<1, 1, 16, 16>, _t_a, _t_b,
          static_cast<int>(gemm_memory_t::local),
          static_cast<int>(gemm_algorithm_t::tall_skinny),
          is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                               _b, _ldb, _beta, _c, _ldc, 1);
    } else if (_M <= 32 || _N <= 32) {
      return blas::Gemm_Launcher<
          256, true, true, true, 64, Tile<2, 2, 16, 16>, _t_a, _t_b,
          static_cast<int>(gemm_memory_t::local),
          static_cast<int>(gemm_algorithm_t::tall_skinny),
          is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                               _b, _ldb, _beta, _c, _ldc, 1);
    } else {
      return blas::Gemm_Launcher<
          256, true, true, true, 64, Tile<4, 4, 16, 16>, _t_a, _t_b,
          static_cast<int>(gemm_memory_t::local),
          static_cast<int>(gemm_algorithm_t::tall_skinny),
          is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                               _b, _ldb, _beta, _c, _ldc, 1);
    }
  } else if (_M * _N <= 65536) {
    return blas::Gemm_Launcher<
        256, false, false, false, 64, Tile<1, 1, 16, 16>, _t_a, _t_b,
        static_cast<int>(gemm_memory_t::local),
        static_cast<int>(gemm_algorithm_t::standard),
        is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                             _b, _ldb, _beta, _c, _ldc,
                                             batch_size);
  } else if (_M * _N <= 262144) {
    return blas::Gemm_Launcher<
        256, false, false, false, 64, Tile<4, 1, 16, 16>, _t_a, _t_b,
        static_cast<int>(gemm_memory_t::local),
        static_cast<int>(gemm_algorithm_t::standard),
        is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                             _b, _ldb, _beta, _c, _ldc,
                                             batch_size);
  } else {
    return blas::Gemm_Launcher<
        256, false, false, false, 64, Tile<8, 8, 16, 16>, _t_a, _t_b,
        static_cast<int>(gemm_memory_t::local),
        static_cast<int>(gemm_algorithm_t::standard),
        is_beta_zero>::template _select_gemm(ex, _M, _N, _K, _alpha, _a, _lda,
                                             _b, _ldb, _beta, _c, _ldc,
                                             batch_size);
  }
}
}  // namespace backend
}  // namespace gemm
}  // namespace blas
#endif
