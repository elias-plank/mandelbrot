/**
 * MIT License
 *
 * Copyright (c) 2023 Elias Engelbert Plank
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LIBFRACTAL_FRACTAL_H
#define LIBFRACTAL_FRACTAL_H

#include "gpu.h"

typedef struct fractal_pipeline {
    vertex_array_t vertex_array;
    vertex_buffer_t vertex_buffer;
    index_buffer_t index_buffer;
    shader_t shader;
} fractal_pipeline_t;

/**
 * Creates a new fractal pipeline
 *
 * @param self pipeline handle
 */
void fractal_pipeline_create(fractal_pipeline_t *self);

/**
 * Destroys the specified fractal pipeline
 *
 * @param self pipeline handle
 */
void fractal_pipeline_destroy(fractal_pipeline_t *self);

/**
 * Submit the pipeline state to the gpu
 *
 * @param self pipeline handle
 */
void fractal_pipeline_submit(fractal_pipeline_t* self);

#endif// LIBFRACTAL_FRACTAL_H
