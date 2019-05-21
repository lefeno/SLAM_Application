#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
#include "RPLidarController.h"
#include "OccuGridMap.h"
#include "IncrementalMLMapping.h"
#include "ServoController.h"
#include "General.h"
#include "MeasurementModel.h"
#include "Employee.h"
#include "Mediator.h"
#include "json/json.h"

#include <math.h>
#include <sys/time.h>
#include <Eigen/Dense>	//Library for matrix and vector
#include <vector>

#include<fstream>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>

using namespace Eigen;
using namespace std;

using namespace boost::program_options;

variables_map gPROG_ARGUMENT;

int processCommandLineArgument(int argc, char **argv);

int main(int argc, char **argv){

	if (processCommandLineArgument(argc, argv) < 0)
	{
		return 0;
	}

	Mediator mediator;

	mediator.start();

	return 0;
}

int processCommandLineArgument(int argc, char **argv)
{
    options_description usage("Usage");

	usage.add_options()
			("help,h", "Print help message")
            ("port,p", value<int>()->default_value(59998), "Server port");

	try
	{
        store(command_line_parser(argc, argv).options(usage).run(), gPROG_ARGUMENT);

		if (gPROG_ARGUMENT.count("help"))
		{
			cout << usage << endl;
			return -1;
		}

        notify(gPROG_ARGUMENT);
	}
    catch (required_option& e)
	{
		cout << "ERROR: " << e.what() << endl;
		cout << usage << endl;
		return -1;
	}
    catch (error& e)
	{
		cout << "ERROR: " << e.what() << endl;
		return -1;
	}

	return 0;
}