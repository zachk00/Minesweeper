#include "Random.h"
#include <random>
#include <ctime>
using namespace std;

mt19937 Random::random(time(0));

int Random::randomInteger(int min, int max)
{
	uniform_int_distribution<int> dist(min, max);

	return dist(random);
	
}
