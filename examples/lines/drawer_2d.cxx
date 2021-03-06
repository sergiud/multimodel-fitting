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

#include "drawer_2d.hxx"
#include <iostream>
#include <random>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

drawer_2d& drawer_2d::get_instance(){
    static drawer_2d drawer("MultiModelFitting Visualization", 900, 900, 0.0f, 0.0f, 1.0f, 1.0f);
    return drawer;
}

drawer_2d::drawer_2d(const char * name, unsigned int width_, unsigned int height_, float x_min_, float y_min_, float x_max_, float y_max_)
    : width(width_), height(height_),
    x_min(x_min_), x_max(x_max_), y_min(y_min_), y_max(y_max_),
    visu(cv::Mat::zeros(int(height_), int(width_), CV_8UC3)),
    disp(name),
    rnd_gen(2), col_hue_detail_gen(0.0f,1.0f), col_hue_raw_gen(0,5)
{
    namedWindow( disp, cv::WINDOW_AUTOSIZE );
}

void drawer_2d::clear()
{
    visu.setTo(cv::Scalar(0,0,0));
}

void drawer_2d::draw_line(line_2d line, std::array<unsigned char, 3> const & color)
{
    point_2d p0 = line.get_point(x_max - x_min + y_max - y_min);
    point_2d p1 = line.get_point(x_min - x_max + y_min - y_max);

    draw_connection(p0, p1, color);
}

void drawer_2d::draw_connection(point_2d p0, point_2d p1, std::array<unsigned char, 3> const & color){
    float p0_x_f = (p0.x - x_min) / (x_max - x_min);
    float p0_y_f = (p0.y - y_min) / (y_max - y_min);
    float p1_x_f = (p1.x - x_min) / (x_max - x_min);
    float p1_y_f = (p1.y - y_min) / (y_max - y_min);
    unsigned int p0_x = static_cast<unsigned int>(p0_x_f * width);
    unsigned int p0_y = static_cast<unsigned int>(p0_y_f * height);
    unsigned int p1_x = static_cast<unsigned int>(p1_x_f * width);
    unsigned int p1_y = static_cast<unsigned int>(p1_y_f * height);

    cv::line(visu, cv::Point_<unsigned int>(p0_x, p0_y), cv::Point_<unsigned int>(p1_x, p1_y),
             CV_RGB(color[0], color[1], color[2]));
}


void drawer_2d::draw_point(point_2d point, std::array<unsigned char, 3> const & color)
{
    float pos_x_f = (point.x - x_min) / (x_max - x_min);
    float pos_y_f = (point.y - y_min) / (y_max - y_min);
    int pos_x = static_cast<int>(pos_x_f * width);
    int pos_y = static_cast<int>(pos_y_f * height);

    cv::rectangle(visu, cv::Point(pos_x - 2, pos_y - 2),
                        cv::Point(pos_x + 2, pos_y + 2),
                        CV_RGB(color[0], color[1], color[2]));
}

void drawer_2d::draw_cross(point_2d point, std::array<unsigned char, 3> const & color)
{
    float pos_x_f = (point.x - x_min) / (x_max - x_min);
    float pos_y_f = (point.y - y_min) / (y_max - y_min);
    int pos_x = static_cast<int>(pos_x_f * width);
    int pos_y = static_cast<int>(pos_y_f * height);

    cv::line(visu, cv::Point(pos_x - 2, pos_y - 2),
                   cv::Point(pos_x + 2, pos_y + 2),
                   CV_RGB(color[0], color[1], color[2]));
    cv::line(visu, cv::Point(pos_x - 2, pos_y + 2),
                   cv::Point(pos_x + 2, pos_y - 2),
                   CV_RGB(color[0], color[1], color[2]));

}

void drawer_2d::display()
{
    cv::imshow( disp, visu );;
}

void drawer_2d::sleep(int milliseconds)
{
    cv::waitKey(milliseconds);
}

void drawer_2d::wait()
{
    int res = 0;
    while(res != -1 && res != 'q' && res != 27){
        res = cv::waitKey(0);
        //std::cout << "waitKey(0) : " << res << std::endl;
    }
}

void drawer_2d::set_datapoints(std::vector<point_2d> const & points)
{
    this->datapoints = points;
}

void drawer_2d::set_hypotheses(std::vector<line_2d> const & hypotheses_)
{
    this->hypotheses = hypotheses_;

    colors_dots.clear();
    colors_lines.clear();
    colors_dots.reserve(hypotheses.size());
    colors_lines.reserve(hypotheses.size());

    unsigned char line_brightness = 180;
    unsigned char dot_brightness = 255;

    for (size_t i = 0; i < hypotheses.size(); i++) {

        std::array<float, 3> rnd_col = generate_color();

        std::array<unsigned char, 3> color_dot = {{
            static_cast<unsigned char>(rnd_col[0]*dot_brightness),
            static_cast<unsigned char>(rnd_col[1]*dot_brightness),
            static_cast<unsigned char>(rnd_col[2]*dot_brightness),
        }};
        std::array<unsigned char, 3> color_line = {{
            static_cast<unsigned char>(rnd_col[0] * line_brightness),
            static_cast<unsigned char>(rnd_col[1] * line_brightness),
            static_cast<unsigned char>(rnd_col[2] * line_brightness),
        }};
        colors_dots.push_back(std::move(color_dot));
        colors_lines.push_back(std::move(color_line));
    }
}

void drawer_2d::draw_all()
{
    clear();
    std::array<unsigned char,3> gray = {{ 180, 180, 180 }};
    for (size_t i = 0; i < hypotheses.size(); i++) {
        draw_line(hypotheses[i], colors_lines[i]);
    }
    for (auto const & datapoint : datapoints) {
        draw_point(datapoint, gray);
    }
    display();
}

std::array<float, 3> drawer_2d::generate_color()
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
