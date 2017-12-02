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


const int noRows = 5;
const int noCols = 13;

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


// Function headers:

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


Mat drawLargestContour(Mat, int, bool);
Mat segmentThresh(Mat);
Mat segmentCanny(Mat);
Mat segment(Mat);
Mat correct(Mat);
Mat processRawImage(int fileIndex = 0);
Mat resultImage();
Contour_t getLongestContour(Mat); // might become soon depracated
int getLongestContourIndex(Contour_t);

int findmax(std::vector<int> vals);
std::vector<int> histogram(std::vector<int> data, int rangeWidth); // discrete histogram
std::vector<int> cont_histogram(std::vector<int> data);
void drawHistogram(vector<int> hist);

int main() {
    for(int i=0;i<20;i++){
        Mat res = processRawImage(i);
        Mat straight = correct(res);
        //imshow("straight", straight);
        //waitKey(0);
    }
    return 0;
}

Mat processRawImage(int fileIndex){


    char inputFile [50];
    sprintf(inputFile,"../input/%d.jpg",fileIndex);

    Mat source = imread(inputFile,1);
    //imshow("source",source);
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

    ////////////////////////////////////////////////////////////
    // Contour closing algorithm should come here when ready:
    //*
    //*
    //*
    ///////////////////////////////////////////////////////////

    double maskPerimeter = 0;
    int approxStrength = 6;
    Mat mask = drawLargestContour(bw,approxStrength,true);
    int morph_size = 3;
    Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    // Point morph size makes it omnidirectional
    morphologyEx(mask, mask, MORPH_CLOSE, element);

    //imshow("mask",mask);
    //waitKey(0);
    return mask;
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
    int lineThickness = 6;
    if(filled){
        drawContours(mask, contours,largestIndex, 255, lineThickness, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    }
    else{
        drawContours(mask, contours,largestIndex, 255, CV_FILLED, 8, hierarchy, 0); //instead of line thickness CV_FILLED
    }

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
    // input should be 1 channel black and white image

    //## Rotating the image using minimum area bounding rectangle:

    Mat bw;
    threshold(src, bw, 150, 255, CV_THRESH_BINARY); // INV: invert for findContours to work properly
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

    YouCanCommentLikeThisLOL:
    //Draw the rectangle:
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

    //## Done rotating the image using minimum area rectangle

    // ##Find the angle of all line segments and create a histogram, then decide the orientation based on that


    std::vector<Vec4i> lines;
    int lineThresh = 6;
    double minLineLength = 30;
    double maxLineGap = 7;


    Mat contourImage =  Mat::zeros(bw.size(), CV_8UC3);
    contourImage =  drawLargestContour(bw,1,false);
    imshow("bw",bw);
    waitKey(0);
    HoughLinesP(bw, lines, 1, CV_PI/180, lineThresh, minLineLength, maxLineGap); //needs binary image!!
    Mat lineImage = Mat::zeros(mask.size(),CV_8UC3);
    std::vector<Vec6f> segments; // Similar to vector<Vec4i> lines but with 2 additional data: Angle to x axis and length
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec6f lineObject;
        //calculate the length of the line:
        Point first = Point(lines[i][0], lines[i][1]);
        Point second = Point(lines[i][2], lines[i][3]);
        Point segment = second-first;
        double length = sqrt((segment.x)^2+(segment.y)^2);
        int theta = int(std::round((90+(180/CV_PI)*atan2(segment.y,segment.x)))); // angle between 0 and 180 degs
        //cout<<"theta: "<<theta<<endl;
        for(int j = 0; j< 4; j++){
            lineObject[j] = (lines[i][j]);
        }
        lineObject[4] = theta;
        lineObject[5] = float(length);
        segments.push_back(lineObject);
        // At this point we have a lineObject that contains x0, y0, x1, y1, the length and angle of a line
        if(length < 10){
            // Blue:
            line( lineImage, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(244,100,0), 1, 1 );
        }
        else{
            // Yellow:
            line( lineImage, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0,255,255), 1, 1 );
        }
    }

    vector<int>angles;
    for(size_t i = 0; i<segments.size();i++){
        angles.push_back(int(segments[i][4]));
    }
    float angleTolerance = 3;

    vector<int> angleHist = histogram(angles,int(angleTolerance));
    drawHistogram(angleHist);

    // Calculate the average frequency of any angle:
    double sum = 0;
    double noNonZero  = 0;
    for(size_t i = 0;i<angleHist.size(); i++){
        sum += angleHist[i];
        if(angleHist[i]!=0){
            noNonZero ++;
        }
    }
    double average = 0;
    if(noNonZero!=0){
        average = sum/noNonZero;
    }

    sum = 0;
    vector<Vec6f> significantSegments;
    for(size_t i = 0;i<angleHist.size(); i++) {
        if(angleHist[i]>int(average)){
           float angle =  angleTolerance*(i+1)-angleTolerance/2; // we get the angle that is in the middle of the range
            cout<<"Possibly statistically significant angles: "<<angle<<endl;
            Vec6f segment ;
            segment[4] = angle;
            significantSegments.push_back(segment);
        }
    }
    // working so far...

    // Arranging everything into groups based on their angles:
    vector<vector<Vec6f>>groups;
    groups.resize(significantSegments.size());
    for(size_t i = 0;i<segments.size(); i++){
        float ang = segments[i][4];
        //cout <<endl<<"angle to examine: "<< ang<<endl;
        for(size_t j = 0;j< significantSegments.size(); j++){
            float significantAngle =  significantSegments[j][4];
           // cout <<endl<<"significant angle to compare with: "<< significantAngle<<endl;
            if( significantAngle-angleTolerance/2 < ang && ang <=significantAngle+angleTolerance/2) {
               // cout<<"angle "<< ang <<"added to group: "<< j<<endl;
                groups[j].push_back(segments[i]);
            }

        }
    }

    for(size_t i = 0;i<groups.size(); i++){
        cout<<endl<< "Group"<< i<<": "<<endl;
        for(size_t j = 0;j<groups[i].size(); j++){
            cout<< "angle = "<<groups[i][j][4]<<endl;
        }

    }
    // Now this should be weighted with the lenght of the lines that the angles belong to
    // find significant lines, then find right angles between these lines (corners)
    imshow("lines",lineImage);
    waitKey(0);


    return mask;

}

int findmax(std::vector<int> vals){
    int max=vals[0];
    for(size_t i=0;i<vals.size();i++)
        if(max<vals[i]) max=vals[i];
    return max;
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