/*
* Copyright (C) 2016  Martin Stumpf
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "drawer_kinect.hxx"
#include <iostream>
#include <random>

drawer_kinect drawer("Kinect Fitting Visualization");

extern unsigned char img_rgb[];
extern size_t img_rgb_len;

drawer_kinect::drawer_kinect(const char * name)
    : disp(name),
    rnd_gen(2), col_hue_detail_gen(0.0f,1.0f), col_hue_raw_gen(0,5)
{
    std::vector<unsigned char> img_rgb_data(img_rgb,img_rgb+img_rgb_len);
    img_background = cv::imdecode(cv::Mat(img_rgb_data), CV_LOAD_IMAGE_GRAYSCALE);

    width=img_background.cols;
    height=img_background.rows;

    visu = cv::Mat::zeros(width*2, height*2, CV_8UC3);

    namedWindow( disp, cv::WINDOW_AUTOSIZE );
}

void drawer_kinect::clear()
{
    visu.setTo(cv::Scalar(0,0,0));
}

size_t drawer_kinect::getX(size_t id)
{
    return id%width;
}

size_t drawer_kinect::getY(size_t id)
{
    return id/width;
}



void drawer_kinect::draw_connection(size_t id0, size_t id1){
    unsigned int p0_x = static_cast<unsigned int>(2*getX(id0));
    unsigned int p0_y = static_cast<unsigned int>(2*getY(id0));
    unsigned int p1_x = static_cast<unsigned int>(2*getX(id1));
    unsigned int p1_y = static_cast<unsigned int>(2*getY(id1));

    cv::line(visu, cv::Point(p0_x, p0_y), cv::Point(p1_x, p1_y),
             CV_RGB(255,255,255));
}


void drawer_kinect::display()
{
    cv::imshow( disp, visu );;
}

void drawer_kinect::sleep(int milliseconds)
{
    cv::waitKey(milliseconds);
}

void drawer_kinect::wait()
{
    int res = 0;
    while(res != -1 && res != 'q' && res != 27){
        res = cv::waitKey(0);
        //std::cout << "waitKey(0) : " << res << std::endl;
    }
}

std::array<float, 3> drawer_kinect::generate_color()
{
    float col_detail = col_hue_detail_gen(rnd_gen);
    unsigned int col_region = col_hue_raw_gen(rnd_gen);
    std::array<float, 3> out;
    switch (col_region) {
    case 0:
        out = {{ 1.0f, col_detail, 0.0f }};
        break;
    case 1:
        out = {{ col_detail, 1.0f, 0.0f }};
        break;
    case 2:
        out = {{ 0.0f, 1.0f, col_detail }};
        break;
    case 3:
        out = {{ 0.0f, col_detail, 1.0f }};
        break;
    case 4:
        out = {{ col_detail, 0.0f, 1.0f }};
        break;
    case 5:
        out = {{ 1.0f, 0.0f, col_detail }};
        break;
    default:
        out = {{ 1.0f, 1.0f, 1.0f }};
        break;
    }
    return out;
}
