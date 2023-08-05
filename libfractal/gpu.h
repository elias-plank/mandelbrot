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

#ifndef LIBFRACTAL_GPU_H
#define LIBFRACTAL_GPU_H

#include "types.h"

// ===================================================================================
// SHADER
// ===================================================================================

typedef struct shader {
    u32 handle;
} shader_t;

/**
 * Creates a shader from the given vertex and fragment shader files
 *
 * @param self shader handle
 * @param vertex path to the vertex shader
 * @param fragment path to the fragment shader
 * @return bool
 */
bool shader_create(shader_t *self, const char *vertex, const char *fragment);

/**
 * Destroys the specified shader
 *
 * @param self shader handle
 */
void shader_destroy(shader_t *self);

/**
 * Sets a sampler2d (texture) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param slot sampler slot for the texture
 */
void shader_uniform_sampler(shader_t *self, const char *name, u32 slot);

/**
 * Sets an integer (s32) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32(shader_t *self, const char *name, s32 value);

/**
 * Sets a 2d-integer (s32vec2_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32vec2(shader_t *self, const char *name, s32vec2_t *value);

/**
 * Sets a 3d-integer (s32vec3_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32vec3(shader_t *self, const char *name, s32vec3_t *value);

/**
 * Sets a 4d-integer (s32vec4_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_s32vec4(shader_t *self, const char *name, s32vec4_t *value);

/**
 * Sets a float (f32) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32(shader_t *self, const char *name, f32 value);

/**
 * Sets a 2d-float (f32vec2_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32vec2(shader_t *self, const char *name, f32vec2_t *value);

/**
 * Sets a 3d-float (f32vec3_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32vec3(shader_t *self, const char *name, f32vec3_t *value);

/**
 * Sets a 4d-float (f32vec4_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32vec4(shader_t *self, const char *name, f32vec4_t *value);

/**
 * Sets a mat4 (f32mat4_t) uniform
 *
 * @param self shader handle
 * @param name uniform name
 * @param value value
 */
void shader_uniform_f32mat4(shader_t *self, const char *name, f32mat4_t *value);

/**
 * Binds the specified shader
 *
 * @param self shader handle
 */
void shader_bind(shader_t *self);

/**
 * Unbinds the currently bound shader
 */
void shader_unbind(void);

typedef enum shader_type {
    INT = 0, INT2, INT3, INT4, FLOAT, FLOAT2, FLOAT3, FLOAT4, SAMPLER = INT
} shader_type_t;

// ===================================================================================
// VERTEX BUFFER
// ===================================================================================

typedef struct vertex_buffer_layout {
    shader_type_t *attributes;
    u32 count;
} vertex_buffer_layout_t;

typedef struct vertex_buffer {
    u32 handle;
    vertex_buffer_layout_t *layout;
} vertex_buffer_t;

/**
 * Creates a vertex buffer on the gpu
 *
 * @param self buffer handle
 */
void vertex_buffer_create(vertex_buffer_t *self);

/**
 * Destroys the vertex buffer
 *
 * @param self buffer Handle
 */
void vertex_buffer_destroy(vertex_buffer_t *self);

/**
 * Sets the data for the specified buffer
 *
 * @param self self handle
 * @param data pointer to the first element of the data
 * @param size size in bytes
 */
void vertex_buffer_data(vertex_buffer_t *self, const void *data, u32 size);

/**
 * Sets the attribute layout for the specified buffer
 * @param self self handle
 * @param layout layout handle
 */
void vertex_buffer_layout(vertex_buffer_t *self, vertex_buffer_layout_t *layout);

/**
 * Binds the specified buffer
 *
 * @param self Buffer handle
 */
void vertex_buffer_bind(vertex_buffer_t *self);

/**
 * Unbinds the currently bound buffer
 */
void vertex_buffer_unbind(void);

// ===================================================================================
// INDEX BUFFER
// ===================================================================================

typedef struct index_buffer {
    u32 handle;
    u32 count;
} index_buffer_t;

/**
 * Creates an index buffer on the gpu
 *
 * @param self Buffer handle
 */
void index_buffer_create(index_buffer_t *self);

/**
 * Destroys the index buffer
 *
 * @param self Buffer Handle
 */
void index_buffer_destroy(index_buffer_t *self);

/**
 * Sets the data for the specified buffer
 *
 * @param self buffer handle
 * @param data pointer to the first index
 * @param count number of indices
 */
void index_buffer_data(index_buffer_t *self, const u32 *data, u32 count);

/**
 * Binds the specified buffer
 *
 * @param self self handle
 */
void index_buffer_bind(index_buffer_t *self);

/**
 * Unbinds the currently bound buffer
 */
void index_buffer_unbind(void);

// ===================================================================================
// VERTEX ARRAY
// ===================================================================================

typedef struct vertex_array {
    u32 handle;
    vertex_buffer_t *vertex_buffer;
    index_buffer_t *index_buffer;
} vertex_array_t;

/**
 * Creates a new vertex array
 *
 * @param self vertex array handle
 */
void vertex_array_create(vertex_array_t *self);

/**
 * Destroys the specified vertex array
 *
 * @param self vertex array handle
 */
void vertex_array_destroy(vertex_array_t *self);

/**
 * Sets the vertex buffer for the vertex array, this sets all the specified attributes
 *
 * @param self vertex array handle
 * @param vertex_buffer vertex buffer handle
 */
void vertex_array_vertex_buffer(vertex_array_t *self, vertex_buffer_t *vertex_buffer);

/**
 * Sets the index buffer for the vertex array
 *
 * @param self vertex array handle
 * @param index_buffer index buffer handle
 */
void vertex_array_index_buffer(vertex_array_t *self, index_buffer_t *index_buffer);

/**
 * Binds the specified vertex array
 *
 * @param self vertex array handle
 */
void vertex_array_bind(vertex_array_t *self);

/**
 * Unbinds the currently bound vertex array
 */
void vertex_array_unbind(void);

#endif// LIBFRACTAL_GPU_H
