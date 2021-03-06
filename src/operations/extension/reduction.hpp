/***************************************************************************
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
 *  @filename reduction.hpp
 *
 **************************************************************************/

#ifndef SYCL_BLAS_EXTENSION_REDUCTION_HPP
#define SYCL_BLAS_EXTENSION_REDUCTION_HPP

#include "operations/extension_trees.h"
#include "views/view.h"
#include <CL/sycl.hpp>
#include <string>

namespace blas {

/* Constructor of the wrapper class */
template <typename operator_t, typename input_t, typename output_t, int ClSize,
          int WgSize, typename element_t, int Reduction_type>
SYCL_BLAS_INLINE
Reduction<operator_t, input_t, output_t, ClSize, WgSize, element_t,
          Reduction_type>::Reduction(input_t in, output_t out,
                                     typename input_t::index_t num_rows,
                                     typename input_t::index_t num_cols)
    : in_(in), out_(out), rows_(num_rows), cols_(num_cols) {}

}  // namespace blas

#endif  // SYCL_BLAS_EXTENSION_REDUCTION_HPP
