#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "feed/input.cpp"

using namespace std;
using namespace cv;
using namespace osv;

int main(int argc, char** argv) {

  InputFeed inputFeed(argc, argv);

  Mat frame;

  for (;;) {
    frame = inputFeed.getFrame();
    if (frame.empty()) {
      cout << "Finished reading: empty frame" << endl;
      break;
    }
  }

  cout << " Usage: ImageToLoadAndDisplay" << endl;
  if (argc != 2) {
    cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
    return -1;
  }

  Mat image;
  image = imread(argv[1], IMREAD_COLOR);  // Read the file

  if (image.empty())  // Check for invalid input
  {
    cout << "Could not open or find the image" << endl;
    return -1;
  }

  namedWindow("Display window",
              WINDOW_AUTOSIZE);  // Create a window for display.

  imshow("Display window", image);  // Show our image inside it.
  waitKey(0);                       // Wait for a keystroke in the window

  return 0;
}