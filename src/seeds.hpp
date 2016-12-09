/**
 * @file   seeds.hpp
 * @author Tien Manh <tien.nguyen@insa-cvl.fr>
 * @date   Mon October 10th 2016 21:59:00
 * 
 * @brief  Object to initialize the superpixel SEEDS
 *         
 *
 */
#ifndef SEEDS_HPP_
#define SEEDS_HPP_

// openCV include
#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc/seeds.hpp"
#include "opencv2/core/core.hpp"

class Seeds{
public:
  Seeds(const int width = 400,
	const int height = 400,
	const int image_channels = 3,
	const int num_superpixels = 400,
	const int num_levels = 4,
	const int prior = 2,
	const int num_histogram_bins = 5,
	const bool double_step = false,
	const int iteration = 4); 
  cv::Mat extractSuperPixelMask(const cv::Mat & image_input);
  cv::Mat applySuperPixel(cv::Mat image_input,
			  cv::Mat image_mask) const;
  int getNumberOfSuperpixels();
private:
  cv::Ptr<cv::ximgproc::SuperpixelSEEDS> seeds;
  int width;
  int height;
  int image_channels;
  int num_superpixels;
  int num_levels;
  int prior;
  int num_histogram_bins;
  bool double_step;
  int iteration;
};

#endif // SEEDS_HPP_
