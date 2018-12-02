#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

struct ApplicationConstants {
  int width = 1600;
  int height = 1200;
  float aspectRatio = (float)width / (float)height;
  std::string title = "Flooids";

  float near = 1.f;
  float far = 2000.f;
};

static ApplicationConstants Constants;

#endif
