//
// Created by Tamas on 2017-12-02.
//

// Using simple thresholding
// Then attempting to close the open contours


#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


#include <vector>
#include <math.h>
#include <iostream>
using namespace cv;


//using namespace std;

//typedefs:

typedef std::vector<std::vector<Point>> Contour_t;
typedef std::vector<Vec4i> Hierarchy_t; //hierarchy object is needed for drawContours apparently

class SolutionElement
{
public:
    SolutionElement(): index(-1), orientation(-1){}
    ~SolutionElement() {}
    void setIndex(uint16_t);
    void setOrientation(uint8_t);

    uint16_t getIndex();
    uint8_t getOrientation();

protected:
    uint16_t index;  //x.jpg
    uint8_t orientation;    //CW rotation = orientation*90 (deg): 0,1,2,3
};

void SolutionElement::setIndex(uint16_t ind) {
    index = ind;
}
void SolutionElement::setOrientation(uint8_t alpha) {
    orientation = alpha;
}
uint16_t SolutionElement::getIndex() {
    return index;
}
uint8_t SolutionElement::getOrientation() {
    return orientation;
}

const int noRows = 5;
const int noCols = 13;


Mat drawLargestContour(Mat);
Mat segmentThresh(Mat);
Mat segmentCanny(Mat);
Mat segment(Mat);
Mat correct(Mat);
Mat processRawImage(int fileIndex = 0);
Mat resultImage();
Contour_t getLongestContour(Mat); // might become soon depracated
int getLongestContourIndex(Contour_t);

int main() {
    for(int i=26;i<27;i++){
        Mat res = processRawImage(i);
        //imshow("output", res);
        //waitKey(0);
    }
    return 0;
}

Mat processRawImage(int fileIndex){


    char inputFile [50];
    sprintf(inputFile,"../input/%d.jpg",fileIndex);

    Mat source = imread(inputFile,1);
    imshow("source",source);
    Mat mask = segmentThresh(source);
    return mask;

}

//Mat resultImage( SolutionElement solutionMx[noCols][noRows]){

Mat segmentThresh(Mat src){
    bool loadfailed = false;
    if (!src.data || src.empty())
    {
        std::cout << "Problem loading image!!!" << std::endl;
    }
    else{
        loadfailed = true;
    }
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);

    // Convert image to binary
    Mat bw;
    threshold(gray,bw,180,255,CV_THRESH_BINARY_INV);
    // adaptiveThreshold(bw, bw, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 5,2);
    //imshow("bw",bw);
    int padding = 100;
    copyMakeBorder(bw, bw, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
    copyMakeBorder(src, src, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));

    /*
    //Find the contour length of the original thresholded image
    Contour_t originalContour;
    Hierarchy_t orig_hierarchy;
    findContours(bw,originalContour,orig_hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    int originalIndex = getLongestContourIndex(originalContour);
    double originalPerimeter = arcLength(originalContour[originalIndex],true);
    */

    double maskPerimeter = 0;

    Mat mask = drawLargestContour(bw);

    double oldPerimeter = -1;

    while(maskPerimeter > oldPerimeter){
        std::cout<<"In the while loop:"<<std::endl;
        //measuring contour length:
        Contour_t maskContour;
        Hierarchy_t mask_hierarchy;
        findContours(bw,maskContour,mask_hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        int maskIndex = getLongestContourIndex(maskContour);
        for(size_t i = 0; i < maskContour.size(); i++){
            approxPolyDP(maskContour[i],maskContour[i],1,true);
            oldPerimeter += arcLength(maskContour[i],true);
        }
        Mat oldLongestContour =Mat::zeros(bw.size(),CV_8UC3);
        for(size_t i = 0; i < maskContour.size(); i++){
            drawContours(oldLongestContour, maskContour,i, Scalar(0,0,255), 2, 8, mask_hierarchy, 0);
        }
        drawContours(oldLongestContour, maskContour,maskIndex, Scalar(0,0,255), 2, 8, mask_hierarchy, 0);
        imshow("oldbw",bw);
        imshow("oldLongest",oldLongestContour);
        // applying contour closing transformation:


        int morph_size =2;
        Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
        // Point morph size makes it omnidirectional
        // morphologyEx(bw, bw, MORPH_CLOSE, element);
        morphologyEx(bw, bw, MORPH_DILATE, element);
        drawContours(bw, maskContour,maskIndex, 255, CV_FILLED, 8, mask_hierarchy, 0);

        //measuring contour length:
        Contour_t nmaskContour;
        Hierarchy_t nmask_hierarchy;
        findContours(bw,nmaskContour,nmask_hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        maskIndex = getLongestContourIndex(nmaskContour);

        for (size_t i = 0; i<nmaskContour.size(); i++){
            maskPerimeter += arcLength(nmaskContour[i],true);
        }



        std::cout<<"old perimeter: "<<oldPerimeter<<std::endl;
        std::cout<<"mask Perimeter: "<<maskPerimeter<<std::endl;
        imshow("mask",mask);



        Mat newLongestContour = Mat::zeros(bw.size(),CV_8UC3);

        for (size_t i = 0; i<nmaskContour.size(); i++) {
            drawContours(newLongestContour, nmaskContour, i, Scalar(0, 0, 255), 2, 8, nmask_hierarchy, 0);
        }

        imshow("newbw",bw);
        imshow("newlongest",newLongestContour);
        waitKey(0);

    } //END WHILE

    imshow("mask",mask);
    return mask;
}




Mat drawLargestContour(Mat bw){

    Mat mask =  Mat::zeros(bw.size(), CV_8UC1);
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int largestIndex = getLongestContourIndex(contours);
    Contour_t  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = 1; // distance between the original contour and the approximated contour
    //approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    drawContours(mask, contours,largestIndex, 255, CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    return(mask);
}


Contour_t getLongestContour(Mat bw){

    Mat mask =  Mat::zeros(bw.size(), CV_8UC1);
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        area = contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area) continue; //
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
    }

    Contour_t  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = 2; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    return(largestContour);
}


