/**
 * @file   slic.hpp
 * @author Tien Manh <tien.nguyen@insa-cvl.fr>
 * @date   Mon October 10th 2016 21:59:00
 * 
 * @brief  Object to initialize the superpixel SLIC
 *         
 *
 */
#ifndef SLIC_HPP_
#define SLIC_HPP_


// openCV include
#include "opencv2/opencv.hpp"
#include "opencv2/ximgproc/slic.hpp"
#include "opencv2/core/core.hpp"

class Superpixel {

public:
  Superpixel(const int algorithm = 101,
	     const int region_size = 20,
	     const int ruler = 10.0,
	     const int iteration = 10,
	     const int min_element_size = 10);
  cv::Mat extractSuperPixelMask(const cv::Mat & image_input);
  cv::Mat applySuperPixel( cv::Mat  image_input,
			   cv::Mat  image_mask
			   ) const ;
  cv::Mat applyPixelRegion(cv::Mat image_mask) const;
  int getNumberOfSuperpixels();
  bool if_white(cv::Mat image,
		int indicateur_i,
		int indicateur_j) const;
  cv::Mat getLabels();

private:
  cv::Ptr<cv::ximgproc::SuperpixelSLIC> slic_image;
  int algorithm;
  int region_size;
  int ruler;
  int iteration;
  int min_element_size;
};

#endif // SLIC_HPP_
