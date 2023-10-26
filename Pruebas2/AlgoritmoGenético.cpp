#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct AlgoritmoGenético
{
    int id;
    int age;
    double survivalProbability;
    int expectedGenerations;
    double emotionalIntelligence;
    double physicalCondition;
    int upperBodyStrength;
    int lowerBodyStrength;
    double endurance;
};

std::vector<AlgoritmoGenético> InitializePopulation(int populationSize){
    std::vector<AlgoritmoGenético> population;

    std::srand(std::time(0));

    for (int i = 0; i < populationSize; i++) {
        AlgoritmoGenético samurai;
        samurai.id = i;
        samurai.age = std::rand() % 100 + 1;
        samurai.survivalProbability = std::rand() % 101;
        samurai.expectedGenerations = std::rand() % 3 + 1;
        samurai.emotionalIntelligence = std::rand() % 10 + 1;
        samurai.physicalCondition = std::rand() % 50 + 1;
        samurai.upperBodyStrength = std::rand() % 10 + 1;
        samurai.lowerBodyStrength = std::rand() % 10 + 1;
        samurai.endurance = samurai.age +
                            samurai.emotionalIntelligence +
                            samurai.physicalCondition +
                            samurai.upperBodyStrength +
                            samurai.lowerBodyStrength;
        population.push_back(samurai);
    }

    return population;
}

int main() {
    int populationSize = 10;

    std::vector<AlgoritmoGenético> population = InitializePopulation(populationSize);

    for(const AlgoritmoGenético& samurai : population) {
        std::cout << "Samurai " << samurai.id << " - Edad: " << samurai.age << " - SP: " << samurai.survivalProbability << " - EG: " << samurai.expectedGenerations << " - EI: " << samurai.emotionalIntelligence << " - PC" << samurai.physicalCondition << " - U: " << samurai.upperBodyStrength << " - L" << samurai.lowerBodyStrength << " - Resistance: " << samurai.endurance << std::endl;
    }
}