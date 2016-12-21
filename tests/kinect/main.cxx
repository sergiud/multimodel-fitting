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

#include <iostream>
#include <mfigp.hxx>

#include "../../examples/kinect/data_reader.hxx"
#include "../../examples/kinect/problem_kinect.hxx"

#include "correct_result.hxx"

int main(){

    try {

        problem_kinect config;

        // Create the datapoints
        auto datapoints = data_reader::get();
        auto hypotheses = config.generateHypotheses( datapoints, 1000 );

        // Initialize the algorithm classes
        mfigp::MultiModelFitter<problem_kinect> fitter;

        // set the input data
        fitter.set_samples(datapoints);
        fitter.set_hypotheses(hypotheses);

        // run the algorithm
        std::cout << "Samples: " << datapoints.size() << std::endl;
        std::cout << "Hypotheses: " << hypotheses.size() << std::endl;
        auto labeling = fitter.fit(config);
        std::cout << "Converged." << std::endl;

        // cleanup
        fitter.clear_hypotheses();
        fitter.clear_samples();

        // check result
        for(size_t i = 0; i < labeling.size(); i++){
            if(labeling[i] != correct_result[i]){
                std::cout << "Result is not correct."
                          << "(label[" << i << "] = " << labeling[i] << "; "
                          << "Expected: " << correct_result[i] << ")"
                          << std::endl;
                return 1;
            }
        }

        /*
        size_t i = 0;
        std::cout << "    ";
        for(const auto & label : labeling){
            if(i>0){
                if(i % 10 == 0){
                    std::cout << "," << std::endl << "    ";
                } else {
                    std::cout << ", ";
                }
            }
            std::string label_string = std::to_string(label);
            for(int j = 0; j + label_string.length() < 3; j++){
                std::cout << " ";
            }
            std::cout << label_string;
            i++;
        }
        std::cout << std::endl;
        */

    }
    catch (std::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
