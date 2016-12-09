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

#include "drawer_2d.hxx"
#include "datapoints.hxx"
#include "problem_lines.hxx"

int main(){

    try {

        problem_lines config;

        // Create the datapoints
        auto datapoints = dataPoints::get();
        auto hypotheses = config.generateHypotheses(datapoints, 1000);

        // Initialization of drawer
        drawer_2d& drawer = drawer_2d::get_instance();
        drawer.set_datapoints(datapoints);
        drawer.set_hypotheses(hypotheses);
        drawer.draw_all();
        drawer.sleep(1000);

        // Initialize the algorithm classes
        mfigp::MultiModelFitter<problem_lines> fitter;

        // Print the Neighbourhood connections (TODO: remove)
        {
            auto connections = config.computeNeighbourhood(datapoints);
            std::array<unsigned char, 3> color = {{255,255,255}};
            for(auto const & connection : connections){
                drawer.draw_connection(
                    datapoints[connection[0]],
                    datapoints[connection[1]],
                    color
                );
            }
            drawer.display();
            drawer.sleep(0);//1000);
        }

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
