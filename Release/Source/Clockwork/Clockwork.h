//
// Copyright (c) 2008-2015 the Clockwork project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef CLOCKWORK_API_H
#define CLOCKWORK_API_H

#ifdef CLOCKWORK_STATIC_DEFINE
#  define CLOCKWORK_API
#  define CLOCKWORK_NO_EXPORT
#else
#  ifndef CLOCKWORK_API
#    ifdef Clockwork_EXPORTS
        /* We are building this library */
#      define CLOCKWORK_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CLOCKWORK_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CLOCKWORK_NO_EXPORT
#    define CLOCKWORK_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef CLOCKWORK_DEPRECATED
#  define CLOCKWORK_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CLOCKWORK_DEPRECATED_EXPORT
#  define CLOCKWORK_DEPRECATED_EXPORT CLOCKWORK_API CLOCKWORK_DEPRECATED
#endif

#ifndef CLOCKWORK_DEPRECATED_NO_EXPORT
#  define CLOCKWORK_DEPRECATED_NO_EXPORT CLOCKWORK_NO_EXPORT CLOCKWORK_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define CLOCKWORK_NO_DEPRECATED
#endif

#endif
