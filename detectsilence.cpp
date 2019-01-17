#include "opencv2/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include "dirent.h"

using namespace std;
using namespace cv;


string OPENCV_ROOT = "C:/opencv/";
string cascades = OPENCV_ROOT + "build/etc/haarcascades/";
string FACES_CASCADE_NAME = cascades + "haarcascade_frontalface_alt.xml";

//  The mouth cascade is assumed to be in the local folder
string MOUTH_CASCADE_NAME = "Mouth.xml";

void drawEllipse(Mat frame, const Rect rect, int r, int g, int b) {
int width2 = rect.width / 2;
int height2 = rect.height / 2;
Point center(rect.x + width2, rect.y + height2);
ellipse(frame, center, Size(width2, height2), 0, 0, 360,
Scalar(r, g, b), 2, 8, 0);
}


bool detectSilence(Mat frame, Point location, Mat ROI, CascadeClassifier cascade)
{
// frame,location are used only for drawing the detected mouths
vector<Rect> mouths;

cascade.detectMultiScale(ROI, mouths, 1.1, 11, 0, Size(15, 15));


int nmouths = (int)mouths.size();
for (int i = 0; i < nmouths; i++) {
Rect mouth_i = mouths[i];
drawEllipse(frame, mouth_i + location, 255, 255, 0);
}
return(nmouths == 0);
}

// you need to rewrite this function
int detect(Mat frame,
CascadeClassifier cascade_face, CascadeClassifier cascade_mouth) {
Mat frame_gray;
vector<Rect> faces;

cvtColor(frame, frame_gray, CV_BGR2GRAY);



equalizeHist(frame_gray, frame_gray); 

cascade_face.detectMultiScale(frame_gray, faces,
1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(60, 60));



int detected = 0;

int nfaces = (int)faces.size();
for (int i = 0; i < nfaces; i++) {
Rect face = faces[i];
drawEllipse(frame, face, 255, 0, 255);
int x1 = face.x;
int y1 = face.y + face.height / 1.57;
Rect lower_face = Rect(x1, y1, face.width, face.height / 2.4);
drawEllipse(frame, lower_face, 100, 0, 255);
Mat lower_faceROI = frame_gray(lower_face);
if (detectSilence(frame, Point(x1, y1), lower_faceROI, cascade_mouth)) {
drawEllipse(frame, face, 0, 255, 0);
detected++;
}
}
return(detected);
}

int runonFolder(const CascadeClassifier cascade1,
const CascadeClassifier cascade2,
string folder) {
if (folder.at(folder.length() - 1) != '/') folder += '/';
DIR *dir = opendir(folder.c_str());
if (dir == NULL) {
cerr << "Can't open folder " << folder << endl;
exit(1);
}
bool finish = false;
string windowName;
struct dirent *entry;
int detections = 0;
while (!finish && (entry = readdir(dir)) != NULL) {
char *name = entry->d_name;
string dname = folder + name;
Mat img = imread(dname.c_str(), CV_LOAD_IMAGE_UNCHANGED);
if (!img.empty()) {
int d = detect(img, cascade1, cascade2);
cerr << d << " detections" << endl;
detections += d;
if (!windowName.empty()) destroyWindow(windowName);
windowName = name;
namedWindow(windowName.c_str(), CV_WINDOW_AUTOSIZE);
imshow(windowName.c_str(), img);
int key = waitKey(0); // Wait for a keystroke
switch (key) {
case 27: // <Esc>
finish = true; break;
default:
break;
}
} // if image is available
}
closedir(dir);
return(detections);
}

void runonVideo(const CascadeClassifier cascade1,
const CascadeClassifier cascade2) {
VideoCapture videocapture(0);
if (!videocapture.isOpened()) {
cerr << "Can't open default video camera" << endl;
exit(1);
}
string windowName = "Live Video";
namedWindow("video", CV_WINDOW_AUTOSIZE);
Mat frame;
bool finish = false;
while (!finish) {
if (!videocapture.read(frame)) {
cout << "Can't capture frame" << endl;
break;
}
detect(frame, cascade1, cascade2);
imshow("video", frame);
if (cvWaitKey(30) >= 0) finish = true;
}
}

int main(int argc, char** argv) {
if (argc != 1 && argc != 2) {
cerr << argv[0] << ": "
<< "got " << argc - 1
<< " arguments. Expecting 0 or 1 : [image-folder]"
<< endl;
return(-1);
}

string foldername = (argc == 1) ? "" : argv[1];
CascadeClassifier faces_cascade, mouth_cascade;

if (
!faces_cascade.load(FACES_CASCADE_NAME)
|| !mouth_cascade.load(MOUTH_CASCADE_NAME)) {
cerr << FACES_CASCADE_NAME << " or " << MOUTH_CASCADE_NAME
<< " are not in a proper cascade format" << endl;
return(-1);
}

int detections = 0;
if (argc == 2) {
detections = runonFolder(faces_cascade, mouth_cascade, foldername);
cout << "Total of " << detections << " detections" << endl;
}
else runonVideo(faces_cascade, mouth_cascade);
return(0);
}