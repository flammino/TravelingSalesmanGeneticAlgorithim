// TravelingSalesmanGeneticAlgorithim.cpp 
// Adam Flammino
// Uses a genetic algorithm to solve the traveling salesman problem

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "../../../../../Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Tools/MSVC/14.10.25017/include/vector"

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

struct CityList
{
private:
	std::vector<City> cities; // Holds cities
public:
	// Adds city
	void addCity(City city) {
		cities.push_back(city);
	}
	// Gets city, takes city index as parameter
	City getCity(int i)
	{
		return cities.at(i);
	}
};

int main()
{
	const int numberOfCities = 50; // Sets number of cities
	srand(time(nullptr)); // Needed so random cities are actually random
	City c;
	int totalDist = 0;
	City cities [numberOfCities];
	for (int i = 0; i < numberOfCities; i++) { // creates 50 random cities
		cities[i] = City();
	}
	for (int i = 0; i < numberOfCities; i++) {
		std::cout << "The coordinates for city " << i + 1 << " are\t" << cities[i].getX() << ", " << cities[i].getY() << std::endl;
		if (i < numberOfCities - 1) {
			totalDist += c.distance(cities[i], cities[i + 1]);
		}
	}
	totalDist += c.distance(cities[0], cities[numberOfCities - 1]); // Must return to starting city
	std::cout << "Visiting cities in order, the total distance traveled is " << totalDist << std::endl;
    return 0;
}

