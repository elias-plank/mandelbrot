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

#include <stdio.h>
#include <stdlib.h>

#include "gpu.h"

// ===================================================================================
// SHADER
// ===================================================================================

static u32 shader_compile(const char* source, u32 type) {
    u32 program = glCreateShader(type);
    const GLchar* shader_source = source;
    glShaderSource(program, 1, &shader_source, NULL);
    glCompileShader(program);

    s32 success;
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        s32 info_length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &info_length);

        binary_buffer_t failure_info;
        failure_info.data = (char*) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetShaderInfoLog(program, info_length, &info_length, failure_info.data);
        glDeleteProgram(program);
        fprintf(stderr, "compilation failed: %s\n", failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return 0;
    }
    return program;
}

bool shader_create(shader_t* self, const char* vertex, const char* fragment) {
    u32 vertex_program = shader_compile(vertex, GL_VERTEX_SHADER);
    u32 fragment_program = shader_compile(fragment, GL_FRAGMENT_SHADER);

    u32 handle = glCreateProgram();
    glAttachShader(handle, vertex_program);
    glAttachShader(handle, fragment_program);
    glLinkProgram(handle);

    s32 link_success;
    glGetProgramiv(handle, GL_LINK_STATUS, &link_success);
    if (!link_success) {
        s32 info_length;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &info_length);

        binary_buffer_t failure_info;
        failure_info.data = (char*) malloc((size_t) info_length);
        failure_info.size = (u32) info_length;
        glGetProgramInfoLog(handle, info_length, NULL, failure_info.data);
        glDeleteProgram(handle);
        glDeleteProgram(vertex_program);
        glDeleteProgram(fragment_program);
        fprintf(stderr, "self linking failed: %s\n", failure_info.data);
        free(failure_info.data);
        failure_info.size = 0;
        return false;
    }

    s32 uniform_count;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniform_count);

    s32 uniform_length;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_length);

    if (uniform_count > 0 && uniform_length > 0) {
        binary_buffer_t uniform_name;
        uniform_name.data = (char*) malloc(uniform_length);
        uniform_name.size = (u32) uniform_length;

        for (u32 i = 0; i < (u32) uniform_count; i++) {
            s32 length, size;
            u32 data_type;

            glGetActiveUniform(handle, i, uniform_length, &length, &size, &data_type, uniform_name.data);
            s32 location = glGetUniformLocation(handle, uniform_name.data);
            fprintf(stderr, "self [%s, %s] uniform %s has location %d\n", vertex, fragment, uniform_name.data,
                    location);
        }
        free(uniform_name.data);
        uniform_name.size = 0;
    }
    self->handle = handle;
    return true;
}

void shader_destroy(shader_t* self) {
    glDeleteProgram(self->handle);
}

void shader_uniform_sampler(shader_t* self, const char* name, u32 slot) {
    shader_uniform_s32(self, name, (s32) slot);
}

void shader_uniform_s32(shader_t* self, const char* name, s32 value) {
    glUseProgram(self->handle);
    glUniform1i(glGetUniformLocation(self->handle, name), value);
}

void shader_uniform_s32vec2(shader_t* self, const char* name, s32vec2_t* value) {
    glUseProgram(self->handle);
    glUniform2i(glGetUniformLocation(self->handle, name), value->x, value->y);
}

