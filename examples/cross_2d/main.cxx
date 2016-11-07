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
#include <multimodelfitting.hxx>

#include "problem_ortholines.hxx"

#include "drawer_2d.hxx"
#include "dataset_generator.hxx"

int main(int argc, char *argv[]){
  
    try {

        // Initialize the visualization window
        auto drawer = std::make_shared<drawer_2d>("cross_2d", 500, 500, 0.0f, 0.0f, 1.0f, 1.0f);

        // Create the datapoints
        auto datapoints = dataset_generator::generate(400,200);
        auto hypotheses = dataset_generator::compute_hypotheses(datapoints, 100);

        // Initialization of drawer
        drawer->set_datapoints(datapoints);
        drawer->set_hypotheses(hypotheses);
        drawer->draw_all();
        
        // Initialize the algorithm classes
        problem_ortholines config(drawer);
        MultiModelFitter<problem_ortholines> fitter;

        { // TODO remove
            auto connections = config.computeNeighbourhood(datapoints); 
            std::array<unsigned char, 3> color = {255,255,255};
            for(auto const & connection : *connections){
                drawer->draw_connection(
                    datapoints[connection[0]],
                    datapoints[connection[1]],
                    color
                );
            }
            drawer->display();
        }

        // set the input data
        fitter.set_samples(datapoints);
        fitter.set_hypotheses(hypotheses);

        // run the algorithm
        fitter.fit(config);

        // cleanup
        fitter.clear_hypotheses();
        fitter.clear_samples();

        // wait for the user to close the window
        drawer->wait();
    }
    catch (std::exception const & e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
