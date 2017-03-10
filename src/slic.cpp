// std
#include <string.h>
#include <chrono>
#include <stack>
#include <queue>
#include <utility>


// Superpixel
#include "slic.hpp"


Superpixel::Superpixel(const int algorithm,
		       const int region_size,
		       const int ruler,
		       const int iteration,
		       const int min_element_size):
  algorithm(algorithm+100),
  region_size(region_size),
  ruler(ruler),
  iteration(iteration),
  min_element_size(min_element_size){};

cv::Mat Superpixel::extractSuperPixelMask(const cv::Mat & image_input) {
 
  cv::Mat mask;
  
  // Convert the image input to HSV:

  slic_image = cv::ximgproc::createSuperpixelSLIC(image_input, algorithm, 
						  region_size, ruler);
  
  slic_image->iterate(10);
  
  slic_image->enforceLabelConnectivity(min_element_size);

  
  // return the mask contour:
  slic_image->getLabelContourMask(mask, true);
  //slic_image->getLabels(mask);
  return mask;
}

cv::Mat Superpixel::getLabels() {
  cv::Mat mask;
  
  // apply getLabels method:
  slic_image->getLabels(mask);
  
  cv::Mat image_result(mask.rows, mask.cols, CV_8UC3);
  
  const int rows = image_result.rows;
  const int cols = image_result.cols;
  bool **is_checked = new bool*[rows];
  for (int i = 0; i < rows; i++)
	  is_checked[i] = new bool[cols];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
      is_checked[i][j] = false;
  
  int array_1[4] = {1, -1, 0, 0};
  int array_2[4] = {0, 0, -1, 1};

  for(int i = 0 ; i < image_result.rows; i++) 
    for(int j = 0; j < image_result.cols; j++) {
      if(!is_checked[i][j]) {
	// set new color for the pixel:
	int b = cv::theRNG().uniform(0, 255);
	int g = cv::theRNG().uniform(0, 255);
	int r = cv::theRNG().uniform(0, 255);
	cv::Vec3b intensity = image_result.at<cv::Vec3b>(i, j);
	intensity.val[0] = b;
	intensity.val[1] = g;
	intensity.val[2] = r;
	std::queue<std::pair<int,int>> pts_region;
	pts_region.push(std::make_pair(i,j));
	
	int region_number = mask.at<int>(i,j);
	while(!pts_region.empty()) {
	  std::pair<int, int> temp;
	  temp = pts_region.front();
	  pts_region.pop();
	  is_checked[temp.first][temp.second] = true;
	  image_result.at<cv::Vec3b>(temp.first, temp.second) = intensity;
	  
	  for(int k = 0; k < 4; k++) {
	    if(temp.first + array_1[k] < rows && temp.first +  array_1[k]  >=0 &&
	       temp.second + array_2[k] < cols && temp.second + array_2[k] >=0) {
	      if(region_number == mask.at<int>(temp.first + array_1[k],
						    temp.second + array_2[k])
		 && !is_checked[temp.first + array_1[k]][temp.second + array_2[k]]) {
		is_checked[temp.first+array_1[k]][temp.second+array_2[k]] = true;
		pts_region.push(std::make_pair (temp.first + array_1[k], 
						temp.second+ array_2[k]));
	      }
	    }
	  }
	}
      }
    }
  
  return image_result;
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

cv::Mat Superpixel::applyPixelRegion(cv::Mat image_mask) const {
  
  cv::Mat image_result;
  cv::Mat inverse_mask;
  
  // using morphology closing to the mask to get the better result:
  cv::Mat element50 = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(3,3),
						cv::Point(-1,-1));

  // cv::morphologyEx(image_mask, image_mask, cv::MORPH_CLOSE, element50);
    cv::dilate(image_mask, image_mask, element50);
  // copy mask to the image result
  image_mask.copyTo(image_result);
  // convert image result to 3-channel BGR Mat: 
  cv::cvtColor(image_result, image_result, CV_GRAY2BGR);
  

  // Test algorithm 1:
  /*
  // inverse the color in mask:
  cv::bitwise_not(image_mask, inverse_mask);
  // extract the contours:
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(inverse_mask, contours,
		   CV_RETR_EXTERNAL,
		   CV_CHAIN_APPROX_NONE);
  std::vector<cv::Vec4i> hierarchy;
  std::cout << contours.size() << std::endl;
  for(uint i = 0; i < contours.size(); i++) {
     int b = cv::theRNG().uniform(0, 255);
     int g = cv::theRNG().uniform(0, 255);
     int r = cv::theRNG().uniform(0, 255);
     cv::Scalar color(b, g, r);
     
     // draw the contour:
     cv::drawContours(image_result, contours, i, color, 
		      -1, 8, hierarchy, 0, cv::Point()); 
  }
  */
  

  // Test algorithm 2:/
  
  const int rows = image_result.rows;
  const int cols = image_result.cols;
  bool **is_checked = new bool*[rows];
  for (int i = 0; i < rows; i++)
	  is_checked[i] = new bool[cols];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
      is_checked[i][j] = false;
  
  int array_1[4] = {1, -1, 0, 0};
  int array_2[4] = {0, 0, -1, 1};

  for(int i = 0; i < rows; i++) 
    for(int j =0; j < cols; j++) {
      if(!if_white(image_mask, i, j) && !is_checked[i][j] ) {

	// set new color for the pixel:
	
	int b = cv::theRNG().uniform(0, 255);
	int g = cv::theRNG().uniform(0, 255);
	int r = cv::theRNG().uniform(0, 255);
	cv::Vec3b intensity = image_result.at<cv::Vec3b>(i, j);
	intensity.val[0] = b;
	intensity.val[1] = g;
	intensity.val[2] = r;
	image_result.at<cv::Vec3b>(i,j)  = intensity;
	std::queue<std::pair<int,int>> pts_region;
	pts_region.push(std::make_pair (i,j));
	
	while(!pts_region.empty()) {
	  std::pair<int, int> temp;
	  temp = pts_region.front();
	  pts_region.pop();
	  is_checked[temp.first][temp.second] = true;
	  image_result.at<cv::Vec3b>(temp.first, temp.second) = intensity;
	  
	  for(int k = 0; k < 4; k++) {
	    if(temp.first + array_1[k] < rows && temp.first +  array_1[k]  >=0 &&
	       temp.second + array_2[k] < cols && temp.second + array_2[k] >=0) {
	      if(!if_white(image_mask, temp.first + array_1[k], 
			   temp.second + array_2[k])
		 && !is_checked[temp.first + array_1[k]][temp.second + array_2[k]]) {
		is_checked[temp.first+array_1[k]][temp.second+array_2[k]] = true;
		pts_region.push(std::make_pair (temp.first + array_1[k], 
						temp.second+ array_2[k]));
	      }
	    }
	  }	  
	}
      }
    }
  
  //  image_result.setTo(cv::Scalar(0, 0, 0), image_mask);
  return image_result;

}

int Superpixel::getNumberOfSuperpixels() {
  return slic_image->getNumberOfSuperpixels();
}
  
  
bool Superpixel::if_white(cv::Mat image,
		  int indicateur_i,
		  int indicateur_j) const  {
  cv::Scalar intensity = image.at<uchar>(indicateur_i, indicateur_j);
  if(intensity.val[0] == 255)
    return true;
  return false;
}
