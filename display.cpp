//
// Created by Tamas on 2017-12-04.
//

// This cpp contains code for displaying a solved puzzle. Inputs are the images and solution matrix, output is the
// completed puzzle



#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


#include <vector>
#include <math.h>
#include <iostream>

//Constants:

using namespace cv;

// common std members:
using std::cout;
using std::cin;
using std::vector;
using std::endl;

int noRows = 5;
int noCols = 5;

//typedefs:

typedef std::vector<std::vector<Point>> Contour_t;
typedef std::vector<Vec4i> Hierarchy_t; //hierarchy object is needed for drawContours apparently

class SolutionElement
// Proposal: make data members public.
{
public:
    SolutionElement(): index(-1), orientation(-1){}
    ~SolutionElement() {}
    /*
    void setIndex(uint16_t);
    void setOrientation(uint8_t);

    uint16_t getIndex();
    uint8_t getOrientation();

protected:
     */
      int index;  //x.jpg
      int orientation;    //CW rotation = orientation*90 (deg): 0,1,2,3
};


// Function headers:

/*
void SolutionElement::setIndex(uint16_t ind) {
    index = ind;
}
//void SolutionElement::setOrientation(uint8_t alpha) {
    orientation = alpha;
}
//uint16_t SolutionElement::getIndex() {
    return index;
}
uint8_t SolutionElement::getOrientation() {
    return orientation;
}
 */

Mat segmentThresh_bad(Mat src);
Mat drawLargestContour(Mat, int, bool);

Mat readImage(int fileIndex = 0);

Mat resultImage( vector<vector< SolutionElement>> solutionMx);
Contour_t getLongestContour(Mat); //  depracated
int getLongestContourIndex(Contour_t);

int findmax(std::vector<int> vals);
int findmin(std::vector<int> vals);
float findmax(std::vector<float> vals);
float findmin(std::vector<float> vals);

std::vector<int> histogram(std::vector<int> data, int rangeWidth); // discrete histogram
std::vector<int> cont_histogram(std::vector<int> data);
void drawHistogram(vector<int> hist);

int main(){


    noRows = 20;
    noCols = 30;
    vector<vector< SolutionElement>> solvedMx;
    solvedMx.resize(noRows);
    int index = 35; // random

    //Making a random 5x5 solution matrix:
    for(int i = 0; i<noRows;i++){
        for(int j = 0; j<noCols;j++){
            SolutionElement element;
            element.index = index;
            element.orientation = 1;
            solvedMx[i].push_back(element);
            index++;
           // cout<<"index: "<<index<<endl;
        }
    }

    // showing the result:
    Mat solvedImage = resultImage(solvedMx);
    imshow("result",solvedImage);
    waitKey(0);


    //Making another solution matrix:
    noRows = 3;
    noCols = 3;
    vector<vector< SolutionElement>> solve3x3;
    solve3x3.resize(noRows);
    for(size_t i = 0; i < solve3x3.size(); i++){
        for(int j = 0; j < noCols; j++){
            SolutionElement element;
            element.index = 0;
            element.orientation = 0;
            solve3x3[i].push_back(element);
        }

    }
    // result written by hand:
    solve3x3[0][0].index = 11;
    solve3x3[0][1].index = 12;
    solve3x3[0][2].index = 13;
    solve3x3[1][0].index = 21;
    solve3x3[1][1].index = 22;
    solve3x3[1][2].index = 23;
    solve3x3[2][0].index = 31;
    solve3x3[2][1].index = 32;
    solve3x3[2][2].index = 33;


    solvedImage = Mat::zeros(solvedImage.size(), CV_8UC3);
    solvedImage = resultImage(solve3x3);
    imshow("result",solvedImage);
    waitKey(0);

    char outputFile [100];
    sprintf(outputFile,"../solution/solution.jpg");
    imwrite(outputFile,solvedImage);

}




Mat readImage(int fileIndex){
    // also crops the image to the outermost contour
    char inputFile [100];
    sprintf(inputFile,"../test_puzzle/%d.jpg",fileIndex);
    Mat source = imread(inputFile,1);
//    int padding = 30;
//    copyMakeBorder(source, source, padding, padding, padding, padding, BORDER_CONSTANT, (0,0,0));
//    Contour_t contours;
//    Hierarchy_t hierarchy;
//    Mat bw;
//    cvtColor(source,bw, CV_BGR2GRAY);
//    threshold(bw,bw,1,255,CV_THRESH_BINARY);
//    findContours(bw,contours,hierarchy,CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
//    int ind = getLongestContourIndex(contours);
//
//    // straight rectangle:
//    Rect roi =  boundingRect(contours[ind]);
//
//
//    //make it square
//    cout<<"cropping"<<endl;
//    if(roi.width>20+roi.height){
//        roi.height = roi.width;
//    }
//    if(roi.height>20+roi.width){
//        roi.width = roi.height;
//    }
//    //Mat crop = source(roi);
    //imshow("source",source);
    //waitKey(0);
    return source;

}

