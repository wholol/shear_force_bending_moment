#include "pch.h"
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
ofstream Shear_force("shear_force.csv"); //instantiate object for writing data to csv file
ofstream bendingmoments("bending_moments.csv"); //instantiate object for beding moments csv file


class drawBeam {									//beamdiagram class (only for drawing and storing).
protected:										//inheritance used, hence protected
	double beamlength;									//beam's length
	char beamDraw[25][200];								//beam array
	int scale_factor = 5;								//scales the beam in the diagram to the actual length.

	//parameter variables. The force type is defined first, followed by either magnitude or location of force.

	vector <double> point_force_magnitude;				//MAGNITUDE of point force.
	vector <double> point_force_location;				// stores the laod force location.

	vector <double> distributed_w_value;				//w value for distributed force
	vector <double> load_force_magnitude;				//load force magnitude
	vector <double> load_force_location_begin;			//stores the beginning of the load force location.
	vector <double> load_force_locaton_end;				//stores the end of the load force lcoation.
	vector <double> load_force_centroid_location;		//centroid of the force

	vector <double> support_location;					//support location
	vector <double> support_magnitude;					// store the location of the supprots (m)

	vector <double> point_force_moments;				// stores the sum of moments for the point force 
	vector <double> load_force_moments;					// stores the sum of of moments for the load force
	vector <double> store_force;						//store the force magnitude
	vector <double> store_location;						//store the vector magnitude

public:

	void initBeam() {												//draw the beam for the user.
		for (int row = 0; row < 25; row++) {
			for (int column = 0; column < 200; column++) {
				cout << beamDraw[row][column];
			}
			cout << endl;
		}
	}

	void setBeam() {												//set beam length.
		for (int row = 0; row < 25; row++) {
			for (int column = 0; column < 200; column++) {
				beamDraw[row][column] = ' ';
			}
		}
		cout << "enter beam length" << endl;
		cin >> beamlength;											// user eneters beam length
		while (beamlength > 38) {
			cout << "the beam has a maximum length of 38 m only. Enter again" << endl;	// the program only sves for the beam of up to 38 m.
			cin >> beamlength;
		}
		for (int column = 0; column <= beamlength * scale_factor; column++) {		//draws the beam in the interface
			beamDraw[12][column] = '=';
		}
	}

