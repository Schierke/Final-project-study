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

namespace boost_opts = boost::program_options;

int main(int argc, char **argv) {
  
  // Input image:
  cv::Mat image_input = cv::imread(argv[1], cv::IMREAD_COLOR);
  cv::Mat image_result;
  image_result = image_input;

  // Show the result:
  cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
  cv::imshow("Result", image_input);
  cv::waitKey(0);
  return 0;
}
