#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

// #include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
// #include "RPLidarController.h"
// #include "OccuGridMap.h"
// #include "IncrementalMLMapping.h"
// #include "ServoController.h"
// #include "General.h"
// #include "MeasurementModel.h"
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
#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>

#include <cstdio>

#include <boost/thread.hpp>

using namespace Eigen;
using namespace std;
// using namespace cv;

using namespace boost::program_options;

variables_map gPROG_ARGUMENT;


int processCommandLineArgument(int argc, char **argv);

// Json::Value mapJson(vector<Vector2f> coordinates){
//     Json::Value res, tmp;
//     Json::Value tmp_arr;
//     for(int i = 0; i < coordinates.size(); ++i){
//         tmp["x"] = coordinates[i][0];
//         tmp["y"] = coordinates[i][1];
//         tmp_arr[i] = tmp;
//     }
//     res["data"] = tmp_arr;
//     return res;
// };


int main(int argc, char **argv){
	// ofstream f;
	// f.open("file.txt");

	if (processCommandLineArgument(argc, argv) < 0)
	{
		return 0;
	}
	
	//test function mapJson
	// std::vector<Vector2f> v;
	// v.push_back(Vector2f(0,1));
	// v.push_back(Vector2f(0,2));
	// v.push_back(Vector2f(0,3));
	// Json::Value t = mapJson(v);
	// Json::StyledWriter s;
	// f << s.write(t);
	// f.close();

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