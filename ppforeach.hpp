#pragma once

#if defined(_MSC_VER) && (!defined(_MSVC_TRADITIONAL) || _MSVC_TRADITIONAL)
# pragma message("Please turn on /Zc:preprocessor before using PP_FOREACH!")
# define PP_VA_OPT_SUPPORT(...) 0
#else
# define PP_CONCAT_(a, b)                             a##b
# define PP_CONCAT(a, b)                              PP_CONCAT_(a, b)
# define PP_GET_1(a, ...)                             a
# define PP_GET_2(a, b, ...)                          b
# define PP_GET_3(a, b, c, ...)                       c
# define PP_GET_4(a, b, c, d, ...)                    d
# define PP_GET_5(a, b, c, d, e, ...)                 e
# define PP_GET_6(a, b, c, d, e, f, ...)              f
# define PP_GET_7(a, b, c, d, e, f, g, ...)           g
# define PP_GET_8(a, b, c, d, e, f, g, h, ...)        h
# define PP_GET_9(a, b, c, d, e, f, g, h, i, ...)     i
# define PP_GET_10(a, b, c, d, e, f, g, h, i, j, ...) j
# define PP_VA_EMPTY_(...)                            PP_GET_2(__VA_OPT__(, ) 0, 1, )
# define PP_VA_OPT_SUPPORT                            !PP_VA_EMPTY_
# if PP_VA_OPT_SUPPORT(?)
#  define PP_VA_EMPTY(...) PP_VA_EMPTY_(__VA_ARGS__)
# else
#  define PP_VA_EMPTY(...) 0
# endif
# define PP_IF(a, t, f)     PP_IF_(a, t, f)
# define PP_IF_(a, t, f)    PP_IF__(a, t, f)
# define PP_IF__(a, t, f)   PP_IF___(PP_VA_EMPTY a, t, f)
# define PP_IF___(a, t, f)  PP_IF____(a, t, f)
# define PP_IF____(a, t, f) PP_IF_##a(t, f)
# define PP_IF_0(t, f)      PP_UNWRAP_BRACE(f)
# define PP_IF_1(t, f)      PP_UNWRAP_BRACE(t)
# define PP_NARG(...) \
     PP_IF((__VA_ARGS__), (0), \
                 (PP_NARG_(__VA_ARGS__, 26, 25, 24, 23, 22, 21, 20, 19, \
                                 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, \
                                 6, 5, 4, 3, 2, 1)))
# define PP_NARG_(...) PP_NARG__(__VA_ARGS__)
# define PP_NARG__(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, \
                         _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, \
                         _23, _24, _25, _26, N, ...) \
     N
# define PP_FOREACH(f, ...) \
     PP_FOREACH_(PP_NARG(__VA_ARGS__), f, __VA_ARGS__)
# define PP_FOREACH_(N, f, ...)  PP_FOREACH__(N, f, __VA_ARGS__)
# define PP_FOREACH__(N, f, ...) PP_FOREACH_##N(f, __VA_ARGS__)
# define PP_FOREACH_0(f, ...)
# define PP_FOREACH_1(f, a)                f(a)
# define PP_FOREACH_2(f, a, b)             f(a) f(b)
# define PP_FOREACH_3(f, a, b, c)          f(a) f(b) f(c)
# define PP_FOREACH_4(f, a, b, c, d)       f(a) f(b) f(c) f(d)
# define PP_FOREACH_5(f, a, b, c, d, e)    f(a) f(b) f(c) f(d) f(e)
# define PP_FOREACH_6(f, a, b, c, d, e, g) f(a) f(b) f(c) f(d) f(e) f(g)
# define PP_FOREACH_7(f, a, b, c, d, e, g, h) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h)
# define PP_FOREACH_8(f, a, b, c, d, e, g, h, i) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i)
# define PP_FOREACH_9(f, a, b, c, d, e, g, h, i, j) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j)
# define PP_FOREACH_10(f, a, b, c, d, e, g, h, i, j, k) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k)
# define PP_FOREACH_11(f, a, b, c, d, e, g, h, i, j, k, l) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l)
# define PP_FOREACH_12(f, a, b, c, d, e, g, h, i, j, k, l, m) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m)
# define PP_FOREACH_13(f, a, b, c, d, e, g, h, i, j, k, l, m, n) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n)
# define PP_FOREACH_14(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o)
# define PP_FOREACH_15(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) f(p)
# define PP_FOREACH_16(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q)
# define PP_FOREACH_17(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r)
# define PP_FOREACH_18(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s)
# define PP_FOREACH_19(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t)
# define PP_FOREACH_20(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t, u) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t) f(u)
# define PP_FOREACH_21(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t, u, v) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t) f(u) f(v)
# define PP_FOREACH_22(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t, u, v, w) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t) f(u) f(v) f(w)
# define PP_FOREACH_23(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t, u, v, w, x) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t) f(u) f(v) f(w) f(x)
# define PP_FOREACH_24(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t, u, v, w, x, y) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t) f(u) f(v) f(w) f(x) f(y)
# define PP_FOREACH_25(f, a, b, c, d, e, g, h, i, j, k, l, m, n, o, p, \
                             q, r, s, t, u, v, w, x, y, z) \
     f(a) f(b) f(c) f(d) f(e) f(g) f(h) f(i) f(j) f(k) f(l) f(m) f(n) f(o) \
         f(p) f(q) f(r) f(s) f(t) f(u) f(v) f(w) f(x) f(y) f(z)
# define PP_STRINGIFY(...)     PP_STRINGIFY(__VA_ARGS__)
# define PP_STRINGIFY_(...)    #__VA_ARGS__
# define PP_EXPAND(...)        PP_EXPAND_(__VA_ARGS__)
# define PP_EXPAND_(...)       __VA_ARGS__
# define PP_UNWRAP_BRACE(...)  PP_UNWRAP_BRACE_ __VA_ARGS__
# define PP_UNWRAP_BRACE_(...) __VA_ARGS__
#endif
