# Wink-and-Shush-Detection-

Computer Vision Project Report 
 Wink and Silence Detection
Prathyusha Kanmanth Reddy
pxk166230@utdallas.edu

Language Used: C++
IDE: Microsoft Visual Studio
Library Used: OpenCV
External Files used: dirent.h, mouth.xml(assumed to be in the local folder)
Analysis:
DetectWink: Detect if the person in an image is winking or not using haar cascades.
The image folder for wink has a total of 24 images with 27 detectable faces out of which 20 faces are winking and 7 are not.
Initially, the accuracy of the code was about 50% because the cascade was not able to detect eyes properly. I figured out that the parameters determining region of interest(ROI) has to be modified. In the detectWink method, we are locating eyes by using the detectMultiScale call on cascade. Also, the detect method will help in detecting the face and modification of the parameters in detectMultiScale will help increase the number of detections. I experimented with various values for these parameters and found the best possible set which gives maximum number of correct detections. So far, the best accuracy I could achieve was 78%. This is the split up of detections:
Face undetected – 1
True Positive -16
True Negative - 5
False Negative - 4
False Positive - 1 
Accuracy = 21/27 = 78%

DetectSilence: Detect of the person in an image is placing finger on mouth or not using haar cascades.
The image folder for shush has a total of 21 images with 27 detectable faces out of which 13 faces are shush-ing and 14 are not.
Same goes here, initially the accuracy was less than 50%. The ROI for mouth has to be modified such that it detect when the mouth is covered by the finger or not. Changing the parameters for the ROI in the detectSilence method as well as the detect method will improve the accuracy drastically. The best accuracy I could achieve was 96%. This is the split up of the detections:
Face undetected-0
True Positive-13
True Negative-13
False Positive -1
False Negative-0
Accuracy = 26/27 = 96%