Mat resultImage( vector<vector< SolutionElement>> solutionMx ){

    int Y_size = 850;
    int separator = 1;
    int partHeight = 90;
    int partWidth;
    auto imageH =  int(round(partHeight* solutionMx.size()));

    if(imageH > Y_size){
        imageH = Y_size;
    }
    partHeight = int(round(imageH /  solutionMx.size()));
    partWidth= partHeight;
    int imageW = int(round( partWidth*solutionMx[1].size()));

    cout<<"imageW "<<imageW <<endl<<"imageH " <<imageH<<endl<<endl;
    cout<<"partW "<<partWidth <<endl<<"partH " <<partHeight<<endl<<endl;
    Mat result(imageH,imageW,CV_8UC3);

    for(size_t i = 0; i<solutionMx.size();i++){
        for(size_t j = 0; j<solutionMx[i].size();j++){
            int imageNumber =  solutionMx[i][j].index;
            //cout<<"imageIndex: "<< imageNumber << endl;
            Mat img = readImage(imageNumber);
//            imshow("readImg",img); // you can comment with Ctrl + / did you know? :D
//            waitKey(0);

            auto ROI_X = int(round(j*partWidth));
            auto ROI_Y = int(round(i*partHeight));
//            cout<<"ROI X: "<< ROI_X<<endl;
//            cout<<"ROI Y: "<< ROI_Y<<endl;

            Rect ROI(ROI_X,ROI_Y , partWidth-separator, partHeight-separator); // j is the x coordinate not i!!
            Mat temp; resize(img,temp, Size(ROI.width, ROI.height));
            temp.copyTo(result(ROI));

//            imshow("result",result);
//            waitKey(0);
        }
    }
    return result;
}



Mat drawLargestContour(Mat bw, int approxStrength, bool filled){

    Mat mask =  Mat::zeros(bw.size(), CV_8UC1);
    Hierarchy_t hierarchy;
    Contour_t contours;
    findContours(bw, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int largestIndex = getLongestContourIndex(contours);
    Contour_t  largestContour;
    largestContour.resize(contours.size()); //Otherwise it is just a null pointer and the program crashes!
    size_t index = 0;
    int strength = approxStrength; // distance between the original contour and the approximated contour
    approxPolyDP(Mat(contours[largestIndex]), contours[largestIndex], strength, true);
    int lineThickness = 2;
    if(filled){
        drawContours(mask, contours,largestIndex, 255,CV_FILLED , 8, hierarchy, 0); //instead of line thickness CV_FILLED
    }
    else{
        drawContours(mask, contours,largestIndex, 255, lineThickness, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    }

    return(mask);
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


int findmax(std::vector<int> vals){
    int max=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(max<vals[i]) max=vals[i];
    return max;
}

float findmax(std::vector<float> vals){
    float max=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(max<vals[i]) max=vals[i];
    return max;
}

int findmin(std::vector<int> vals){
    int min=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(min>vals[i]) min=vals[i];
    return min;
}

float findmin(std::vector<float> vals){
    float min=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(min>vals[i]) min=vals[i];
    return min;
}

std::vector<int> histogram(std::vector<int> data, int rangeWidth){

    int dataMax = findmax(data);
    int noRanges = (dataMax/rangeWidth+1);
    std::vector<int> discreteFreq;
    for(int i = 0; i<noRanges; i++){
        discreteFreq.push_back(0);
    }
    //cout<<"number of ranges:"<<noRanges<<std::endl;
    //cout<<"max val:"<<dataMax<<std::endl;
    for(size_t i=0; i<data.size();i++) {
        for(int j = 0; j< noRanges; j++){
            double min = j*rangeWidth;
            double max = (j+1)*rangeWidth;
            //std::cout<<"Data: "<<data[i]<<std::endl;
            //std::cout<<"min: "<<min<<" max: "<<max<<std::endl;
            if(min < data[i] && data[i] <= max){
                //std::cout<<data[i]<<" : added to range: "<<j<<std::endl;
                discreteFreq[j]++;
            }
        }

    }
    return(discreteFreq);

}

std::vector<int> cont_histogram(std::vector<int> data){

    int dataMax = findmax(data);
    std::vector<int> frequency;
    for(int i = 0; i<=dataMax; i++){
        frequency.push_back(0);
    }
    for(size_t i = 0; i<data.size(); i++){
        frequency[data[i]]++;
    }
    return(frequency);

}

void drawHistogram(vector<int> hist){
    cout<<"Histogram: "<<endl;
    for(size_t i=0; i<hist.size();i++){

        std::cout<<"|";
        for(int j = 0;j<hist[i];j++){
            std::cout<<"x";
        }
        std::cout<<std::endl;
    }
    std::cout<<"============"<<std::endl;
}