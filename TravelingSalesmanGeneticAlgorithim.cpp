// TravelingSalesmanGeneticAlgorithim.cpp 
// Adam Flammino
// Uses a genetic algorithm to find a good solution to the traveling salesman problem

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <string>

struct City {
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
};

// List of cities
class CityList
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
// Holds each tour individual
class Tour
{
private:
	std::vector<City> tour;
	double distance = 0;
	double fitness = 0;
	int size;
public:
	// Constructor, Generates a random tour
	Tour(std::vector<City> cities) 
	{
		tour = cities; // Copies vector of cities
		std::random_shuffle(tour.begin(), tour.end()); // Shuffles tour vector
	}
	// Gets city at i
	City getCity(int i)
	{
		return tour.at(i);
	}
	// Sets city at position i
	void setCity(int i, City c)
	{
		tour.at(i) = c;
	}
	// Gets distance between two cities
	double distanceTwoCities(City city1, City city2) {
		int latDist = abs(city1.getX() - city2.getX());
		int lonDist = abs(city1.getY() - city2.getY());
		return sqrt((latDist * latDist) + (lonDist *lonDist)); // a^2 + b^2 = c^2
	}
	// Gets distance traveled
	double getDistance()
	{
		distance = 0;
		size = tour.size();
		for (int i = 0; i < size - 1; i++)
		{
			distance += distanceTwoCities(tour.at(i), tour.at(i + 1));
		}
		distance += distanceTwoCities(tour.at(0), tour.at(size - 1)); // Return to starting city
		return distance;
	}
	// Gets fitness of tour individual
	double getFitness()
	{
		return 1 / getDistance();
	}
	// Check if a city is on tour
	bool onTour(City c)
	{
		return find(tour.begin(), tour.end(), c) != tour.end();
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

