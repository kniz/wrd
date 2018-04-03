/*
 * Copyright (C) 2012 William Swanson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */
#pragma once

#define _NE_EVALUATOR0(...) __VA_ARGS__
#define _NE_EVALUATOR1(...) _NE_EVALUATOR0(_NE_EVALUATOR0(_NE_EVALUATOR0(__VA_ARGS__)))
#define _NE_EVALUATOR2(...) _NE_EVALUATOR1(_NE_EVALUATOR1(_NE_EVALUATOR1(__VA_ARGS__)))
#define _NE_EVALUATOR3(...) _NE_EVALUATOR2(_NE_EVALUATOR2(_NE_EVALUATOR2(__VA_ARGS__)))
#define _NE_EVALUATOR4(...) _NE_EVALUATOR3(_NE_EVALUATOR3(_NE_EVALUATOR3(__VA_ARGS__)))
#define NE_EVALUATOR(...)  _NE_EVALUATOR4(_NE_EVALUATOR4(_NE_EVALUATOR4(__VA_ARGS__)))
