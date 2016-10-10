// superpixel project's libraries
#include "slic.hpp"

// std library
#include <string.h>
#include <algorithm>
#include <iostream>
#include <chrono>
// boost 
#include <boost/program_options.hpp>
#include <boost/numeric/interval.hpp>

// openCV
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ximgproc.hpp"
#include "opencv2/ximgproc/slic.hpp"
#include "opencv2/core/core.hpp"

namespace boost_opts = boost::program_options;

// Display matrix:
void display_matrix(const std::string window_name,
		    const cv::Mat window_matrix,
		    const cv::Size display_size,
		    const bool resize) {
  if(resize) {
    cv::Mat window_display;
    cv::resize(window_matrix, window_display, display_size);
    cv::imshow(window_name, window_display);
  }
  else {
    cv::imshow(window_name, window_matrix);
  }
}
 
int main(int argc, char **argv) {
  
  if(argc !=2) {
    std::cerr << "Please provide the image" << std::endl;
    return -1;
  }
  
  // Initialisation:
  cv::Mat image_input = cv::imread(argv[1], cv::IMREAD_COLOR);
  cv::Mat image_result;
  cv::Mat mask;
  double tp1, tp2;
  
  // save the start timer1:
  tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  // Intialize SuperpixelSLIC object
  Superpixel sp_slic(101, 20, 10.0, 10);

  // apply super pixel mask:
  mask = sp_slic.extractSuperPixelMask(image_input);

  // get the superpixel image:
  image_result = sp_slic.applySuperPixel(image_input, mask);
   
  // Number of superpixel:
  std::cout << " Number of superpixels is : " << sp_slic.getNumberOfSuperpixels()
	    << std::endl;
  
  tp2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  // Calculate execution time:
  std::cout <<" Execution time : " << tp2-tp1 << " ms." << std::endl; 
  
  // Show the result:
  const cv::Size display_size = cv::Size(800,600);
  display_matrix("Original", image_input,
		 display_size, true);
  display_matrix("Result", image_result,
		 display_size, true);
  cv::waitKey(0);
  return 0;
}
