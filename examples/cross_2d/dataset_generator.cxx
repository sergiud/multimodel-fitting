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

#include "dataset_generator.hxx"

std::vector<point_2d> dataset_generator::generate(size_t num_data, size_t num_outliers)
{
    std::vector<point_2d> data = generate_data(num_data);
    std::vector<point_2d> outliers = generate_outliers(num_outliers);

    data.reserve(num_data + num_outliers);
    data.insert(data.end(), outliers.begin(), outliers.end());

    return data;
}

std::vector<point_2d> dataset_generator::generate_data(size_t num)
{
    std::random_device rd;
    std::mt19937 gen(1);//rd());
    auto norm_gen = std::normal_distribution<float>(0.0f, 0.01f);
    auto real_gen = std::uniform_real_distribution<float>(0.1f, 0.9f);
    std::vector<point_2d> result;
    result.reserve(num);
    for (size_t i = 0; i < num; i++) {
        float f1 = real_gen(gen);
        float f2 = norm_gen(gen);
        float x = f1+f2;
        float y = f1-f2;
        if (i % 2 == 0)
            x = 1.0f-x;
        result.push_back(point_2d(x, y));
    }
    return result;
}

std::vector<point_2d> dataset_generator::generate_outliers(size_t num)
{
    std::random_device rd;
    std::mt19937 gen(2);//rd());
    std::uniform_real_distribution<float>real_gen(0.0f, 1.0f);
    std::vector<point_2d> result;
    result.reserve(num);
    for (size_t i = 0; i < num; i++) {
        result.push_back(point_2d(real_gen(gen), real_gen(gen)));
    }
    return result;
}

std::vector<line_2d> dataset_generator::compute_hypotheses(std::vector<point_2d> const & datapoints, size_t num) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> index_gen1(0, num - 1);
    std::uniform_int_distribution<size_t> index_gen2(0, num - 2);

    std::vector<line_2d> hypotheses;
    hypotheses.reserve(num);
    hypotheses.push_back(line_2d(point_2d(0.0f, 1.0f), point_2d(1.0f, 0.0f)));
    hypotheses.push_back(line_2d(point_2d(0.0f, 0.0f), point_2d(1.0f, 1.0f)));
    hypotheses.push_back(line_2d(point_2d(0.0f, 1.0f), point_2d(0.9f, 0.0f)));
    /*for (size_t i = 0; i < num-2; i++) {
        size_t p0 = index_gen1(gen);
        size_t p1 = index_gen2(gen);
        if (p1 >= p0) p1++;
        hypotheses.push_back(line_2d(datapoints[p0], datapoints[p1]));
    }*/

    return hypotheses;
}
