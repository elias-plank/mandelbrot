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

#ifndef LIBFRACTAL_DISPLAY_H
#define LIBFRACTAL_DISPLAY_H

#include "types.h"

#include <GLFW/glfw3.h>

typedef struct display {
    GLFWwindow* handle;
    u32 width;
    u32 height;
    f64 time;
    bool running;
} display_t;

/**
 * Creates a new window and a corresponding OpenGL context
 *
 * @param self display handle
 * @param title title of the window
 * @param width width of the window
 * @param height height of the window
 * @return bool
 */
bool display_create(display_t* self, const char* title, u32 width, u32 height);

/**
 * Destroys the window
 *
 * @param self display handle
 */
void display_destroy(display_t* self);

/**
 * Swaps front and back buffer
 *
 * @param self display handle
 * @return frame time
 */
f64 display_update_frame(display_t* self);

/**
 * Checks if the window should be closed or not
 *
 * @param self display handle
 * @return
 */
bool display_running(display_t* self);

#endif// LIBFRACTAL_DISPLAY_H
