#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void magnitudeSobel()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");

    // convert image to grayscale
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // apply smoothing using the GaussianBlur() function from the OpenCV
    // https://docs.opencv.org/4.x/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html
    cv::Mat img_blurred;
    cv::Size kernel_size (5, 5);
    cv::GaussianBlur(imgGray, img_blurred, kernel_size, 2.0, 2.0, cv::BORDER_DEFAULT);

    // create filter kernels using the cv::Mat datatype both for x and y
    float sobel_x[9] = {-1, 0, +1,
                        -2, 0, +2, 
                        -1, 0, +1};
    cv::Mat kernel_x = cv::Mat(3, 3, CV_32F, sobel_x);

    float sobel_y[9] = {-1, -2, -1,
                         0,  0,  0, 
                        +1, +2, +1};
    cv::Mat kernel_y = cv::Mat(3, 3, CV_32F, sobel_y);

    // apply filters using the OpenCv function filter2D()
    // ToDo : Add your code here
    cv::Mat result_x;
    cv::filter2D(img_blurred, result_x, -1, kernel_x, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    cv::Mat result_y;
    cv::filter2D(img_blurred, result_y, -1, kernel_y, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // compute magnitude image based on the equation presented in the lesson 
    // ToDo : Add your code here
    // create matrix
    int nrows = img_blurred.rows;
    int ncols = img_blurred.cols;
    cv::Mat img_magnitude;
    img_magnitude.create(nrows, ncols, CV_8U);
    img_magnitude.setTo(cv::Scalar(0));

    for (int r = 0; r < nrows; r++)
    {
        for (int c = 0; c < ncols; c++)
        {
            img_magnitude.at<unsigned char>(r, c) = sqrt(result_x.at<unsigned char>(r, c)*result_x.at<unsigned char>(r, c) + result_y.at<unsigned char>(r, c)*result_y.at<unsigned char>(r, c));
            //img_magnitude.at<unsigned char>(r, c) = result_x.at<unsigned char>(r, c);
        }
    }
    
    // show result
    string windowName = "Sobel Magnitude after blurring";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, img_magnitude);
    cv::waitKey(0); // wait for keyboard input before continuing
}

int main()
{
    magnitudeSobel();
}