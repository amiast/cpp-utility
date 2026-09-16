# from kotone.misc.line import *
# https://github.com/amiast/cpp-utility

""" Given two points `(px, py)` and `(qx, qy)` where `px != qx`,
    the following snippet computes the `y`-intercept of the line passing both points.
"""

px, py = ...
qx, qy = ...
assert(px != qx)
intercept = (py * qx - qy * px) / (qx - px)

""" Alternatively, the following snippet computes the `y`-intercept
    from an integer equation `ax + by + c = 0` of the line.
"""

a = py - qy
b = qx - px
c = -(a * px + b * py)
assert(a * px + b * py + c == a * qx + b * qy + c == 0)
intercept = -c / b

# normalization (optional)
from math import gcd
g = gcd(a, b, c)
a, b, c = a // g, b // g, c // g
if a < 0 or a == 0 and b < 0:
    a, b, c = -a, -b, -c

""" Note that both methods are more precise than
    `intercept = py - (py - qy) * px / (px - qx)`
    due to rounding errors.
"""
