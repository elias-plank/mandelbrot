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
#include "math.h"

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
layout(location = 0) in vec4 attrib_position;
layout(location = 0) out vec4 passed_position;

// matrix for transforming fragments into mandelbrot space
uniform mat4 uniform_projection;

void main() {
    passed_position = inverse(uniform_projection) * attrib_position;
    gl_Position = attrib_position;
});

// ===================================================================================
// FRAGMENT SHADER SOURCE
// ===================================================================================

DEFINE_SHADER(shader_fragment,
layout(location = 0) out vec4 output_color;
layout(location = 0) in vec4 passed_position;

vec4 color(float t) {
    float r = 9.0 * (1.0 - t) * t * t * t;
    float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;
    return vec4(r, g, b, 1.0);
}

vec4 mandelbrot(vec2 c) {
    int iteration = 0;
    int max_iteration = 50;

    vec2 z = vec2(0);
    for (; iteration < max_iteration; ++iteration) {
        // z^2 in complex:
        // -> (x + y*i)^2
        // -> (x + y*i) * (x + y*i)
        // -> x^2 + 2 * x * y * i - y^2
        // real part (x):
        // -> x^2 - y^2
        // imaginary part (y):
        // -> 2 * x * y * i
        float x = z.x * z.x - z.y * z.y;
        float y = 2 * z.x * z.y;

        // bounds check
        if (x * x + y * y > 4) {
            break;
        }

        z.x = x + c.x;
        z.y = y + c.y;
    }

    if (iteration < max_iteration) {
        return color(
        float(iteration) / float(max_iteration));
    }
    return vec4(0.0);
}

void main() {
    output_color = mandelbrot(passed_position.xy);
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
    static shader_type_t attributes[] = {FLOAT4};
    static vertex_buffer_layout_t layout;
    layout.attributes = attributes;
    layout.count = STACK_ARRAY_SIZE(attributes);

    vertex_buffer_layout(&self->vertex_buffer, &layout);
    vertex_array_vertex_buffer(&self->vertex_array, &self->vertex_buffer);
    vertex_array_index_buffer(&self->vertex_array, &self->index_buffer);

    shader_create(&self->shader, shader_vertex, shader_fragment);

    // Two triangles are the drawing surface of our computation shader
    static vertex_t vertices[] = {
            {{1.0f,  -1.0f, 0.0f, 1.0f}},
            {{-1.0f, -1.0f, 0.0f, 1.0f}},
            {{-1.0f, 1.0f,  0.0f, 1.0f}},
            {{1.0f,  1.0f,  0.0f, 1.0f}}
    };

    static u32 indices[] = {0, 1, 2, 2, 0, 3};

    vertex_buffer_data(&self->vertex_buffer, vertices, sizeof vertices);
    index_buffer_data(&self->index_buffer, indices, STACK_ARRAY_SIZE(indices));
}

void fractal_pipeline_destroy(fractal_pipeline_t *self) {
    shader_destroy(&self->shader);
    index_buffer_destroy(&self->index_buffer);
    vertex_buffer_destroy(&self->vertex_buffer);
    vertex_array_destroy(&self->vertex_array);
}

void fractal_pipeline_submit(fractal_pipeline_t *self) {
    f32mat4_t projection;
    f32mat4_create_orthogonal(&projection, -2.0f, 0.47f, -1.12f, 1.12f);
    shader_uniform_f32mat4(&self->shader, "uniform_projection", &projection);

    shader_bind(&self->shader);
    vertex_array_bind(&self->vertex_array);
    glDrawElements(GL_TRIANGLES, (GLsizei) self->vertex_array.index_buffer->count, GL_UNSIGNED_INT, NULL);
    vertex_array_unbind();
}