	void setForces() {								//draws forces.
		while (1) {
			double force_location; char force_direction; char escape; char force_type; double force_value; //beam properties for drawing.
			cout << "enter force type. p for point load, l for uniform distributed load" << endl;
			cin >> force_type;																				//asks the user to eter the force type.
			while ((force_type != 'p') && (force_type != 'l')) {											// if the force type is not a point load or distributed load.
				cout << "invalid ouput" << endl;
				cin >> force_type;
			}

			if (force_type == 'p') {													// point load.
				cout << "Enter force location." << endl;								// force location.
				cin >> force_location;
				point_force_location.push_back(force_location);					//stores the point force vector.
				store_location.push_back(force_location);								//stores the force's location;
				while (force_location > beamlength) {									// if the specidfied force location is not within the beam's length
					cout << "invalid output" << endl;
					cin >> force_location;
				}
				cout << "enter force direction. Enter 'u' for upwards force, 'd' for downwards force." << endl;		//force direction
				cin >> force_direction;
				while ((force_direction != 'u') && (force_direction != 'd')) {		// if the force direction entererd is invalid
					cout << "invalid ouput" << endl;
					cin >> force_direction;
				}

				if (force_direction == 'd') {												//downward force
					cout << "enter force magnitude in Newton. (postive only)" << endl;
					cin >> force_value;
					store_force.push_back(force_value*-1);						//NEW. store the force magnitude
					point_force_magnitude.push_back(force_value*-1);			//store point force magnitude
					for (int row = 10; row >= 4; row--) {						//draws the force
						int beamlengthConvert = force_location * scale_factor;
						beamDraw[row][beamlengthConvert] = '|';
						beamDraw[11][beamlengthConvert] = 'v';
					}
				}

				if (force_direction == 'u') {									//upward force
					cout << "enter force magnitude in Newton. (postive only)" << endl;
					cin >> force_value;
					store_force.push_back(force_value);				//NEW.. store the force magnitude
					point_force_magnitude.push_back(force_value);		//store force
					for (int row = 14; row <= 18; row++) {
						int beamlengthConvert = force_location * scale_factor;
						beamDraw[row][beamlengthConvert] = '|';
						beamDraw[13][beamlengthConvert] = '^';
					}
				}
			}

			///////////////////////////////////////////////////////////////////////////////////distributed force below//////////////////////////////////////////////////////////////////////////////

			if (force_type == 'l') {							//distributed load setting//
				double force_location_begin; double force_location_end; char force_direction_distributed;
				cout << " specify force location at left end (meters)." << endl;
				cin >> force_location_begin;								//force begin = left end. hence it should have a smaller value.
				load_force_location_begin.push_back(force_location_begin);


				while (force_location_begin > beamlength) {									// if location is greater than beamlength
					cout << "invalid output" << endl;
					cin >> force_location_begin;											// stores the location where the force begins
					load_force_location_begin.push_back(force_location_begin);		//stores the location where the force begins
				}

				cout << "specify force location at right end (meters)." << endl;
				cin >> force_location_end;
				load_force_locaton_end.push_back(force_location_end);				// stores the location where the force ends
				while (force_location_end > beamlength) {
					cout << "invalid output" << endl;
					load_force_locaton_end.push_back(force_location_end);			//stores the locaction where the force end
					cin >> force_location_end;
				}

				cout << "forces direction. enter u for upwards, d for downwards." << endl;
				cin >> force_direction_distributed;



				if (force_direction_distributed == 'u') {		//upward force for distributed load
					cout << "enter load distribution in N/m" << endl;
					cin >> force_value;
					distributed_w_value.push_back(force_value);		//stores load distribution gradient value. ideally same size as force begin and end

					for (int row = 14; row <= 16; row++) {											//this loop is pretty much for drawing purposes
						int ConvertForceBegin = force_location_begin * scale_factor;				//scale the force for drawing purposes and also convert to int
						int ConvertForceEnd = force_location_end * scale_factor;					// scale the force for drawing purposes and convert to int
						beamDraw[row][ConvertForceBegin] = '|';
						beamDraw[13][ConvertForceBegin] = '^';
						beamDraw[row][ConvertForceEnd] = '|';
						beamDraw[13][ConvertForceEnd] = '^';
						for (int straightLine = ConvertForceBegin; straightLine <= ConvertForceEnd; straightLine++) {
							beamDraw[16][straightLine] = '-';
							beamDraw[13][straightLine] = '^';
						}
					}
				}

				if (force_direction_distributed == 'd') {		// downward force for distributed load
					cout << "enter load distribution in N/m" << endl;
					cin >> force_value;
					distributed_w_value.push_back(force_value*-1);			//stores load force magnitude
					for (int row = 10; row >= 7; row--) {
						int ConvertForceBegin = force_location_begin * scale_factor;
						int ConvertForceEnd = force_location_end * scale_factor;
						beamDraw[row][ConvertForceBegin] = '|';
						beamDraw[11][ConvertForceBegin] = 'v';
						beamDraw[row][ConvertForceEnd] = '|';
						beamDraw[11][ConvertForceEnd] = 'v';
						for (int straightLine = ConvertForceBegin; straightLine <= ConvertForceEnd; straightLine++) {
							beamDraw[7][straightLine] = '-';
						}
					}
				}
			}
			cout << "enter e to quit force setup and proceed to support setup, else press anything else." << endl;
			cin >> escape;
			if (escape == 'e') {
				break;
			}
		}
	}

	void setSupports() {
		double support_w; int InitSupport = 9999;			//initiate the supprot value to be zero.
		while (1) {
			cout << "enter your support locations." << endl;
			cin >> support_w;
			int supportConvert = support_w * scale_factor;
			while (beamDraw[13][supportConvert] == '^') {
				cout << "there is currently a force being placed here. Insert support again." << endl;
				cin >> support_w;
				supportConvert = support_w * scale_factor;
			}
			store_force.push_back(InitSupport);								//NEW. STORE SUPPORT MAGNITUDE.
			store_location.push_back(support_w);						//NEW. STORE SUPPORT LOCATION.
			support_location.push_back(support_w);
			beamDraw[13][supportConvert] = 'O';
			if (support_location.size() == 2) {
				if (support_location[0] > support_location[1]) {		//this condition allows us to set the smaller support location value to be always the first one.
					swap(support_location[0], support_location[1]);		//swaps supports. we always use the left support to initiate the moment calculation.
				}
				break;
			}
		}
	}
};

