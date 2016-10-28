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

  // Init for image option and video option:
  std::string NO_IMAGE="";
  std::string NO_VIDEO="";
  
  boost_opts::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("image", boost_opts::value<std::string>()->default_value(NO_IMAGE), " the image that needs analyze")
    ("video", boost_opts::value<std::string>()->default_value(NO_VIDEO), " the video that needs analyze")
    ("algorithm", boost_opts::value<int>()->default_value(101), " the algorithm that is going to be used: 100 - SLIC, 101 - SLICO")
    ("region_size", boost_opts::value<int>()->default_value(50), " size of the region of superpixel")
    ("ruler", boost_opts::value<float>()->default_value(30), " compactness of the superpixel")
    ("iteration", boost_opts::value<int>()->default_value(3), " the amount of iteration that superpixelSLIC algorithm needs to perform")
    ;
  
  boost_opts::variables_map variables_map;
  boost_opts::store(boost_opts::parse_command_line(argc, argv, desc), variables_map);
  boost_opts::notify(variables_map);

  std::string image_filename = variables_map["image"].as<std::string>();
  std::string video_filename = variables_map["video"].as<std::string>();
  int algorithm = variables_map["algorithm"].as<int>();
  int region_size = variables_map["region_size"].as<int>();
  float ruler = variables_map["ruler"].as<float>();
  int iteration = variables_map["iteration"].as<int>();

  // Initialisation:
  cv::VideoCapture capture_video;
  cv::Mat image_input = cv::imread(image_filename, cv::IMREAD_COLOR);
  cv::Mat image_result;
  cv::Mat image_hsv;
  cv::Mat mask;
  double tp1, tp2;
  
  // If we choose the image to analyze:
  if(image_filename != NO_IMAGE) {

    // save the start timer1:
    tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    // Convert the image to HSV:
    cv::cvtColor(image_input, image_hsv, CV_RGB2HSV);
   
    // Intialize SuperpixelSLIC object
    Superpixel sp_slic(algorithm, region_size, ruler, iteration);

    // apply super pixel mask:
    mask = sp_slic.extractSuperPixelMask(image_hsv);
    
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
  }
  // If we choose the video to analyze:
  else if(video_filename != NO_VIDEO) {
    // Intialize SuperpixelSLIC object
    Superpixel sp_slic(algorithm, region_size, ruler, iteration);
    capture_video.open(video_filename);
    cv::Mat frame;

    while(1) {
      // save the start timer1:
      tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      if(!capture_video.read(frame)) {
	break;
      }
      capture_video >> frame;
      cv::resize(frame, frame, cv::Size(360, 240), 0, 0, cv::INTER_CUBIC);

      cv::cvtColor(frame, image_hsv, CV_RGB2HSV);

      // apply super pixel mask:
      mask = sp_slic.extractSuperPixelMask(image_hsv);
      // get the superpixel image:
      image_result = sp_slic.applySuperPixel(frame, mask);
   
      // Number of superpixel:
      std::cout << " Number of superpixels is : " << sp_slic.getNumberOfSuperpixels()
		<< std::endl;
      
      tp2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      
      // Calculate execution time:
      std::cout <<" Execution time : " << tp2-tp1 << " ms." << std::endl; 
  
      // Show the result:
      const cv::Size display_size = cv::Size(360,240);
      //display_matrix("Original", frame,
      //		     display_size, true);
      display_matrix("Result", image_result,
		     display_size, true);
      cv::waitKey(20);
    }
  }
  else {
    std::cout<<" CAN'T GET ANY VIDEO OR IMAGE PROVIDED!!!" << std::endl;
  }
  return 0;
}
