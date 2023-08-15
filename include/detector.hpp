#include <opencv2/objdetect.hpp>

using namespace cv;
using namespace std;

namespace osv
{

/** Class used for all detection using the Histogram of Oriented Gradients
 * feature descriptor. */
class Detector
{
  enum Mode
  {
    Default,
    Daimler
  } m;

  HOGDescriptor hog, hog_d;

public:
  Detector ()
      : m (Default), hog (),
        hog_d (Size (48, 96), Size (16, 16), Size (8, 8), Size (8, 8), 9);

  void toggleMode ();

  char modeName () const;

  /* Threshold for the distance between features and SVM classifying plane.
   * Usually it is 0 and should be specified in the detector coefficients (as
   * the last free coefficient). But if the free coefficient is omitted (which
   * is allowed), you can specify it manually here.*/
  double hitThreshold = 0.0;

  /* Coefficient to regulate the similarity threshold. When detected, some
   * objects can be covered by many rectangles. 0 means not to perform
   * grouping.*/
  double groupThreshold = 0.0;

  /* Run the detector with default parameters. To get a higher hit-rate (and
   * more false alrams, respectively), decrease the HitThre */
  vector<Rect> detect (InputArray img);

  /* The HOG detector returns slightly larger rectangles than the real objects,
   * so we slightly shrink the rectangles to get a nicer output. */
  void adjustRect (Rect &r) const;
};
} // namespace osv