/**
 * File   main.cpp
 * 
 * This code converts a 2D discrete function from Cartesian representation to 
 * Polar one using node interpolation.
 * 
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
 *    Ei: i, i+1, i+1+n_x, i+n_x 
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
  PointCartesian(double x, double y):x_(x),y_(y){ }
};

// Class constructing a point in 2D Polar space
class PointPolar{
 public:
  double r_;
  double theta_;
  PointPolar(double r, double theta): r_(r), theta_(theta){ }
};

// Class constructing a pixel element. 
/* REMOVE(SMY): PixelElement is not useful.
class PixelElement{
 public:
  PixelElement(int id, 
               double value, 
               double pos_x, 
               double pos_y, 
               double width, 
               double height, 
               int adj_elements[])
    : id_(id),
      value_(value),
      pos_x_(pos_x),
      pos_y_(pos_y),
      width_(width),
      height_(height)
  {
    //Set adj_elements_
    //for(int i=0; i<8; i++){adj_elements_[i] = adj_elements[i];} 
  }

  int id() const {return id_;}
  int x() const {return pos_x_;}
  int y() const {return pos_y_;}
  double value() const {return value_;}
  // Match types
  //int adj_elements() const {return adj_elements_;} 

 private:
  int id_;
  double value_;
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  int adj_elements_[8]; // From a 3x3 matrix excluding #5 (see header comment)
};
*/

// Class constructing an interpolation element
class InterpolationElement{
 public:
  InterpolationElement(double pos_x, 
                       double pos_y,
                       double width, 
                       double height, 
                       double node_values[], 
                       int node_ids[])
    : pos_x_(pos_x),
      pos_y_(pos_y),
      width_(width),
      height_(height)
  {
    for (int i=0; i<4; i++){
      node_ids_[i] = node_ids[i];
      node_values_[i] = node_values[i];
    }
  }
  
  double ValueAt(double x, double y){
    // Normalization
    double a = (x - pos_x_) / (width_/2);
    double b = (y - pos_y_) / (height_/2);

    // Shape functions
    double w1 = 0.25 * (1 - a) * (1 - b);
    double w2 = 0.25 * (1 + a) * (1 - b);
    double w3 = 0.25 * (1 + a) * (1 + b);
    double w4 = 0.25 * (1 - a) * (1 + b);
    
    return w1 * node_values_[0] + 
           w2 * node_values_[1] + 
           w3 * node_values_[2] + 
           w4 * node_values_[3];
  }
  
  double x(){return pos_x_;}
  double y(){return pos_y_;}

 private:
  double pos_x_;
  double pos_y_;
  double width_;
  double height_;
  int node_ids_[4];
  double node_values_[4];
};

// Class constructing a 2D grid based on the center of pixels
/* REMOVE(SMY): PixelGrid is not useful.
class PixelGrid{
 public:
  // Better to move it to private and call a get function
  std::vector<PixelElement> pixels_; 
  
  PixelGrid(int n_pixels_x, 
            int n_pixels_y, 
            double pixel_width, 
            double pixel_height, 
            double grid_base_value)
    : n_pixels_x_(n_pixels_x),
      n_pixels_y_(n_pixels_y),
      pixel_width_(pixel_width),
      pixel_height_(pixel_height),
      grid_base_value_(grid_base_value)
  {
    double pos_x = 0;
    double pos_y = 0;
    for (int row = 0; row < n_pixels_y_; row++){
      for (int col = 0; col < n_pixels_x_; col++){
        pos_x = pixel_width_ / 2 + col * pixel_width_;
        pos_y = pixel_height_ / 2 + row * pixel_height_;
        pixels_.push_back(PixelElement(col + n_pixels_x_ * row + 1, 
                                       grid_base_value_, 
                                       pos_x, 
                                       pos_y, 
                                       pixel_width_, 
                                       pixel_height_, 
                                       0));
        // Change the last arg. to an array
      }
    }
  };
  ~PixelGrid(){};

  double pixel_width(){return pixel_width_;}
  double pixel_height(){return pixel_height_;}
  double n_pixels_x(){return n_pixels_x_;}
  double n_pixels_y(){return n_pixels_y_;}

 private:
  int n_pixels_x_;
  int n_pixels_y_;
  double pixel_width_;
  double pixel_height_;
  double grid_base_value_;
};
*/

// Class constructing a 2D grid based on the interpolated elements
class InterpolatedGrid{
 public:
  // Create an interpolated grid form an array
  InterpolatedGrid(double img_arr[],
                   int n_pixels_x,
                   int n_pixels_y, 
                   double pixel_width, 
                   double pixel_height)
    :  pixel_width_(pixel_width),
       pixel_height_(pixel_height)
  {
    double pos_x = 0;
    double pos_y = 0;
    double node_values_array[4];
    int node_ids_array[4];
    int i = 0;
    for (int row = 0; row < n_pixels_y - 1 ; row++){
      for (int col = 0; col < n_pixels_x - 1 ; col++){
        pos_x = (1 + col) * pixel_width_;
        pos_y = (1 + row) * pixel_height_;
        i = row * n_pixels_x + col;
        node_values_array[0] = img_arr[i];
        node_values_array[1] = img_arr[i + 1];
        node_values_array[2] = img_arr[i + n_pixels_x + 1];
        node_values_array[3] = img_arr[i + n_pixels_x];
        node_ids_array[0] = i;
        node_ids_array[1] = i + 1;
        node_ids_array[2] = i + n_pixels_x + 1;
        node_ids_array[3] = i + n_pixels_x;
        elements_.push_back(InterpolationElement(pos_x,
                                                 pos_y,
                                                 pixel_width_,
                                                 pixel_height_,
                                                 node_values_array,
                                                 node_ids_array));
      }
    }
  }

