/**
 * File   main.cpp
 * 
 * This code converts a 2D discrete function from Cartesian representation to 
 * Polar one using node interpolation.
 *   Input(s):
 *     - Grid:      Type PixelCenterGrid
 *     - Point:     Type PointPolar
 *   Output(s):
 *     - z value:   Type double Interpolated value
 * 
 *   1. Create a grid of pixels as shwon below
 *   2. Create an interpolation element
 *   3. Add element adjacency matrix
 *   4. Find element containing the given point
 * 
 *   // NOTE: This code does not support boundary elements.
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

// Class constructing a point in 2D Cartesian space
class PointCartesian{
 public:
  double x_;
  double y_;
  PointCartesian(double x, double y){
    x_ = x;
    y_ = y;
  };
};

// Class constructing a point in 2D Polar space
class PointPolar{
 public:
  double r_;
  double theta_;
  PointPolar(double r, double theta){
    double r_ = r;
    double theta_ = theta;
  }
};

// Class constructing a pixel element. 
class Element{
 public:
  Element(int id, double value, double pos_x, double pos_y, double width,
          double height, int adj_elements[]){
    id_ = id;
    value_ = value;
    pos_x_= pos_x;
    pos_y_= pos_y;
    width_ = width;
    height_ = height;
    for(int i=0; i < 8; i++){adj_elements_[i] = adj_elements[i];}
  };

  int id() const {return id_;}
  int x() const {return pos_x;}
  int y() const {return pos_y;}
  int adj_elements() const {return adj_elements_;}

 private:
  int id_;
  double value_;
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  int adj_elements_[8]; // Conventional matrix notaion excluding #5 (see header comment 1.1)
};

// Class constructing an interpolation element
class InterpolationElement{
 public:
  InterpolationElement(double pos_x, double pos_y, 
                       double width, double height, 
                       double node_values[], int node_ids[]){
    pos_x_ = pos_x;
    pos_y_ = pos_y;
    width_ = width;
    height_ = height;
    for (int i = 0; i < 4; i++ ){
      node_ids_[i] = node_ids[i];
      node_values_[i] = node_values[i];
    }
  };
  
  double ValueAt(double x, double y){
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

 private:
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  int node_ids[4];
  double node_values_[4];
};

// Class constructing a vector of pixel elements in a 2D grid
class PixelCenterGrid{
 public:
  std::vector<Element> pixels_;
  
  PixelCenterGrid(int n_pixels_x, int n_pixels_y, 
                  double pixel_width, double pixel_height, 
                  double grid_base_value){
    n_pixels_x_ = n_pixels_x;
    n_pixels_y_ = n_pixels_y;
    pixel_width_ = pixel_width;
    pixel_height_ = pixel_height;
    grid_base_value_= grid_base_value;
    
    double pos_x = pos_y = 0;
    for (int row = 0; row < n_pixels_y_; row++){
      for (int col = 0; col < n_pixels_x_; col++){
        pos_x = pixel_width_ / 2 + col * pixel_width_;
        pos_y = pixel_height_ / 2 + row * pixel_height_;
        pixels_.push_back(Element(col + n_pixels_x_ * row + 1, 
                                  grid_base_value_, 
                                  pos_x, pos_y, 
                                  pixel_width_, pixel_height_, 0));
                                  // TODO(SMY): Change the last arg. to an array
      }
    }
  };
  ~PixelCenterGrid(){};

 private:
  int n_pixels_x_;
  int n_pixels_y_;
  double pixel_width_;
  double pixel_height_;
  double grid_base_value_;
};

// Converts polar coordinate (r, theta) to cartesian coordinate (x, y)
PointCartesian ConvertPolarToCartesian(PointPolar pp){
   return PointCartesian(pp.r_ * cos(pp.theta_), pp.r_ * sin(pp.theta_));
}

// Returns the element number of a given cartesian point p(x, y) within a grid
int FindElementId(PointCartesian point, PixelCenterGrid grid){
  int id;
  int i = 0;
  double min_distance, distance;
  min_distance = grid.pixel_width_ + grid.pixel_height_;
  int pixel_id;
  for(auto p : grid.pixels_){
    distance = pow(point.x_ - grid.pixels_[pixel_id].pos_x_, 2) + 
               pow(point.y_ - grid.pixels_[pixel_id].pos_y_, 2);
    if(distance < min_distance){
      min_distance = distance;
      id = i;
    }
    i++;
  }
  // for(int j; j < grid.n_pixels_y_; j++){
  //   for(int i; i < grid.n_pixels_x_; i++){
  //     pixel_id = j + i;
  //     min_distance = pow(point.x_ - grid.pixels_[pixel_id].pos_x_, 2) + 
  //                    pow(point.y_ - grid.pixels_[pixel_id].pos_y_, 2);
  //   }
  //}
  return id;
};

template<typename T>
// Find the value within an element by interpolation
T FindValueAtCartesian(PointCartesian point, InterpolationElement element, 
                       T temp_val){
  // Test a single element
  int node_ids[4] = {101, 102, 103, 104};
  double node_values[4] = {101, 102, 103, 105};
  InterpolationElement test_element(1.0, 1.0, 4.0, 4.0, node_values, node_ids);
  return test_element.ValueAt(point.x_, point.y_);
}

int main(){
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

  PixelCenterGrid my_pixel_center_grid(3, 2, 1.0, 1.0, 101);

  for (Element pixel_center : my_pixel_center_grid.pixels_){
    std::cout << "Pixel " << pixel_center.id_ << " at (" 
              << pixel_center.pos_x_ << ", " << pixel_center.pos_y_ 
              << ") has a value of " << pixel_center.value_ << "\n";
  }


  //grid_elements = f(image)
  
  double arr1[] = {10, 20, 30, 40};
  int arr2[] = {1, 2, 3, 4};

  // Define a test point
  double r = 1;
  double theta = 1;
  PointPolar test_point_ploar(r, theta);
  PointCartesian test_point_xy = ConvertPolarToCartesian(test_point_ploar);

  // Find the element that contains the test point
  int element_id = FindElementId(test_point_xy, my_pixel_center_grid);

  //InterpolationElement test_element = InterpolationElement(1.0, 1.0, 1.0, 1.0, arr1, arr2);
  //double result = FindValueAtCartesian(test_point_xy, test_element, 99);
  double result = FindValueAtCartesian(test_point_xy, 
                  my_pixel_center_grid.pixels_[element_id], 99);
  std::cout << "The value at (r, theta) = (" << r << ", " << theta <<  ") is "
            << result << "\n";
}