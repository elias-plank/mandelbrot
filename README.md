# Mandelbrot (WIP)

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

High performance, work in progress C and OpenGL rewrite of [the infamous FractalViewer](https://github.com/elias-plank/FractalViewer).

 - Plain C/OpenGL implementation with [GLFW](https://github.com/glfw/glfw) and [glad](https://glad.dav1d.de) as the only dependencies
 - Cross-platform
 - Zero dynamic memory allocations

## Definition

The mandelbrot set is defined in the complex plane as the complex numbers $c$ for which the function $f_c(z) = z^2 + c$ does not diverge to infinity when iterated starting at $z = 0$.

## Progress

 - [x] GPU hello world (colored rectangle)
 - [ ] First mandelbrot
 - [ ] Explorable mandelbrot
 - [ ] Custom color functions
 - [ ] Julia set

## Implementation details

The _actual_ code for computing the mandelbrot set is defined in a GLSL fragment shader inside [fractal.c](libfractal).