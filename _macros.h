// Copyright (c) 2012-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef _MACROS_H
#define _MACROS_H


#if defined(_MAC) && defined(_MAC_INT_64)
typedef unsigned int       DWORD;
#endif
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;

#define _ULONGLONG_
#if (!defined (_MAC) && (!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)))
//typedef __int64 LONGLONG;
//typedef unsigned __int64 ULONGLONG;

#define MAXLONGLONG                      (0x7fffffffffffffff)
#else

#if defined(_MAC) && defined(_MAC_INT_64)
typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;

#define MAXLONGLONG                      (0x7fffffffffffffff)
#else
typedef double LONGLONG;
typedef double ULONGLONG;
#endif //_MAC and int64

#endif

typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character

//#define ALIGN_DOWN(x, align)    (x & ~(align-1))
#define ALIGN_DOWN(x, align)    ((x) & ~((align) - 1))
#define ALIGN_UP(x, align)      (((x) & ((align) - 1)) ? ALIGN_DOWN((x), (align)) + (align) : (x))
#define MAXIMAL(a,b)            (((a)>(b))?(a):(b))
#define MINIMAL(a,b)            (((a) < (b)) ? (a) : (b))
//#define max(a,b)                (((a) > (b)) ? (a) : (b))
//#define min(a,b)                (((a) < (b)) ? (a) : (b))

#define __MAKEWORD(a, b)          ((WORD)(((BYTE)((DWORD *)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#define __MAKELONG(a, b)          ((LONG)(((WORD)((DWORD *)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
#define __LOWORD(l)               ((unsigned short)((unsigned int)(l) & 0xffff))
#define __HIWORD(l)               ((unsigned short)((unsigned int)(l) >> 16))
#define __LOBYTE(w)               ((BYTE)((DWORD *)(w) & 0xff))
#define __HIBYTE(w)               ((BYTE)((DWORD *)(w) >> 8))

#endif // _MACROS_H
