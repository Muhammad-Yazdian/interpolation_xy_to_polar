/**
 * File   main.cpp
 * 
 * This code converts a 2D discrete function from Cartesian representation to Polar 
 * one using node interpolation.
 * 
 * How does it works:
 *   1. Grid configuration:
 *     1.1. Element numbering
 * 
 *     y
 *     ^
 *     |
 *     |    *     *     *     *
 *     |  * * * * * * * * * * * *
 *     |    *  1  *  2  *  3  *
 *     |  * * * * * * * * * * * *
 *     |    *  4  *  5  *  6  *
 *     |  * * * * * * * * * * * *
 *     |    *  7  *  8  *  9  *
 *     |  * * * * * * * * * * * *
 *     |    *     *     *     *
 *     |
 *     . ----------------------------> x
 * 
 *     1.2. Node numbering
 *     1.3. Element adjacency matrix
 *     1.4. Node adjacency matrix
 **/

#include <iostream>
#include <string>

class PointCartesian{
 public:
  double x_;
  double y_;
  PointCartesian();
};

class PointPolar{
 public:
  double r_;
  double theta_;
  PointPolar();
};

class Element{
 public:
  double width_;
  double height_;
  double value_;
  Element();
};

PointCartesian ConvertPolarToCartesian(PointPolar p);
int ConvertCartesianToElementNumber(PointCartesian p);

int main()
{
  std::cout << "Test"  << std::endl;
}
