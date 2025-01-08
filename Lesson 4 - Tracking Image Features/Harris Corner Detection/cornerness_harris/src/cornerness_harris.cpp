#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    // visualize results
    string windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 1);
    cv::imshow(windowName, dst_norm_scaled);
    cv::waitKey(0);

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.
    std::vector<cv::KeyPoint> keypoints;

    int sw_size = 7;  //sliding window size, should be odd
    int sw_dist = floor(sw_size / 2);

    //create output image
    int nrows = dst_norm_scaled.rows;
    int ncols = dst_norm_scaled.cols;
    cv::Mat result_img = cv::Mat::zeros(nrows, ncols, CV_8U);

    for (int r = sw_dist; r < nrows - sw_dist - 1; r++)
    {
        for (int c = sw_dist; c < ncols - sw_dist - 1; c++)
        {
            unsigned int max_val{0};

            for (int rs = r - sw_dist; rs <= r + sw_dist; rs++)
            {
                for (int cs = c - sw_dist; cs <= c + sw_dist; cs++)
                {
                    unsigned int new_val = dst_norm_scaled.at<unsigned int>(rs, cs);
                    if (new_val > max_val)
                        max_val = new_val;
                }
            }

            // if the current pixel is the max val then save as local maximum
            if (dst_norm_scaled.at<unsigned int>(r, c) == max_val)
            {
                keypoints.push_back(cv::KeyPoint(c, r, 2.0));
                result_img.at<unsigned int>(r, c) = max_val;
            }
            else
            {
                result_img.at<unsigned int>(r, c) = 0;
            }
                
        }
    }

    // show NMS image
    std::string windowName2 = "NMS Result Image";
    cv::namedWindow(windowName2, 1);
    //cv::Mat visImage = dst_norm_scaled.clone();
    //cv::drawKeypoints(dst_norm_scaled, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imshow(windowName2, result_img);
    cv::waitKey(0);

}

int main()
{
    cornernessHarris();
}