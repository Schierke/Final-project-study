#include <string.h>
#include <chrono>

// SuperpixelSEEDS
#include "seeds.hpp"

Seeds::Seeds(const int width,
	     const int height,
	     const int image_channels,
	     const int num_superpixels,
	     const int num_levels,
	     const int prior,
	     const int num_histogram_bins,
	     const bool double_step,
	     const int iteration):
  width(width),
  height(height),
  image_channels(image_channels),
  num_superpixels(num_superpixels),
  num_levels(num_levels),
  prior(prior),
  num_histogram_bins(num_histogram_bins),
  double_step(double_step),
  iteration(iteration){};

  
cv::Mat Seeds::extractSuperPixelMask(const cv::Mat & image_input) {
  cv::Mat img_hsv;
  cv::Mat mask;

  // Convert the image input to HSV:
  cv::cvtColor(image_input, img_hsv, CV_RGB2HSV);
  
  seeds = cv::ximgproc::createSuperpixelSEEDS(width, height, image_channels, num_superpixels,
				    num_levels, prior, num_histogram_bins,
				    double_step);
  seeds->iterate(img_hsv, iteration);

  // return the mask contour:
  seeds->getLabelContourMask(mask, false);
  return mask;
}

cv::Mat Seeds::applySuperPixel(cv::Mat image_input,
			       cv::Mat image_mask) const {
  cv::Mat image_result;
  image_input.copyTo(image_result);
  
  // set mask:

  image_result.setTo(cv::Scalar(0, 0, 255), image_mask);
  return image_result;
}

int Seeds::getNumberOfSuperpixels() {
  return seeds->getNumberOfSuperpixels();
}
