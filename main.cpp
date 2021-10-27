/**
 * File   main.cpp
 * 
 * This code converts a 2D discrete function from Cartesian representation to Polar 
 * one using node interpolation.
 * 
 *   1. Grid configuration:
 *   2. Element numbering
 *   3. Node numbering
 *   4. Element adjacency matrix
 *   5. Node adjacency matrix
 * 
 * NOTE: This code does not support boundary elements.
 *                                            
 *                                            
 *   > _____V______V______V_______    0---x
 *    |  |      |      |      |  |    |
 *    - C01 -- C02 -- C03 -- C04 -    y       _______
 *   >|  |  E1  |  E2  |  E3  |  |           |       |
 *    - C05 -- C06 -- C07 -- C08 -           |   0---|---a
 *   >|  |  E4  |  E5  |  E6  |  |           |___|___|
 *    - C09 -- C10 -- C11 -- C12 -               |
 *   >|  |      |      |      |  |               |
 *                                               b
 *    E1: 1, 2, 6, 5  
 *    E2: 2, 3, 7, 6
 * 
 **/

#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <cmath>

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

class ElementSecondary{
 public:
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  int node_ids[4];
  double node_values_[4];;
  ElementSecondary(double pos_x, double pos_y, double width, double height, double node_values[], int node_ids[]){
    pos_x_ = pos_x;
    pos_y_ = pos_y;
    width_ = width;
    height_ = height;
    //node_ids_ = node_ids
    for (int i = 0; i < 4; i++ ){
      node_values_[i] = node_values[i];
    }
  };
  double valueAt(double x, double y){
    // Normalization
    double a = (x - pos_x_) / (width_/2);
    double b = (y - pos_y_) / (height_/2);

    // Shape functions
    double w1 = 0.25 * (1 - a) * (1 - b);
    double w2 = 0.25 * (1 + a) * (1 - b);
    double w3 = 0.25 * (1 + a) * (1 + b);
    double w4 = 0.25 * (1 - a) * (1 + b);
    return w1 * node_values_[0] + w2 * node_values_[1] + w3 * node_values_[2] +
     w4 * node_values_[3];
  }
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

// Converts polar coordinate (r, theta) to cartesian coordinate (x, y)
PointCartesian ConvertPolarToCartesian(PointPolar pp){
   return PointCartesian(pp.r_ * cos(pp.theta_), pp.r_ * sin(pp.theta_));
}

// Returns the element number of a given cartesian point p(x, y) with in a grid
int FindElemetId();

template<typename T>
// Find the value within an element by interpolation
T FindValueAtCartesian(double x, double y, T temp_val){
  // Test a single element
  int node_ids[4] = {101, 102, 103, 104};
  double node_values[4] = {101, 102, 103, 105};
  ElementSecondary temp(1.0, 1.0, 4.0, 4.0, node_values, node_ids);
  return temp.valueAt(x, y);
}

int main(){
  PixelCenterGrid my_pixel_center_grid(3, 2, 1.0, 1.0, 101);
  for (Element pixel_center : my_pixel_center_grid.pixels_){
    std::cout << "Pixel " << pixel_center.id_ << " at (" << pixel_center.pos_x_ << ", " << pixel_center.pos_y_ << ") has a value of " << pixel_center.value_ << "\n";
  }
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

  //grid_elements = f(image)
  //ConvertPolarToCartesian();
  //FindElemetId(grid_elements, x,y)
  FindValueAtCartesian(1.0, 1.0, 99);
}