class beamCalc :public drawBeam {							//the BeamCalc class provides the computational analysis of the shear force and bending moment.
private:
	vector <double> shear_to_moments;					
	vector <double> bending_moment_location;
	vector <double> shear_force_area;					//computes the shear force area to be added for the bending momenets
	vector <double> bending_moment_magnitude;
	vector <pair <double, double> > vectPair;			//sorting purposes to couple force and location vector
	vector <double> shear_force;						//stores the end values of shear force that needs to be plotted
	double ShearForce = 0;							// initialize a previousshear variable which would store the previous shear force

public:

	void Compute_support_reaction() {
		double sum_moments_point_force = 0;					//sum of My due to point force = 0
		double sum_magnitude_point_force = 0;				//sum of Fy due to point force = 0
		double sum_moments_load_force = 0;					// sum of My due to load force = 0	
		double sum_magnitude_load_force = 0;				// sum of Fy due to load force = 0
	
		if (point_force_location.size() > 0) {					//checks for point force presence.
			for (int i = 0; i < point_force_location.size(); i++) {		//loop through all point forces
				double pointForceMoments = point_force_magnitude[i] * (point_force_location[i] - support_location[0]);	//compute the moments for each point
				point_force_moments.push_back(pointForceMoments);				//contains the moments for each point force into this vector
				sum_moments_point_force += point_force_moments[i];			//sums all the moments of eahc force
				sum_magnitude_point_force += point_force_magnitude[i];		//sums all the magnitude of forces 
			}
		}

		if (load_force_location_begin.size() > 0) {												//checks for load force presence.
			for (int i = 0; i < distributed_w_value.size(); i++) {						//for each looad force element
				double storeCentroid = (load_force_locaton_end[i] - load_force_location_begin[i])*0.5;		//store the centroid value for each load
				//cout << store_load_force_location_begin[i] << "," << store_load_force_location_end[i] << endl;
				load_force_centroid_location.push_back(storeCentroid);// stores the centroid of each element into this array


				double storeLoadForce = distributed_w_value[i] * (abs(load_force_location_begin[i] - load_force_locaton_end[i]));	//force*distance for force value.location not calculated.												
				load_force_magnitude.push_back(storeLoadForce);			//stores each load force into this vector
				//cout << storeLoadForce << endl;

				double storeLoadMoments = load_force_magnitude[i] * (load_force_centroid_location[i] + load_force_location_begin[i] - support_location[0]);		//store the load force moments
				load_force_moments.push_back(storeLoadMoments);		//stores all moments of each load forces into this array
				//cout << storeLoadMoments << endl;							

				sum_moments_load_force += load_force_moments[i];		//adds all the moments load forces 		
				sum_magnitude_load_force += load_force_magnitude[i];	// adds the magnitude of all load forces
			}
		}

		double storeSupport = -(sum_moments_point_force + sum_moments_load_force) / (abs(support_location[1] - support_location[0])); //compute first support value. Fsupport = Moment/distance
		support_magnitude.push_back(storeSupport);					//store the supports in the vector
		storeSupport = -support_magnitude[0] - (sum_magnitude_point_force + sum_magnitude_load_force);  	//compute second support value.
		support_magnitude.push_back(storeSupport);
		cout << "the support at " << support_location[1] << " meters is " << support_magnitude[0] << endl;			//prints out support value at [1] since [0] is stationary.
		for (int i = 0; i < store_force.size(); i++) {
			if (store_force[i] == 9999 && store_location[i] == support_location[1]) {			//detects if a store force element is 9999, since the supports were initialized to be 9999
				store_force[i] = support_magnitude[0];
			}
			else if (store_force[i] == 9999 && store_location[i] == support_location[0]) {
				store_force[i] = support_magnitude[1];
			}

		}
		cout << "the support at " << support_location[0] << " meters is " << support_magnitude[1] << endl;			//prints out support value
		// stationary support is the last to be computed, hence it is store_support_magnitude[1] instead.
	}


