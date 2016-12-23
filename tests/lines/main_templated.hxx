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

#include "../../examples/lines/datapoints.hxx"
#include "problem_lines.hxx"

#include "correct_result.hxx"

template<typename precision>
int main_templated(){

    try {

        typedef problem_lines<precision> problem_type;

        problem_type config;

        // Create the datapoints
        auto datapoints = dataPoints::get();
        auto hypotheses = config.generateHypotheses(datapoints, 1000);

        // Initialize the algorithm classes
        mfigp::MultiModelFitter<problem_type> fitter;

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

    }
    catch (std::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
