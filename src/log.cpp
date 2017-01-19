#include "log.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

Log::Log() {
   std::cout << "Opening file" << std::endl;
    file.open("../result/result.log", std::ios::app);
    if (!file.good())
      return;
    std::cout << "file Opened" << std::endl;
    file << "===========================\n"
         << "Begining Output log\n"
         << "===========================\n";
}

Log::~Log() {
  if (!file.good())
    return;
  file << "\n=========================\n"
       << " End saving logs at "<< std::endl
       << "===========================";
  file.close();
}


void Log::receiveSLIC(int nb_image,
		      int algorithm, int region_size, int ruler,
		      int min_element_size, int iteration, 
		      int execution_time) {
  superpixel_algo = 0;
  this->nb_image = nb_image;
  this->algorithm = algorithm +100;
  this->region_size = region_size;
  this->ruler = ruler;
  this->min_element_size = min_element_size;
  this->iteration = iteration;
  this->execution_time = execution_time;
  };

void Log::receiveSEEDS(int nb_image,
		       int num_superpixels,
		       int num_levels,
		       int prior,
		       int iteration,
		       int execution_time) {
  superpixel_algo = 1;
  this->nb_image = nb_image;
  this->num_superpixels = num_superpixels;
  this->num_levels = num_levels;
  this->prior = prior;
  this->iteration = iteration;
  this->execution_time = execution_time;
  };

void Log::write() {
  // if SLIC:
  if(superpixel_algo == 0) {
    file << " Image number: " << nb_image << std::endl;
    file << " Algorithm :" << algorithm << std::endl; 
    file << " Region size : " << region_size << std::endl;
    file << " Ruler : " << ruler << std::endl;
    file << " Connectivity :" << min_element_size << std::endl;
    file << " Iteration :" << iteration << std::endl;
    file << " Execution time : " <<execution_time << std::endl;
    file << "===========================" << std::endl;

  }
  else {
    file << " Image number: " << nb_image << std::endl;
    file << " Number of superpixels :" << num_superpixels << std::endl; 
    file << " Number of levels : " << num_levels << std::endl;
    file << " Prior : " << prior << std::endl;
    file << " Iteration :" << iteration << std::endl;
    file << " Execution time :" << execution_time << std::endl;
    file << "===========================" << std::endl;
  }
}