	void computeShearForce() {					//function to compute shear force.

		for (int i = 0; i < store_force.size(); i++) {								//sorting the store_location vetor and store_force vector
			vectPair.push_back(make_pair(store_location[i], store_force[i]));		//sorting vector 

		}

		sort(vectPair.begin(), vectPair.end());					//sorting the vectors. SORTS LOCATION IN ACSESDING ORDER. this vector is important when it comes to sectioning
		shear_force.push_back(vectPair[0].second);				//initialize 0th element of shear force with the initial force to the left. for plotting.

															// if there is no presence of load force
				for (int i = 1; i < store_force.size(); i++) {
					ShearForce = vectPair[i].second + shear_force[i - 1];
					shear_force.push_back(ShearForce);
				}
			

		
		for (double i = 0; i <= vectPair[0].first; i += 0.2) {
			Shear_force << i << "," << 0 << endl;				//fill the beam with zero forces, IF there is no shear force prevalent, or it is out of bounds
		}

		for (int force_plot = 0; force_plot < shear_force.size() - 1; force_plot++) {		//goes through each loaction, except the last one (last element)
			for (float i = vectPair[force_plot].first; i < vectPair[force_plot + 1].first; i += 0.2) {	//further split the locations into increments of 0.2


				if (i == vectPair[force_plot].first && force_plot != 0) {				//if the value of i is the value of the location array, and it is not at the beginning.	
					Shear_force << i << "," << shear_force[force_plot - 1] << endl;		//plots the extra point on the location where the force exists.
					//cout << i << "," << shear_force[force_plot - 1] << endl;
				}
				Shear_force << i << "," << shear_force[force_plot] << endl;			//only for point force
				//cout << shear_force[force_plot] << endl;
				shear_to_moments.push_back(shear_force[force_plot]);		//to plot the bending moment of the diagram

			}
		}
	
		for (double i = vectPair[vectPair.size() - 1].first; i <= beamlength; i += 0.2) {		//fills remaning parts of the beam that is out of bounds with zero
			Shear_force << i << "," << 0 << endl;
		}


		for (int i = 0; i < shear_force.size(); i++) {				//test//
		//	cout << "shear forces: " << shear_force[i] << endl;
		}

		for (int i = 0; i < vectPair.size(); i++) {
		//	cout << " these are the store_location numbers: " << vectPair[i].first << "   These are the store_force numbers: " << vectPair[i].second << endl;
		}
	}

	void computeBendingMoments() {
		bendingmoments << "beam distance" << "," << "bending moment" << endl;

		for (double i = 0; i <= vectPair[0].first; i += 0.2) {
			bendingmoments << i << "," << 0 << endl;				//fill the beam with zero forces, IF there is no shear force prevalent, or it is out of bounds
		}
												//IF THERE IS ONLY POINT FORCES AVAILABLE

			for (int j = 0; j < shear_force.size() - 1; j++) {	//location loop
				double location = (vectPair[j + 1].first - vectPair[j].first);
				double shearForceArea = shear_force[j] * location;
			//	cout << shearForceArea << "these are the shear force areas " << endl;
				shear_force_area.push_back(shearForceArea);						//stores the area occupied by each shear force segment
			}

			//the zeroth shear force area is the same as the bending moments, hence we can initialize the first shear force area
			bendingmoments << vectPair[1].first << " , " << shear_force_area[0] << endl;	//initialize the first bending moment of the graph (the non zero bending moment).
			double moment = shear_force_area[0];		//initialize the bending moment to be the first shear force area
			bending_moment_magnitude.push_back(shear_force_area[0]);			//initialize the second bending moment of the graph and store that magnitude

			for (int i = 1; i < shear_force_area.size() - 1; i++) {				//goes through each shear force area
				double moments_new = shear_force_area[i] + moment;
				bending_moment_magnitude.push_back(moments_new);
				moment = moments_new;
			}

			for (int i = 0; i < bending_moment_magnitude.size(); i++) {											//passes data to the csv file
				bendingmoments << vectPair[i + 1].first << "," << bending_moment_magnitude[i] << endl;			//data points
			}

			for (double i = vectPair[vectPair.size() - 1].first; i <= beamlength; i += 0.2) {		//fills remaning parts of the beam that is out of bounds with zero
				bendingmoments << i << "," << 0 << endl;
			}
	}

};


int main() {
	beamCalc beam;
	beam.setBeam();
	beam.setForces();
	beam.setSupports();
	system("cls");
	beam.initBeam();
	beam.Compute_support_reaction();
	beam.computeShearForce();
	beam.computeBendingMoments();


	system("pause");
}
