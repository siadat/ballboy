#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctime>
#include <highgui.h>
#define IMG_WIDTH 320
#define IMG_HIGHT 240
#define MAX_R 160

using namespace cv;
using namespace std;

int debugging = false;

Mat image, image_blurred, src_gray;
int fcounter = 0;
char* dt;
time_t now;

int hsv_h_min,
    hsv_s_min,
    hsv_v_min,
    hsv_h_max,
    hsv_s_max,
    hsv_v_max;

int hough_p1,
    hough_p2;

void on_trackbar(int, void*) {
  if(debugging) {
    printf("# debug: new HSV values: %d %d %d %d %d %d | new p1 p2: %d %d\n",
           hsv_h_min, hsv_s_min, hsv_v_min,
           hsv_h_max, hsv_s_max, hsv_v_max,
           hough_p1, hough_p2);
  }
}

int main(int argc, char ** argv) {

  int c;
  while((c = getopt(argc, argv, "v")) != -1) {
    switch(c) {
      case 'v':
        debugging = true;
        break;
    }
  }

  int xi = optind;
  int xargc = argc - xi;

  if(xargc < 6) {
    cout << "Usage:" << endl;
    cout << "  ballboy [-v] minH minS minV maxH maxS maxV" << endl;
    cout << "Options:" << endl;
    cout << "    -v: enable verbose mode" << endl;
    cout << "  minH: [0...256] min hue" << endl;
    cout << "  minS: [0...256] min saturation" << endl;
    cout << "  minV: [0...256] min value" << endl;
    cout << "  maxH: [0...256] max hue" << endl;
    cout << "  maxS: [0...256] max saturation" << endl;
    cout << "  maxV: [0...256] max value" << endl;
    cout << "Example:" << endl;
    cout << "  ballboy 66 47 24 90 256 204" << endl;
    return 1;
  }

  hough_p1 = 73;
  hough_p2 = 73;

  if(xargc >= 6) {
    hsv_h_min = atoi(argv[xi + 0]);
    hsv_s_min = atoi(argv[xi + 1]);
    hsv_v_min = atoi(argv[xi + 2]);

    hsv_h_max = atoi(argv[xi + 3]);
    hsv_s_max = atoi(argv[xi + 4]);
    hsv_v_max = atoi(argv[xi + 5]);
  }

  if(xargc == 8) {
    hough_p1  = atoi(argv[xi + 6]);
    hough_p2  = atoi(argv[xi + 7]);
  }


  if(debugging) {
    cout << "# debug: ==============" << endl;
    cout << "# debug: = debug mode =" << endl;
    cout << "# debug: ==============" << endl;
  }


  // Captures webcam:
  VideoCapture cap(0);

  int erosion_size = 1;
  int dilation_size = 5;

  Mat erode_el = getStructuringElement(MORPH_ELLIPSE,
                             Size(2 * erosion_size + 1, 2 * erosion_size+1),
                             Point(erosion_size, erosion_size));

  Mat dilate_el = getStructuringElement(MORPH_ELLIPSE,
                             Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                             Point(dilation_size, dilation_size));

  if(!cap.isOpened()) {
    cout << "error: not opened";
    return 1;
  }

  if(debugging) {
    //Gets width and height:
    namedWindow("Video");
    cout << "# debug: width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
    cout << "# debug: height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    //Gets FPS:
    // double fps = cap.get(CV_CAP_PROP_FPS);
    //Sets width and height:
  }
  cap.set(CV_CAP_PROP_FRAME_WIDTH, IMG_WIDTH);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, IMG_HIGHT);

  if(debugging) {
    createTrackbar("p1", "Video", &hough_p1, 500, on_trackbar);
    createTrackbar("p2", "Video", &hough_p2, 500, on_trackbar);

    createTrackbar("Min H", "Video", &hsv_h_min, 256, on_trackbar);
    createTrackbar("Min S", "Video", &hsv_s_min, 256, on_trackbar);
    createTrackbar("Min V", "Video", &hsv_v_min, 256, on_trackbar);

    createTrackbar("Max H", "Video", &hsv_h_max, 256, on_trackbar);
    createTrackbar("Max S", "Video", &hsv_s_max, 256, on_trackbar);
    createTrackbar("Max V", "Video", &hsv_v_max, 256, on_trackbar);
  }

  while(char(waitKey(1)) != 'q' && cap.isOpened()) {

    Mat frame;
    cap >> frame;
    vector<Vec3f> circles;

    if(frame.empty()) {
      if(debugging) {
        cout << "# debug: Video over" << endl;
      }
      break;
    }

    cvtColor(frame, src_gray, CV_BGR2HSV);
    inRange(src_gray, Scalar(hsv_h_min, hsv_s_min, hsv_v_min, 0),
                      Scalar(hsv_h_max, hsv_s_max, hsv_v_max, 0), src_gray);
    // dilate(src_gray, src_gray, dilate_el);
    // erode(src_gray, src_gray, erode_el);

    GaussianBlur(src_gray, src_gray, Size(17, 17), 15);

    if(true || !debugging) {
      HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 2, IMG_HIGHT, hough_p1, hough_p2, 2, MAX_R);
    }
    // Draws the circles detected
    double circles_x = 0,
           circles_y = 0,
           circles_r = 0;
    for(size_t i = 0; i < circles.size(); i++) {
      circles_x += 100.0 * circles[i][0] / IMG_WIDTH;
      circles_y += 100.0 * circles[i][1] / IMG_HIGHT;
      circles_r += 100.0 * circles[i][2] / (IMG_HIGHT / 2.0);

      if(debugging) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle(src_gray, center, 3, Scalar(100, 255, 100), -1, 8, 0);
        // circle outline
        circle(src_gray, center, radius, Scalar(100, 100, 255), 3, 8, 0);
      }
    }

    if(circles.size() > 0) {
      cout << "x:" << cvRound(circles_x/circles.size()) << "\ty:" << cvRound(circles_y/circles.size()) << "\tr:" << cvRound(circles_r/circles.size()) << endl;
    } else {
      cout << "x:" << -1 << "\ty:" << -1 << "\tr:" << -1 << endl;
    }

    now = time(0);
    dt = ctime(&now);

    if(debugging) {
      imshow("Video", src_gray);
    }
  }
  return 0;
}
