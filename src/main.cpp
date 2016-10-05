// std library
#include <string.h>
#include <algorithm>
#include <iostream>

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
  cv::Ptr<cv::ximgproc::SuperpixelSLIC> slic_image;
  //

  // Apply the superpixel to the image:
  slic_image = cv::ximgproc::createSuperpixelSLIC(image_input, 101, 10, 10.0);
  slic_image->iterate(10);
  
  // Number of superpixel:
  int nb_superpixel = slic_image->getNumberOfSuperpixels();

  std::cout << " Number of superpixels is : " << nb_superpixel << std::endl;
  
  image_input.copyTo(image_result);

  // get the contours for displaying:
  slic_image->getLabelContourMask(mask, true);

  // Set mask to the result image:
  image_result.setTo(cv::Scalar(0, 0, 255), mask);

    // Show the result:
  const cv::Size display_size = cv::Size(800,600);
  display_matrix("Original", image_input,
		 display_size, true);
  display_matrix("Result", image_result,
		 display_size, true);
  cv::waitKey(0);
  return 0;
}
