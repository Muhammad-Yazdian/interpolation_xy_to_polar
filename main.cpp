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

class PixelCenterGrid{
 public:
  int n_pixels_x_;
  int n_pixels_y_;
  double pixel_width_;
  double pixel_height_;
  double grid_base_value_;
  std::vector<Element> pixels_;
  
  PixelCenterGrid(int n_pixels_x, int n_pixels_y, double pixel_width, double pixel_height, double grid_base_value){
    n_pixels_x_ = n_pixels_x;
    n_pixels_y_ = n_pixels_y;
    pixel_width_ = pixel_width;
    pixel_height_ = pixel_height;
    grid_base_value_= grid_base_value;
    double pos_x, pos_y;

    for(int col = 0; col < n_pixels_y_; col++){
      for(int row = 0; row < n_pixels_x_; row++){
        pos_x = pixel_width_/2 + col * pixel_width_;
        pos_y = pixel_height_/2 + row * pixel_height_;
        pixels_.push_back(Element(row + n_pixels_x_ * col + 1, grid_base_value_, pos_x, pos_y, pixel_width_, pixel_height_, 0));
      }
    }
  };
  ~PixelCenterGrid(){};
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
 * 
 *   > _____V______V______V_______   0---x      0---a
 *    |  |      |      |      |  |   |          |
 *    - C01 -- C02 -- C03 -- C04 -   y          b
 *   >|  |  E1  |  E2  |  E3  |  |   
 *    - C05 -- C06 -- C07 -- C08 -   
 *   >|  |  E4  |  E5  |  E6  |  |
 *    - C09 -- C10 -- C11 -- C12 -
 *   >|  |      |      |      |  |
 * 
 * 
 * E1: 1,2,6,5
 * E2: 2,3,7,6
 * En: n,n+1, d * w + 1, d*w 
 * 
 * 
**/
int ConvertCartesianToElementNumber(PointCartesian p);


class ElementSecondary{
 public:
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  //int node_ids[4];
  double node_values_[4];;
  ElementSecondary(double pos_x, double pos_y, double width, double height, double node_values, int node_ids){
    pos_x_ = pos_x;
    pos_y_ = pos_y;
    width_ = width;
    height_ = height;
    //node_ids_ = node_ids
    for (int i = 0; i < 4; i++ ){
      node_values_[i] = 101+i;
    }
  };
  double valueAt(double x, double y){
    double a = (x - pos_x_) / (width_/2);
    double b = (y - pos_y_) / (height_/2);
    double w1 = 0.25 * (1 - a) * (1 - b);
    double w2 = 0.25 * (1 + a) * (1 - b);
    double w3 = 0.25 * (1 + a) * (1 + b);
    double w4 = 0.25 * (1 - a) * (1 + b);
    return w1 * node_values_[0] + w2 * node_values_[1] + w3 * node_values_[2] + w4 * node_values_[3];
  }
  
};


int main(){
  PixelCenterGrid my_pixel_center_grid(3,2,1.0,1.0,101);
  for (Element pixel_center : my_pixel_center_grid.pixels_){
    std::cout << "Pixel " << pixel_center.id_ << " at (" << pixel_center.pos_x_ << ", " << pixel_center.pos_y_ << ") has a value of " << pixel_center.value_ << "\n";
  }

  // Test a single element
  ElementSecondary temp(1.0, 1.0, 4.0, 4.0, 0, 0);
  std::cout << temp.valueAt(1.0, 1.0) << std::endl;
  std::cout << temp.valueAt(3.0, 3.0) << std::endl;

  // Load a image
  double image[10][10] = {
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0},
      {1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}
  };

  // r = 1;
  // theta = 1;
  // x = fx(r, theta);
  // y = fy(r, theta);
  // element_id = g(grid_elements, x,y);
  // z_value_at_xy = h(grid_elements.elements[element_id], x,y);

}

int ConvertCartesianToElementNumber(){

}


