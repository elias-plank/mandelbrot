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

#include "fractal.h"

// ===================================================================================
// SHADER SOURCE MACRO HELPERS
// ===================================================================================

#define SHADER_VERSION "#version 450 core\n"
#define DEFINE_SHADER_VARIABLE_IMPL(type, name) static type name
#define DEFINE_SHADER_VARIABLE(type, name) DEFINE_SHADER_VARIABLE_IMPL(type, name)
#define DEFINE_SHADER_IMPL(type, source) DEFINE_SHADER_VARIABLE(const char*, type) = SHADER_VERSION #source
#define DEFINE_SHADER(type, source) DEFINE_SHADER_IMPL(type, source)

// ===================================================================================
// VERTEX SHADER SOURCE
// ===================================================================================

DEFINE_SHADER(shader_vertex,
layout(location = 0) in vec3 attrib_position;
layout(location = 0) out vec3 passed_position;
void main() {
    passed_position = attrib_position;
    gl_Position = vec4(attrib_position, 1.0);
});

// ===================================================================================
// FRAGMENT SHADER SOURCE
// ===================================================================================

DEFINE_SHADER(shader_fragment,
layout(location = 0) out vec4 output_color;
layout(location = 0) in vec3 passed_position;
void main() {
    output_color = vec4(passed_position, 1.0);
});

// ===================================================================================
// FRACTAL PIPELINE
// ===================================================================================

void fractal_pipeline_create(fractal_pipeline_t *self) {
    // Pipeline gpu objects
    vertex_array_create(&self->vertex_array);
    vertex_buffer_create(&self->vertex_buffer);
    index_buffer_create(&self->index_buffer);

    // We only need the position in the vertex shader
    static shader_type_t attributes[] = {FLOAT3};
    static vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);

    shader_create(&self->shader, shader_vertex, shader_fragment);

    // Two triangles are the drawing surface of our computation shader
    static vertex_t vertices[] = {
            {{-1.0f, -1.0f, 0.0f}},
            {{1.0f,  -1.0f, 0.0f}},
            {{1.0f,  1.0f,  0.0f}},
            {{-1.0f, 1.0f,  0.0f}}
    };

    static u32 indices[] = {0, 1, 2, 2, 0, 3 };

    vertex_buffer_data(&self->vertex_buffer, vertices, sizeof vertices);
    index_buffer_data(&self->index_buffer, indices, STACK_ARRAY_SIZE(indices));
}

void fractal_pipeline_destroy(fractal_pipeline_t *self) {
    shader_destroy(&self->shader);
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);
}

void fractal_pipeline_submit(fractal_pipeline_t* self) {
    shader_bind(&self->shader);
    vertex_array_bind(&self->vertex_array);
    glDrawElements(GL_TRIANGLES, (GLsizei) self->vertex_array.index_buffer->count, GL_UNSIGNED_INT, NULL);
    vertex_array_unbind();
}
