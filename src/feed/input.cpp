#include <iomanip>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

/**
 *
*/
class InputFeed {
  /**
   *
  */
  VideoCapture cap;

  /**
   *
  */
  CommandLineParser ifParser;

  /**
   *
  */
  const string keys =
      "{ help h | | print help message }"
      "{ camera c | 0 | capture video from camera (device index starting from "
      "0) "
      "}"
      "{ video v | | use video as input }";

 private:
  int setupInput() {
    int camera = ifParser.get<int>("camera");
    string file = ifParser.get<string>("video");

    if (!ifParser.check()) {
      ifParser.printErrors();
      return 1;
    }

    if (file.empty()) {
      cap.open(camera);
    } else {
      file = samples::findFileOrKeep(file);
      cap.open(file);
    }

    if (!cap.isOpened()) {
      cout << "Cannot open video stream: '"
           << (file.empty() ? "<camera>" : file) << "'" << endl;
      return 2;
    }
  }

 public:
  InputFeed() {
    throw invalid_argument("No arguments were provided");
  }

  InputFeed(int argc, char** argv) {
    CommandLineParser parser(argc, argv, keys);
    ifParser = parser;
    if (ifParser.has("help")) {
      ifParser.printMessage();
      return;
    }

    setupInput();
  }

  /**
   *
  */
  Mat getFrame() {
    Mat frame;
    cap >> frame;

    return frame;
  }
}