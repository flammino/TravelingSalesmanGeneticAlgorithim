// TravelingSalesmanGeneticAlgorithim.cpp 
// Adam Flammino 6/28
// Uses a genetic algorithm to solve the traveling salesman problem

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>

class City {
private:
	int x;
	int y;
public:
	// Constructor with random city coordinatates in a 1000 unit grid
	City() {
		x = rand() % 1000;
		y = rand() % 1000;
	}
	// Constructor with specific city coordinates
	City(int lat, int lon) {
		x = lat;
		y = lon;
	}
	// Get x coordinat;
	int getX() {
		return x;
	}
	// Get y coordinate
	int getY() {
		return y;
	}
	// Distance between cities
	double distance(City city1, City city2) {
		int latDist = abs(city1.getX() - city2.getX());
		int lonDist = abs(city1.getY() - city2.getY());
		return sqrt((latDist * latDist) + (lonDist *lonDist)); // a^2 + b^2 = c^2
	}
};


int main()
{
	int numberOfCities;
	srand(time(NULL)); // Needed so random cities are actually random
	City c;
	int totalDist = 0;
	std::vector<City> cities;
	for (int i = 0; i < 50; i++) { // creates 50 random cities
		cities.push_back(City());
	}
	numberOfCities = cities.size();
	for (int i = 0; i < numberOfCities; i++) {
		std::cout << "The coordinates for city " << i + 1 << " are\t" << cities[i].getX() << ", " << cities[i].getY() << std::endl;
		if (i < numberOfCities - 1) {
			totalDist += c.distance(cities[i], cities[i + 1]);
		}
	}
	totalDist += c.distance(cities[0], cities[numberOfCities]); // Must return to starting city
	std::cout << "Visiting cities in order, the total distance traveled is " << totalDist << std::endl;
    return 0;
}

