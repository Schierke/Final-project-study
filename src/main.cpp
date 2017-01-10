// superpixel project's libraries
#include "slic.hpp"
#include "seeds.hpp"

// std library
#include <string.h>
//#include <stdeexcept>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
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


void read_video(cv::VideoCapture capture_video,  cv::Mat & frame) {
  capture_video >> frame;

}



int main(int argc, char **argv) {

  // Init for image option and video option:
  std::string NO_IMAGE="";
  std::string NO_VIDEO="";
  
  boost_opts::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("superpixel_algo", boost_opts::value<int>()->default_value(0), " Choose the superpixel algorithm that will be using: 0 is SLIC while 1 is SEEDS")
    ("image", boost_opts::value<std::string>()->default_value(NO_IMAGE), " the image that needs analyze")
    ("video", boost_opts::value<std::string>()->default_value(NO_VIDEO), " the video that needs analyze")
    ("algorithm", boost_opts::value<int>()->default_value(0), " the algorithm that is going to be used: 0 - SLIC, 1 - SLICO")
    ("region_size", boost_opts::value<int>()->default_value(50), " size of the region of superpixel")
    ("ruler", boost_opts::value<int>()->default_value(10), " compactness of the superpixel")
    ("min_element_size", boost_opts::value<int>()->default_value(10), " the minimum value to enforce connectivity")
    ("iteration", boost_opts::value<int>()->default_value(10), " the amount of iteration that superpixelSLIC algorithm needs to perform")
    ("num_superpixels", boost_opts::value<int>()->default_value(400), " the number of superpixel using SEEDS")
    ("num_levels", boost_opts::value<int>()->default_value(4), " number of levels using SEEDS")
    ("prior", boost_opts::value<int>()->default_value(2), " numbe of prior using SEEDS")
    ("num_histogram_bins", boost_opts::value<int>()->default_value(5), " number of histogram using SEEDS")
    ("double_step", boost_opts::value<bool>()->default_value(false), "using double step or not?")
    ;
  

  // Intitialize : take the input values:
  boost_opts::variables_map variables_map;
  boost_opts::store(boost_opts::parse_command_line(argc, argv, desc), variables_map);
  boost_opts::notify(variables_map);

  std::string image_filename = variables_map["image"].as<std::string>();
  std::string video_filename = variables_map["video"].as<std::string>();
  int superpixel_algo = variables_map["superpixel_algo"].as<int>();
  int algorithm = variables_map["algorithm"].as<int>();
  int region_size = variables_map["region_size"].as<int>();
  int ruler = variables_map["ruler"].as<int>();
  int min_element_size = variables_map["min_element_size"].as<int>();
  int iteration = variables_map["iteration"].as<int>();
  int num_superpixels = variables_map["num_superpixels"].as<int>();
  int num_levels = variables_map["num_levels"].as<int>();
  int prior = variables_map["prior"].as<int>();
  int num_histogram_bins = variables_map["num_histogram_bins"].as<int>();
  bool double_step = variables_map["double_step"].as<bool>();
  
  // Initialisation:
  cv::VideoCapture capture_video;
  cv::Mat image_hsv;
  cv::Mat mask;
  cv::Mat image_result;
  cv::Mat image_result_2;
  double tp1, tp2;
  // if we using the superpixel algorithm SLIC:
  if(superpixel_algo == 0) {
  
    // If we choose the image to analyze:
    if(image_filename != NO_IMAGE) {
      
      // get the input image and resize it:
      cv::Mat image_input = cv::imread(image_filename, cv::IMREAD_COLOR);
      
      //cv::Size default_size = image_input.size();

       cv::resize(image_input, image_input, cv::Size(400,400));
      

      // save the start timer1:
      tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
      // Convert the image to CIELAB space color:
      cv::cvtColor(image_input, image_hsv, CV_RGB2Luv);
   
      // Intialize SuperpixelSLIC object
      Superpixel sp_slic(algorithm, region_size, ruler, iteration, min_element_size);
    
      // apply super pixel mask:
      mask = sp_slic.extractSuperPixelMask(image_hsv);
    
      // get the superpixel image:
       image_result = sp_slic.applySuperPixel(image_input, mask);

      // get the superpixel region image:
      image_result_2 = sp_slic.applyPixelRegion(mask);
   
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
      display_matrix("Result_2", image_result_2,
		     display_size, true);
      cv::waitKey(0);
    }


    // If we choose the video to analyze:
    else if(video_filename != NO_VIDEO) {
    

      static const char * window_name = "SLIC Superpixels";
      // create a window for trackbar:
      cv::namedWindow(window_name, 0 );
      cv::createTrackbar("Algorithm", window_name, &algorithm, 1, 0);
      cv::createTrackbar("Region size", window_name, &region_size, 100, 0);
      cv::createTrackbar("Ruler", window_name, &ruler, 40, 0);
      cv::createTrackbar("Connectivity", window_name, &min_element_size, 100, 0);
      cv::createTrackbar("Iterations", window_name, &iteration, 12, 0);
      
      
      // Intialize SuperpixelSLIC object
      capture_video.open(video_filename);
      
      cv::Mat frame;
      
      int iterator_frame = 0;

      // Take the first frame to analyzing:
      capture_video >> frame;
      
      cv::cvtColor(frame, image_hsv, CV_RGB2Luv);
      while(1) {
	Superpixel sp_slic(algorithm, region_size, ruler, iteration, min_element_size);

	// save the start timer1:
	tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if(!capture_video.read(frame)) {
	  break;
	}
      
	std::thread video_thread(read_video, capture_video, std::ref(frame));
	
	// Convert image to CIELAB color space:
	cv::cvtColor(frame, image_hsv, CV_RGB2Luv);

	// apply super pixel mask:
	mask = sp_slic.extractSuperPixelMask(image_hsv);
	// get the superpixel image:
	image_result = sp_slic.applySuperPixel(frame, mask);
	
	// Number of superpixel:
	std::cout << " Number of superpixels is : " << sp_slic.getNumberOfSuperpixels()
		  << std::endl;
	video_thread.join();

	tp2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      
	// Calculate execution time:
	std::cout <<" Execution time : " << tp2-tp1 << " ms." << std::endl; 

	// save the video to the result folder:
	cv::imwrite("../result/" + std::to_string(iterator_frame)+ ".png",image_result);
	
	iterator_frame ++ ;
	// Show the result:
	const cv::Size display_size = cv::Size(600,400);
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
  }
  // if we using the SEED algorithm:
  else if(superpixel_algo == 1) {
        if(image_filename != NO_IMAGE) {
	  cv::Mat image_input = cv::imread(image_filename, cv::IMREAD_COLOR);

	  // save the start timer1:
	  tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
	  // Convert the image to hsv :
	  cv::cvtColor(image_input, image_hsv, CV_RGB2HSV);
      
   
	  // Intialize SuperpixelSLIC object
	  Seeds seeds(image_hsv.size().width,
		      image_hsv.size().height,
		      image_hsv.channels(),
		      num_superpixels,
		      num_levels,
		      prior,
		      num_histogram_bins,
		      double_step,
		      iteration);
      
    
	  // apply super pixel mask:
	  mask = seeds.extractSuperPixelMask(image_hsv);
	  
	  // get the superpixel image:
	  image_result = seeds.applySuperPixel(image_input, mask);
	  
	  // Number of superpixel:
	  std::cout << " Number of superpixels is : " << seeds.getNumberOfSuperpixels()
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
	else if(video_filename != NO_VIDEO) {
    

	  static const char * window_name = "SEEDS Superpixels";
	  // create a window for trackbar:
	  cv::namedWindow(window_name, 0 );
	  cv::createTrackbar("Nb of Superpixels", window_name, &num_superpixels, 1000, 0);
	  cv::createTrackbar("Number of levels", window_name, &num_levels, 10, 0);
	  cv::createTrackbar("Smoothing Prior", window_name, &prior, 5, 0);
	  cv::createTrackbar("Iterations", window_name, &iteration, 12, 0);
      
      
	  // Intialize SuperpixelSLIC object
	  capture_video.open(video_filename);
      
	  cv::Mat frame;
      
	  int iterator_frame = 0;

	  // Take the first frame to analyzing:
	  capture_video >> frame;
      
	  cv::cvtColor(frame, image_hsv, CV_RGB2HSV);
	  while(1) {
	    Seeds seeds(image_hsv.size().width,
			image_hsv.size().height,
			image_hsv.channels(),
			num_superpixels,
			num_levels,
			prior,
			num_histogram_bins,
			double_step,
			iteration);

	    // save the start timer1:
	    tp1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	    if(!capture_video.read(frame)) {
	      break;
	    }
      
	    std::thread video_thread(read_video, capture_video, std::ref(frame));
	    
	    // Convert image to HSV:
	    cv::cvtColor(frame, image_hsv, CV_RGB2HSV);
	    
	    // apply super pixel mask:
	    mask = seeds.extractSuperPixelMask(image_hsv);
	    // get the superpixel image:
	    image_result = seeds.applySuperPixel(frame, mask);
	    
	    // Number of superpixel:
	    std::cout << " Number of superpixels is : " << seeds.getNumberOfSuperpixels()
		      << std::endl;
	    video_thread.join();

	    tp2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	    
	    // Calculate execution time:
	    std::cout <<" Execution time : " << tp2-tp1 << " ms." << std::endl; 
	    
	    // save the video to the result folder:
	    cv::imwrite("../result/" + std::to_string(iterator_frame)+ ".png",image_result);
	    
	    iterator_frame ++ ;
	    // Show the result:
	    const cv::Size display_size = cv::Size(600,400);
	    //display_matrix("Original", frame,
	    //		     display_size, true);
	    display_matrix("Result", image_result,
			   display_size, true);
      
	    cv::waitKey(20);
	  }
	}
	else {
	  std::cout << " CAN'T GET ANY VIDEO OR IMAGE PROVIDED!!!" << std::endl;
	}
  }
  else std::cout <<" CHOOSE THE SUPERPIXEL ALGORITHM!! " << std::endl;
  return 0;
}
