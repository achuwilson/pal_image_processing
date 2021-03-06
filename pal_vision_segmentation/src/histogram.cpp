/*
 * Software License Agreement (Modified BSD License)
 *
 *  Copyright (c) 2012, PAL Robotics, S.L.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of PAL Robotics, S.L. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/** \author Bence Magyar */


#include "histogram.h"
#include <opencv2/highgui/highgui.hpp>

namespace pal_vision_util
{
    cv::MatND calcHSVHist(const cv::Mat& src)
    {
        cv::Mat hsv;
        cv::cvtColor(src, hsv, CV_BGR2HSV);
        // Quantize the hue to 30 levels
        // and the saturation to 32 levels
        int hbins = 30, sbins = 32;
        int histSize[] = {hbins, sbins};
        // hue varies from 0 to 179, see cvtColor
        float hranges[] = { 0, 180 };
        // saturation varies from 0 (black-gray-white) to
        // 255 (pure spectrum color)
        float sranges[] = { 0, 256 };
        const float* ranges[] = { hranges, sranges };
        cv::MatND hist;
        // we compute the histogram from the 0-th and 1-st channels
        int channels[] = {0, 1};

        cv::calcHist( &hsv, 1, channels, cv::Mat(), // do not use mask
                  hist, 2, histSize, ranges,
                  true, // the histogram is uniform
                  false );

        return hist;
    }

    cv::Mat histogramImage(const cv::MatND& hist)
    {
        // Quantize the hue to 30 levels
        // and the saturation to 32 levels
        int hbins = 30, sbins = 32;
        double maxVal;
        cv::minMaxLoc(hist, 0, &maxVal, 0, 0);

        int scale = 10;
        cv::Mat histImg = cv::Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

        for( int h = 0; h < hbins; h++ )
            for( int s = 0; s < sbins; s++ )
            {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            cv::rectangle( histImg, cv::Point(h*scale, s*scale),
                           cv::Point( (h+1)*scale - 1, (s+1)*scale - 1),
                           cv::Scalar::all(intensity),
                           CV_FILLED );
        }
        return histImg;
    }

    void showHist(const cv::MatND& hist)
    {
        cv::Mat histImg = histogramImage(hist);

        cv::namedWindow( "H-S Histogram", 1 );
        cv::imshow( "H-S Histogram", histImg );
        cv::waitKey();
    }

}
