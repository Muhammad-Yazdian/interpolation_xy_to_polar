# interpolation_xy_to_polar
This code converts a 2D discrete function from Cartesian representation to Polar one using node interpolation.

# 2021-10-29, 15:13 Code Tested
## Input
10 by 10 grid with unit pixel size and unit step at x=5.0
```
 {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}
```

## Output
```
Point (x, y)=(5, 0) is on element #4. The interpolated z value is 0.5.
Point (x, y)=(4.99375, 0.249896) is on element #4. The interpolated z value is 0.506249.
Point (x, y)=(4.97502, 0.499167) is on element #4. The interpolated z value is 0.524979.
Point (x, y)=(4.94386, 0.747191) is on element #4. The interpolated z value is 0.556145.
Point (x, y)=(4.90033, 0.993347) is on element #4. The interpolated z value is 0.599667.
Point (x, y)=(4.84456, 1.23702) is on element #4. The interpolated z value is 0.655438.
Point (x, y)=(4.77668, 1.4776) is on element #4. The interpolated z value is 0.723318.
Point (x, y)=(4.69686, 1.71449) is on element #13. The interpolated z value is 0.803136.
Point (x, y)=(4.6053, 1.94709) is on element #13. The interpolated z value is 0.894695.
Point (x, y)=(4.50224, 2.17483) is on element #13. The interpolated z value is 0.997764.
Point (x, y)=(4.38791, 2.39713) is on element #12. The interpolated z value is 1.
Point (x, y)=(4.26262, 2.61344) is on element #21. The interpolated z value is 1.
Point (x, y)=(4.12668, 2.82321) is on element #21. The interpolated z value is 1.
Point (x, y)=(3.98042, 3.02593) is on element #21. The interpolated z value is 1.
Point (x, y)=(3.82421, 3.22109) is on element #21. The interpolated z value is 1.
Point (x, y)=(3.65844, 3.40819) is on element #21. The interpolated z value is 1.
Point (x, y)=(3.48353, 3.58678) is on element #29. The interpolated z value is 1.
Point (x, y)=(3.29992, 3.7564) is on element #29. The interpolated z value is 1.
Point (x, y)=(3.10805, 3.91663) is on element #29. The interpolated z value is 1.
Point (x, y)=(2.90842, 4.06708) is on element #29. The interpolated z value is 1.
Point (x, y)=(2.70151, 4.20735) is on element #29. The interpolated z value is 1.
Point (x, y)=(2.48786, 4.33712) is on element #28. The interpolated z value is 1.
Point (x, y)=(2.26798, 4.45604) is on element #28. The interpolated z value is 1.
Point (x, y)=(2.04244, 4.56382) is on element #37. The interpolated z value is 1.
Point (x, y)=(1.81179, 4.6602) is on element #37. The interpolated z value is 1.
Point (x, y)=(1.57661, 4.74492) is on element #37. The interpolated z value is 1.
Point (x, y)=(1.33749, 4.81779) is on element #36. The interpolated z value is 1.
Point (x, y)=(1.09503, 4.87862) is on element #36. The interpolated z value is 1.
Point (x, y)=(0.849836, 4.92725) is on element #36. The interpolated z value is 1.
Point (x, y)=(0.602514, 4.96356) is on element #36. The interpolated z value is 1.
```
