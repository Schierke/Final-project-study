// std
#include <string.h>
#include <chrono>

// Superpixel
#include "slic.hpp"

Superpixel::Superpixel(const int algorithm,
		       const int region_size,
		       const float ruler,
		       const int iteration):
  algorithm(algorithm),
  region_size(region_size),
  ruler(ruler),
  iteration(iteration){};

cv::Mat Superpixel::extractSuperPixelMask(const cv::Mat & image_input) {
 
  cv::Mat mask;
  
  // Convert the image input to HSV:

  slic_image = cv::ximgproc::createSuperpixelSLIC(image_input, algorithm, 
						  region_size, ruler);
  
  slic_image->iterate(10);
  
  // return the mask contour:
  slic_image->getLabelContourMask(mask, true);
  
  return mask;
}

cv::Mat Superpixel::applySuperPixel(cv::Mat  image_input,
				    cv::Mat  image_mask
				    ) const {
  cv::Mat image_result;
  image_input.copyTo(image_result);
 // set mask :
  image_result.setTo(cv::Scalar(0, 0, 255), image_mask);
  return image_result;
}

int Superpixel::getNumberOfSuperpixels() {
  return slic_image->getNumberOfSuperpixels();
}
  
  
