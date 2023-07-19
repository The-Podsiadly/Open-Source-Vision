#include <opencv2/objdetect.hpp>

using namespace cv;
using namespace std;

/**  */
class Detector {
  enum Mode { Default, Daimler } m;

  HOGDescriptor hog, hog_d;

 public:
  Detector()
      : m(Default),
        hog(),
        hog_d(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9) {
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    hog_d.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
  }

  void toggleMode() { m = (m == Default ? Daimler : Default); };

  string modeName() const { return (m == Default ? "Default" : "Daimler"); }

  /* Threshold for the distance between features and SVM classifying plane. Usually it is 0 and should be specified in the detector coefficients (as the last free coefficient). But if the free coefficient is omitted (which is allowed), you can specify it manually here.*/
  double hitThreshold = 0.0;

  /* Coefficient to regulate the similarity threshold. When detected, some objects can be covered by many rectangles. 0 means not to perform grouping.*/
  double groupThreshold = 0.0;

  /* Run the detector with default parameters. To get a higher hit-rate (and more false alrams, respectively), decrease the HitThre */
  vector<Rect> detect(InputArray img) {
    vector<Rect> found;

    if (m == Default) {
      hog.detectMultiScale(img, found, hitThreshold, Size(8, 8), Size(), 1.05,
                           groupThreshold, false);
    } else if (m == Daimler) {
      hog_d.detectMultiScale(img, found, hitThreshold, Size(8, 8), Size(), 1.05,
                             groupThreshold, true);
    }

    return found;
  }

  /* The HOG detector returns slightly larger rectangles than the real objects, so we slightly shrink the rectangles to get a nicer output. */
  void adjustRect(Rect& r) const {
    r.x += cvRound(r.width * 0.1);
    r.width = cvRound(r.width * 0.8);
    r.y += cvRound(r.height * 0.1);
    r.height = cvRound(r.height * 0.8);
  }
};