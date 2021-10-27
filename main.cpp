/**
 * File   main.cpp
 * 
 * This code converts a 2D discrete function from Cartesian representation to Polar 
 * one using node interpolation.
 * 
 * How does it works:
 *   1. Grid configuration:
 *     1.1. Element numbering
 *       - Boundary is defined by Element 0.
 * 
 *     y
 *     ^
 *     |
 *     |  0 *  0  *  0   *  0  *
 *     |  * * * * * * * * * * * *
 *     |  0 *  1  *  2  *  3  *
 *     |  * * * * * * * * * * * *
 *     |  0 *  4  *  5  *  6  *
 *     |  * * * * * * * * * * * *
 *     |  0 *  7  *  8  *  9  *
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
#include <vector>
#include <iterator>
#include <string>

class PointCartesian{
 public:
  double x_;
  double y_;
  PointCartesian(double x, double y){
    x_ = x;
    y_ = y;
  };
};

class PointPolar{
 public:
  double r_;
  double theta_;
  PointPolar();
};

class Element{
 public:
  int id_;
  double value_;
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  int adj_elements_[8]; // Conventional matrix notaion excluding #5 (see header comment 1.1)

  Element(int id, double value, double pos_x, double pos_y, double width, double height, int adj_element){
    id_ = id;
    value_ = value;
    pos_x_= pos_x;
    pos_y_= pos_y;
    width = width;
    height_ = height;
    //adj_elements_ = adj_elements_;
  };
};

class Grid{
 public:
  int num_elements_x_;
  int num_elements_y_;
  double element_width_;
  double element_height_;
  double grid_base_value_;
  std::vector<Element> elements_;
  
  Grid(int num_elements_x, int num_elements_y, double element_width, double element_height, double grid_base_value){
    num_elements_x_ = num_elements_x;
    num_elements_y_ = num_elements_y;
    element_width_ = element_width;
    element_height_ = element_height;
    grid_base_value_= grid_base_value;
    double pos_x, pos_y;

    for(int col = 0; col < num_elements_y_; col++){
      for(int row = 0; row < num_elements_x_; row++){
        pos_x = element_width_/2 + col * element_width_;
        pos_y = element_height_/2 + row * element_height_;
        elements_.push_back(Element(row + num_elements_x_ * col + 1, grid_base_value_, pos_x, pos_y, element_width_, element_height_, 0));
      }
    }
  };
  ~Grid(){};
};

PointCartesian ConvertPolarToCartesian(PointPolar p);

/**
 * Returns the element number of a given cartesian point p(x,y)
 * 
 * Example:
 *
 *     * * * * * * * * * * * * * x
 *     *  1  *  2  *  3  *
 *     * * * * * * * * * * *
 *     *  4  *  5  *  6  *
 *     * * * * * * * * * * *
 *     *  7  *  8  *  9  *
 *     * * * * * * * * * * *
 *     *     *     *     *
 *     *
 *     y
**/
int ConvertCartesianToElementNumber(PointCartesian p);

int main(){
  Grid my_grid(3,2,1.0,1.0,101);
  for (Element e : my_grid.elements_){
    std::cout << e.id_ << " at (" << e.pos_x_ << ", " << e.pos_y_ << ") has a value of " << e.value_ << "\n";
  }
}

int ConvertCartesianToElementNumber(){

}
