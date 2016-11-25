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

#include "data_reader.hxx"
#include "problem_kinect.hxx"
#include "drawer_kinect.hxx"

int main(int argc, char *argv[]){

    try {

        problem_kinect config;

        // Create the datapoints
        auto datapoints = data_reader::get();
        auto hypotheses = config.generateHypotheses( datapoints, 500 );

        // initialize drawer
        drawer.set_datapoints(datapoints);
        drawer.set_hypothesis_count(hypotheses.size());
        drawer.set_neighbourhood(config.computeNeighbourhood(datapoints));

        // Print the Neighbourhood connections (TODO: remove)
        drawer.draw_empty();
        drawer.sleep(1000);

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

        // wait for the user to close the window
        drawer.draw_labeled(labeling);

        drawer.wait();

    }
    catch (std::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
