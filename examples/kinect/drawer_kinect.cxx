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
    rnd_gen(11), col_hue_detail_gen(0.0f,1.0f), col_hue_raw_gen(0,5)
{
    std::vector<unsigned char> img_rgb_data(img_rgb,img_rgb+img_rgb_len);
    img_background = cv::imdecode(cv::Mat(img_rgb_data), CV_LOAD_IMAGE_GRAYSCALE);

    int width, height;
    width=img_background.cols;
    height=img_background.rows;

    visu = cv::Mat::zeros(height, width, CV_8UC3);

    namedWindow( disp, cv::WINDOW_AUTOSIZE );
}

void
drawer_kinect::draw_empty(){
    std::vector<int32_t> labels(datapoints.size());
    for(size_t i = 0; i < labels.size(); i++){
        labels[i] = -1;
    }
    draw_labeled(labels);
}

void
drawer_kinect::draw_labeled(std::vector<int32_t> const & labels){
    clear();
    auto outlier_color = CV_RGB(64,64,64);

    for (size_t i = 0; i < labels.size(); i++) {
        int pos_x = datapoints[i].u;
        int pos_y = datapoints[i].v;
        if(labels[i] >= 0){
            auto const & color =  colors[labels[i]];
            cv::rectangle(visu, cv::Point(pos_x - 1, pos_y - 1),
                                cv::Point(pos_x + 1, pos_y + 1),
                                CV_RGB(255*color[0], 255*color[1], 255*color[2]));
        } else {
            cv::line(visu, cv::Point(pos_x - 1, pos_y - 1),
                           cv::Point(pos_x + 1, pos_y + 1),
                           outlier_color);
            cv::line(visu, cv::Point(pos_x - 1, pos_y + 1),
                           cv::Point(pos_x + 1, pos_y - 1),
                           outlier_color);
        }
    }

    for(auto const & connection : neighbourhood){
        if(labels[connection[0]] != labels[connection[1]] ||
           labels[connection[0]] == -1) continue;
        auto const & color = colors[labels[connection[0]]];
        auto const & p0 = datapoints[connection[0]];
        auto const & p1 = datapoints[connection[1]];
        cv::line(visu, cv::Point(p0.u, p0.v),
                       cv::Point(p1.u, p1.v),
                       CV_RGB(255*color[0], 255*color[1], 255*color[2]));
    }

    display();
}

void
drawer_kinect::set_datapoints(std::vector<point_3d> const & datapoints_){
    this->datapoints = datapoints_;
}

void
drawer_kinect::set_neighbourhood(std::vector<std::array<size_t, 2>> const & neighbourhood_){
    this->neighbourhood = neighbourhood_;
}

void drawer_kinect::set_hypothesis_count(size_t count){
    colors.clear();
    colors.reserve(count);

    for(size_t i = 0; i < count; i++){
        colors.push_back(generate_color());
    }
}

void drawer_kinect::clear()
{
    //img_background.convertTo(visu, CV_8UC3, 0.3f, 0);
    visu.setTo(cv::Scalar(0,0,0));
    cvtColor(img_background, visu, CV_GRAY2RGB);
}

void drawer_kinect::draw_connection(size_t id0, size_t id1){
    int p0_x = datapoints[id0].u;
    int p0_y = datapoints[id0].v;
    int p1_x = datapoints[id1].u;
    int p1_y = datapoints[id1].v;

    cv::line(visu, cv::Point_<int>(p0_x, p0_y), cv::Point_<int>(p1_x, p1_y),
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
