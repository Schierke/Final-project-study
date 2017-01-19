#ifndef _LOG_H
#define _LOG_H
#include <fstream>
/**
 * @brief [the base class for logging system]
 * @details [this class is for saving all the data from the source]
 * @return [description]
 */
class Log {
private:
  std::ofstream file;
  int nb_image;
  int superpixel_algo; // which superpixel algo: SLIC or SEED
  int algorithm; // SLIC or SLICO
  int region_size; // SLIC
  int ruler; // SLIC
  int min_element_size; // SLIC
  int iteration; // SLIC & SEED
  int num_levels; // SEEDS
  int num_superpixels; // SEEDS
  int prior; // SEEDS

public:
  Log();
  ~Log();
  void write();
  void receiveSLIC(int, int, int, int, int, int);
  void receiveSEEDS(int, int, int, int, int);
};
#endif