void shader_uniform_s32vec3(shader_t* self, const char* name, s32vec3_t* value) {
    glUseProgram(self->handle);
    glUniform3i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

void shader_uniform_s32vec4(shader_t* self, const char* name, s32vec4_t* value) {
    glUseProgram(self->handle);
    glUniform4i(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

void shader_uniform_f32(shader_t* self, const char* name, f32 value) {
    glUseProgram(self->handle);
    glUniform1f(glGetUniformLocation(self->handle, name), value);
}

void shader_uniform_f32vec2(shader_t* self, const char* name, f32vec2_t* value) {
    glUseProgram(self->handle);
    glUniform2f(glGetUniformLocation(self->handle, name), value->x, value->y);
}

void shader_uniform_f32vec3(shader_t* self, const char* name, f32vec3_t* value) {
    glUseProgram(self->handle);
    glUniform3f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z);
}

void shader_uniform_f32vec4(shader_t* self, const char* name, f32vec4_t* value) {
    glUseProgram(self->handle);
    glUniform4f(glGetUniformLocation(self->handle, name), value->x, value->y, value->z, value->w);
}

void shader_uniform_f32mat4(shader_t* self, const char* name, f32mat4_t* value) {
    glUseProgram(self->handle);
    glUniformMatrix4fv(glGetUniformLocation(self->handle, name), 1, GL_FALSE, &value->value[0].x);
}

void shader_bind(shader_t* self) {
    glUseProgram(self->handle);
}

void shader_unbind() {
    glUseProgram(0);
}

static s32 shader_type_stride(shader_type_t type) {
    switch (type) {
        case INT:
            return sizeof(GLint);
        case INT2:
            return 2 * sizeof(GLint);
        case INT3:
            return 3 * sizeof(GLint);
        case INT4:
            return 4 * sizeof(GLint);
        case FLOAT:
            return sizeof(float);
        case FLOAT2:
            return 2 * sizeof(float);
        case FLOAT3:
            return 3 * sizeof(float);
        case FLOAT4:
            return 4 * sizeof(float);
        default:
            return 0;
    }
}

static s32 shader_type_opengl(shader_type_t type) {
    switch (type) {
        case INT:
        case INT2:
        case INT3:
        case INT4:
            return GL_INT;
        case FLOAT:
        case FLOAT2:
        case FLOAT3:
        case FLOAT4:
            return GL_FLOAT;
        default:
            return 0;
    }
}

static s32 shader_type_primitives(shader_type_t type) {
    switch (type) {
        case INT:
            return 1;
        case INT2:
            return 2;
        case INT3:
            return 3;
        case INT4:
            return 4;
        case FLOAT:
            return 1;
        case FLOAT2:
            return 2;
        case FLOAT3:
            return 3;
        case FLOAT4:
            return 4;
        default:
            return 0;
    }
}

// ===================================================================================
// VERTEX BUFFER
// ===================================================================================

static s32 vertex_buffer_layout_stride(vertex_buffer_layout_t *layout) {
    s32 stride = 0;
    for (u32 i = 0; i < layout->count; i++) {
        shader_type_t attribute = *(layout->attributes + i);
        stride += shader_type_stride(attribute);
    }
    return stride;
}

void vertex_buffer_create(vertex_buffer_t *self) {
    self->handle = 0;
    self->layout = NULL;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

void vertex_buffer_destroy(vertex_buffer_t *self) {
    glDeleteBuffers(1, &self->handle);
    self->layout = NULL;
}

void vertex_buffer_data(vertex_buffer_t *self, const void *data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void vertex_buffer_layout(vertex_buffer_t *self, vertex_buffer_layout_t *layout) {
    self->layout = layout;
}

void vertex_buffer_bind(vertex_buffer_t *self) {
    glBindBuffer(GL_ARRAY_BUFFER, self->handle);
}

void vertex_buffer_unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ===================================================================================
// INDEX BUFFER
// ===================================================================================

void index_buffer_create(index_buffer_t *self) {
    self->handle = 0;
    self->count = 0;
    glGenBuffers(1, &self->handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

void index_buffer_destroy(index_buffer_t *self) {
    glDeleteBuffers(1, &self->handle);
}

void index_buffer_data(index_buffer_t *self, const u32 *data, u32 count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (count * sizeof count), data, GL_DYNAMIC_DRAW);
    self->count = count;
}

void index_buffer_bind(index_buffer_t *self) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->handle);
}

void index_buffer_unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ===================================================================================
// VERTEX ARRAY
// ===================================================================================

void vertex_array_create(vertex_array_t *self) {
    self->handle = 0;
    self->vertex_buffer = NULL;
    self->index_buffer = NULL;
    glGenVertexArrays(1, &self->handle);
    glBindVertexArray(self->handle);
}

void vertex_array_destroy(vertex_array_t *self) {
    glDeleteVertexArrays(1, &self->handle);
}

void vertex_array_vertex_buffer(vertex_array_t *self, vertex_buffer_t *vertex_buffer) {
    glBindVertexArray(self->handle);
    vertex_buffer_bind(vertex_buffer);

    s64 offset = 0;
    s32 stride = vertex_buffer_layout_stride(vertex_buffer->layout);
    for (u32 i = 0; i < vertex_buffer->layout->count; i++) {
        glEnableVertexAttribArray(i);
        shader_type_t attribute = *(vertex_buffer->layout->attributes + i);
        GLenum opengl_type = (GLenum) shader_type_opengl(attribute);
        if (opengl_type == GL_FLOAT) {
            glVertexAttribPointer(i, shader_type_primitives(attribute), opengl_type, GL_FALSE, stride,
                                  (const void *) offset);
        } else if (opengl_type == GL_INT) {
            glVertexAttribIPointer(i, shader_type_primitives(attribute), opengl_type, stride, (const void *) offset);
        }
        offset += shader_type_stride(attribute);
    }
    self->vertex_buffer = vertex_buffer;
}

void vertex_array_index_buffer(vertex_array_t *self, index_buffer_t *index_buffer) {
    glBindVertexArray(self->handle);
    index_buffer_bind(index_buffer);
    self->index_buffer = index_buffer;
}

void vertex_array_bind(vertex_array_t *self) {
    glBindVertexArray(self->handle);
}

void vertex_array_unbind() {
    glBindVertexArray(0);
}