  // Create an interpolated grid form a file
  // InterpolatedGrid(file) //TODO: Create grid form a text file

  /* REMOVE(SMY): Pixel grid is not useful.
  // Create an interpolated grid form a pixel grid
  InterpolatedGrid(PixelGrid pixel_grid)
    : pixel_width_(pixel_grid.pixel_width()),
      pixel_height_(pixel_grid.pixel_height())
  {
    double pos_x = 0;
    double pos_y = 0;
    for (int row = 0; row < pixel_grid.n_pixels_y() - 1 ; row++){
      for (int col = 0; col < pixel_grid.n_pixels_x() - 1 ; col++){
        pos_x = (1 + col) * pixel_width_;
        pos_y = (1 + row) * pixel_height_;
        double node_values_array[4]={1,2,3,4}; //FIX: Use actual values
        int node_ids_array[4]={1,2,3,4};      // FIX: Use actual values
        elements_.push_back(InterpolationElement(pos_x, 
                                                 pos_y, 
                                                 pixel_width_, 
                                                 pixel_height_, 
                                                 node_values_array, 
                                                 node_ids_array)); 
        // Change the last arg. to an array
      }
    }
  }
  */
  ~InterpolatedGrid(){};

  // Returns the id of element containing the given point p(x, y)
  // Returning zero means the point is outside of the grid (about 2 pixel size)
  // TODO: Improve this algorithm by smart search using ajd_elemets
  int FindElementId(PointCartesian point){
    int id = 0;
    double distance = 0;
    // NOTE: Use a large enough value to start camparison.
    double min_distance = pixel_width_ + pixel_height_; 

    int count = 0;
    for (auto element : elements_){
      distance = pow(point.x_ - element.x(), 2) +
                 pow(point.y_ - element.y(), 2);
      if (distance < min_distance){
        min_distance = distance;
        id = count;
      }
      count++;
    }
    return id;
  }

  double FindValueAt(PointCartesian point){
    return elements_[FindElementId(point)].ValueAt(point.x_, point.y_);
  }

  template <typename T>
  T FindGenericValueAt(PointCartesian point){
    // TODO: Use a template to support generic output value
    return elements_[FindElementId(point)].ValueAt(point.x_, point.y_);
  }

 private:
  double pixel_width_;
  double pixel_height_;
  std::vector<InterpolationElement> elements_; 
};

// Converts polar coordinate (r, theta) to cartesian coordinate (x, y)
PointCartesian ConvertPolarToCartesian(PointPolar polar_point){
   return PointCartesian(polar_point.r_ * cos(polar_point.theta_), 
                         polar_point.r_ * sin(polar_point.theta_));
}

// TODO: Use this template to support generic output type
template<typename T>
// Find the value within an element by interpolation
T FindValueAtCartesian(PointCartesian point, 
                       InterpolationElement element, 
                       T temp_val){
  // Test a single element
  int node_ids[4] = {101, 102, 103, 104}; //FIX: Use actual data
  double node_values[4] = {101, 102, 103, 105}; //FIX: Use actual data
  InterpolationElement test_element(1.0, 1.0, 4.0, 4.0, node_values, node_ids);
  return test_element.ValueAt(point.x_, point.y_);
}

int main(){
  // TODO(SMY): Load image file and convert it to an array of use 
  // load file constructor

  // Test image 2D Array
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

  // Test image 1D Array
  int image2_n_pixels_x = 10;
  int image2_n_pixels_y = 10;
  double image2_pixel_width = 1;
  double image2_pixel_height = 1;
  double image2[100] = {
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0
  };

  InterpolatedGrid my_inter_grid(image2, 
                                 image2_n_pixels_x, 
                                 image2_n_pixels_y,
                                 image2_pixel_width,
                                 image2_pixel_height);

  // Define a test point
  PointPolar test_point_ploar(0.0, 0.0);
  PointCartesian test_point_xy(0.0, 0.0);
  
  double r = 5.0;
  double theta = 0;
  int id = 0;
  double value = 0;
  for (theta = 0; theta < 1.5; theta = theta + 0.05){
    test_point_ploar = PointPolar(r, theta);
    test_point_xy = ConvertPolarToCartesian(test_point_ploar);
    id = my_inter_grid.FindElementId(test_point_xy);
    //value = my_inter_grid.FindValueAt(test_point_xy);
    value = my_inter_grid.FindGenericValueAt<double>(test_point_xy);
    
    // Results
    std::cout << "Point (x, y)=(" 
              << test_point_xy.x_ << ", " 
              << test_point_xy.y_ << ") " 
              << "is on element #" << id
              << ". The interpolated z value is " << value << ".\n\n";
  }
}