int getLongestContourIndex(Contour_t contours){
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        area = contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area) continue; //
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
    }
    return(largestIndex);
}

Mat correct(Mat src) {
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    // Convert image to binary
    Mat bw;
    threshold(gray, bw, 150, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); // INV: invert for findcontours to work properly
    // Find all the contours in the thresholded image
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    double area = 0;
    double largestArea = 0;
    size_t largestIndex = 0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        // Calculate the area of each contour
        area = contourArea(contours[i]);
        // Ignore contours that are too small or too large. If true, than the loop starts again
        if (area < 1e2 || 1e5 < area) continue; //
        //else:
        // Find the contour with the largest area:
        if (area > largestArea) {
            largestArea = area;
            largestIndex = i; // i is type size_t, should be converted to int.
        }
        drawContours(src, contours, static_cast<int>(i), Scalar(0, 0, 255), 3, 8, hierarchy, 0);
    }
    //Now we know the index of the largest contour in the image
    Contour_t  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = 6; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), largestContour[index], strength, true);
    Mat mask =  Mat::zeros(bw.size(), CV_8UC3);
    drawContours(mask, largestContour, static_cast<int>(index), Scalar(255,255,255), CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED

    RotatedRect box;
    box = minAreaRect(largestContour[index]);

    Point2f rect_points[4];
    box.points( rect_points );
    double alpha = 0;
    Draw:
    for( int j = 0; j < 4; j++ )
        line( mask, rect_points[j], rect_points[(j+1)%4], Scalar(0,255,0), 2, 8 );

    alpha =  atan2( (rect_points[1].x-rect_points[0].x) , (rect_points[1].y-rect_points[0].y)  );

    alpha = alpha*180/CV_PI;
    //std::cout<<alpha;



    //Rotate conter clockwise by alpha:
    Point2f center;
    center.x = mask.cols/2;
    center.y = mask.rows/2;
    Mat RotMatrix = getRotationMatrix2D(center,int(-alpha),1);
    warpAffine(mask,mask,RotMatrix, mask.size());
    return mask;

}
