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
#include <map>
#include <unordered_map>
#include <unordered_set>

// Holds city details

struct City {
private:
	int x;
	int y;
	int id;
public:
	// Constructor with random city coordinatates in a 1000 unit grid
	City(int i) {
		x = rand() % 1000;
		y = rand() % 1000;
		id = i;
	}
	// Constructor with specific city coordinates
	City(int lat, int lon, int i) {
		x = lat;
		y = lon;
		id = i;
	}
	// Get x coordinat;
	int getX() {
		return x;
	}
	// Get y coordinate
	int getY() {
		return y;
	}
	// Get city id
	int getID()
	{
		return id;
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
	// Get list of cities
	std::vector<City> getCityList()
	{
		return cities;
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
	// Gets distance between two cities
	double distanceTwoCities(City city1, City city2) {
		int latDist = abs(city1.getX() - city2.getX());
		int lonDist = abs(city1.getY() - city2.getY());
		return sqrt((latDist * latDist) + (lonDist *lonDist)); // a^2 + b^2 = c^2
	}
public:
	// Constructor
	Tour(std::vector<City> cities)
	{
		createIndividual(cities);
	}
	// Default constructor
	Tour()
	{
		size = 0;
	}
	// Generates a random tour of all cities
	void createIndividual(std::vector<City> cities) 
	{
		tour = cities; // Copies vector of cities
		random_shuffle(tour.begin(), tour.end()); // Shuffles tour vector
	}
	// Gets city at i
	City getCity(int i)
	{
		return tour.at(i);
	}
	// Pushes city onto tour
	void setCity(City c)
	{
		tour.push_back(c);
		size++;
	}
	// Sets city at position i
	void setCity(int i, City c)
	{
		tour.at(i) = c;
	}
	// Gets distance traveled
	double getDistance()
	{
		distance = 0;
		int size = tour.size();
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
	/* 
	 * REPLACED BY city.getID()
	 * 
	// Check if a city is on tour
	bool onTour(City c)
	{
		for (int i = 0; i < size; i++)
		{
			if(tour.at(i).getX() == c.getX()) // Does X coord match?
			{
				if(tour.at(i).getY() == c.getY()) // Does Y coord match?
				{
					return true;
				}
			}
		}
		return false;
	}
	*/
	int getTourSize()
	{
		size = tour.size();
		return size;
	}
};
// Population of tours
class Population
{
private:
	std::vector<Tour> tours; // Holds population of tours
	CityList cl;
	int size = 0;
public:
	// Constructor that seeds random tours of cities
	Population(int popSize, int numCities)
	{
		for (int i = 0; i < numCities; i++) // First, add cities to list
		{
			cl.addCity(City(i));
		}
		for (int i = 0; i < popSize; i++) // Next, create population of tours
		{
			tours.push_back(cl.getCityList());
		}
		size = tours.size();
	}
	// Default constructor
	Population()
	{
		size = 0;
	}
	// Adds a tour
	void addTour(Tour t)
	{
		tours.push_back(t);
		size++;
	}
	// Adds a tour to a specific index
	void addTour(Tour t, int i)
	{
		tours.at(i) = t;
	}
	// Gets tour at i
	Tour getTour(int i)
	{
		return tours.at(i);
	}
	// Finds shortest tour
	Tour getFittest()
	{
		Tour fittest = tours.at(0);
		for (int i = 1; i < size; i++)
		{
			if (fittest.getFitness() < tours.at(i).getFitness())
			{
				fittest = tours.at(i);
			}
		}
		return fittest;
	}
	int getPopSize()
	{
		return size;
	}
};

// Breeds population to increase fitness
class Genetics
{
private:
	const double mutationRate = .01; // Reccomended to be between .005 and .01
	const int tournamentSize = 5;
	bool elitism = true; // If true fittest individual will move to next generation
	std::vector<Tour> tours;
	// Decides which tours to crossover
	Tour tournament(Population p)
	{
		Population tourney;
		int selectionRange = p.getPopSize();
		for(int i = 0; i < tournamentSize; i++) // Grab random tours for tournament
		{
			int randomTour = rand() % selectionRange;
			tourney.addTour(p.getTour(randomTour));
		}
		Tour fit = tourney.getFittest();
		return fit;
	}
	// Tournament that excludes one tour from population
	Tour tournament(Population p, Tour t)
	{
		Population tourney;
		int selectionRange = p.getPopSize() - 1;
		int i = 0;
		int dist = t.getDistance(); // Distance of first parent for comparison
		while (i < tournamentSize)
		{
			int randomTour = rand() % selectionRange;
			if (p.getTour(randomTour).getDistance() != dist) // Ensures second parent isn't first parent
			{
				tourney.addTour(p.getTour(randomTour));
				i++;
			}
		}
		Tour fit = tourney.getFittest();
		return fit;
	}
	// Randomly mutates individuals based on mutation rate
	void mutate(Tour t)
	{
		int tSize = t.getTourSize(); // Tour size
		for(int i = 0; i < tSize; i++)
		{
			if(double(rand() / RAND_MAX) <= mutationRate) // roll for mutation
			{
				int swap = rand() % tSize; // random city to swap
				City c1 = t.getCity(i);
				City c2 = t.getCity(swap);
				t.setCity(swap, c1);
				t.setCity(i, c2);
			}
		}
	}
	// Crosses a set of parents, returns child
	Tour cross(Tour parent1, Tour parent2)
	{
		int tSize = parent1.getTourSize();
		Tour child = Tour();
		std::unordered_set<int> citiesOnTour; // Holds IDs of cities currently on tour, prevents repeats
		/*
		City dummy = parent1.getCity(0);
		for(int i = 0; i < tSize; i++) // need dummy cities in child in order to swap in parent cities
		{
			child.setCity(dummy);
		}
		*/
		int split1 = rand() % (tSize / 2); // How much of tour to take from first half of parent1
		int split2 = rand() % (tSize / 2); // How much of tour to take from second half of parent1
		split2 = tSize - split2; // Where to start taking chromosones from parent1
		for (int i = 0; i < split1; i++) // Add cities from first half of parent1;
		{

			child.setCity(parent1.getCity(i));
			citiesOnTour.insert(parent1.getCity(i).getID());
		}
		for (int i = split2; i < tSize; i++)
		{
			child.setCity(parent1.getCity(i));
			citiesOnTour.insert(parent1.getCity(i).getID());
		}
		// Fill in missing cities using parent 2
		int index = 0;
		while (child.getTourSize() != tSize)
		{
			if (citiesOnTour.count(parent2.getCity(index).getID()) == 0)
			{
				child.setCity(parent2.getCity(index));
			}
			index++;
		}
		/*
		for (int i = split1; i < split2; i++) // Add center region cities from parent2
		{
			child.setCity(parent2.getCity(i));
		}

		for (int i = split2; i < tSize; i++) // Add cities from second half of parent1
		{

			child.setCity((parent1.getCity(i)));
		}
		*/
		return child;
	}
public:
	// Evolves for one generation
	Population evolve(Population p)
	{
		int size = p.getPopSize();
		Population nextGen = Population();
		int eliteOffset = 0;
		if (elitism) // Saves fittest individual
		{
			eliteOffset = 1;
			nextGen.addTour(p.getFittest());
		}
		for (int i = eliteOffset; i < size; i++) // Create next generation
		{
			Tour parent1 = tournament(p);
			Tour parent2 = tournament(p, parent1);
			Tour child = cross(parent1, parent2);
			nextGen.addTour(child);
		}
		
		for (int i = eliteOffset; i < size; i++) // Mutate next generation
		{
			mutate(nextGen.getTour(i));
		}
		return nextGen;
	}
};

int main()
{
	const int numberOfCities = 50; // Sets number of cities
	int populationSize = 25; // Sets size of population
	const int numberGenerations = 100; // Number of generations to evolve
	srand(time(nullptr)); // Needed so random cities are actually random
	Population p = Population(populationSize, numberOfCities); // Creates population
	Tour fittest = p.getFittest();
	Genetics g;
	
	double initialDistance = fittest.getDistance();
	std::cout << "Of the " << populationSize << " initial tours of the " << numberOfCities <<
		" cities the shortest distance was:\t" << initialDistance << std::endl;
	std::cout << "\n\nThe intial route was:\n";
	
	for (int i = 0; i < numberOfCities; i++) // Shows route of cities identified by coordinates
	{
		std::cout << "(" << fittest.getCity(i).getX() << "," << fittest.getCity(i).getY() << ") -> ";
	}
	
	std::cout << "(" << fittest.getCity(0).getX() << "," << fittest.getCity(0).getY() << ")\n"; // Back to first city

	for(int i = 0; i < numberGenerations; i++) // Evolve
	{
		p = g.evolve(p);
	}
	fittest = p.getFittest();
	double improvedDistance = fittest.getDistance();
	double improvement = initialDistance - improvedDistance;
	double percentImprovement = improvement/initialDistance * 100;
	std::cout << "\n\nAfter " << numberGenerations << " generations, the shortest route is:\t" << improvedDistance <<
		".\nThis is an improvement of " << percentImprovement << "%\n";
	std::cout << "\n\nThe evolved route is:\n";
	for (int i = 0; i < numberOfCities; i++) // Shows route of cities identified by coordinates
	{
		std::cout << "(" << fittest.getCity(i).getX() << "," << fittest.getCity(i).getY() << ") -> ";
	}
	std::cout << "(" << fittest.getCity(0).getX() << "," << fittest.getCity(0).getY() << ")\n"; // Back to first city
}

