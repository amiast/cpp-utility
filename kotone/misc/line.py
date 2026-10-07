# from kotone.misc.line import *
# https://github.com/amiast/cpp-utility

""" The following snippet manipulates lines of the form `ax + by + c = 0`
    where `a, b, c` are real numbers.
"""

from math import hypot

def same_line(s, t, eps = 1e-10):
    """ Returns whether `s` and `t` are identical lines. """
    sa, sb, sc = s
    ta, tb, tc = t
    return abs(sa * tb - sb * ta) < eps and abs(sa * tc - sc * ta) < eps and abs(sb * tc - sc * tb) < eps

def is_normalized(s, eps = 1e-10):
    """ Returns whether line `s` is in normal form. """
    a, b, _ = s
    return abs(a ** 2 + b ** 2 - 1) < eps

def normalize(a, b, c):
    """ Returns the normal form of line `ax + by + c = 0`. """
    d = hypot(a, b)
    assert d != 0
    a, b, c = a / d, b / d, c / d
    if a < 0 or a == 0 and b < 0:  # canonicalization (optional)
        a, b, c = -a, -b, -c
    return a, b, c

def line_equation(p, q):
    """ Returns the line passing distinct points `p` and `q`. """
    px, py = p
    qx, qy = q
    assert px != qx or py != qy
    a = py - qy
    b = qx - px
    return normalize(a, b, -(a * px + b * py))

def line_bisectors(s, t):
    """ Returns a list of bisectors of distinct, normalized lines `s` and `t`. """
    assert is_normalized(s) and is_normalized(t)
    sa, sb, sc = s
    ta, tb, tc = t
    bisectors = []
    if sa + ta or sb + tb:
        bisectors.append(normalize(sa + ta, sb + tb, sc + tc))
    if sa - ta or sb - tb:
        bisectors.append(normalize(sa - ta, sb - tb, sc - tc))
    return bisectors

def point_bisector(p, q):
    """ Returns the bisector of distinct points `p` and `q`. """
    px, py = p
    qx, qy = q
    assert px != qx or py != qy
    bisector = normalize((px - qx) * 2, (py - qy) * 2, qx ** 2 + qy ** 2 - px ** 2 - py ** 2)
    return bisector

def intersection(s, t):
    """ Returns the intersection of two non-parallel lines. """
    sa, sb, sc = s
    ta, tb, tc = t
    d = sa * tb - sb * ta
    assert d != 0
    x = (sb * tc - sc * tb) / d
    y = (sc * ta - sa * tc) / d
    return x, y

def distance(p, s):
    """ Returns the distance of point `p` and normalized line `s`. """
    assert is_normalized(s)
    x, y = p
    a, b, c = s
    dist = abs(a * x + b * y + c)
    return dist

def perpendicular(p, s):
    """ Returns the line through point `p` perpendicular to line `s`. """
    x, y = p
    a, b, _ = s
    return normalize(-b, a, b * x - a * y)

def parallel(p, s):
    """ Returns the line through `p` parallel to `s`. """
    x, y = p
    a, b, _ = s
    return normalize(a, b, -(a * x + b * y))
