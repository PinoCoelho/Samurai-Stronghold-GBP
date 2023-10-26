#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "gnuplot-iostream.h"


int main(){
    Gnuplot gp;
    // Datos de ejemplo
    std::vector<int> generaciones = {1, 2, 3, 4, 5};
    std::vector<double> mejorAptitud = {90, 92, 95, 97, 99};
    std::vector<double> aptitudMedia = {80, 85, 90, 93, 96};

    gp << "set title 'Avance Generacional'" << std::endl;
    gp << "set xlabel 'Generación'" << std::endl;
    gp << "set ylabel 'Aptitud'" << std::endl;
    gp << "plot '-' with lines title 'Mejor Aptitud', '-' with lines title 'Aptitud Media'" << std::endl;
    gp.send1d(std::make_tuple(generaciones, mejorAptitud));
    gp.send1d(std::make_tuple(generaciones, aptitudMedia));

    // Espera hasta que el usuario cierre la ventana del gráfico
    std::cout << "Presiona enter para salir..." << std::endl;
    std::cin.get();

    return 0;
}


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

std::vector<AlgoritmoGenético> Selection(const std::vector<AlgoritmoGenético>& population) {
    std::vector<AlgoritmoGenético> selectedSamurais;

    for(const AlgoritmoGenético& samurai : population) {
        if(samurai.endurance >= 100) {
            selectedSamurais.push_back(samurai);
        }
    }

    return selectedSamurais;
}

AlgoritmoGenético Crossover(const AlgoritmoGenético& parent1, const AlgoritmoGenético& parent2) {
    AlgoritmoGenético child;
    
    child.id = 
    child.age = (parent1.age + parent2.age) / 2;
    child.survivalProbability = (parent1.survivalProbability + parent2.survivalProbability) / 2;
    child.expectedGenerations = (parent1.expectedGenerations + parent2.expectedGenerations) - 1;
    child.emotionalIntelligence = (parent1.emotionalIntelligence + parent2.emotionalIntelligence) / 2;
    child.physicalCondition = (parent1.physicalCondition + parent2.physicalCondition) / 2;
    child.upperBodyStrength = (parent1.upperBodyStrength + parent2.upperBodyStrength) / 2;
    child.lowerBodyStrength = (parent1.lowerBodyStrength + parent2.lowerBodyStrength) / 2;
    child.endurance = child.age +
                      child.emotionalIntelligence +
                      child.physicalCondition +
                      child.upperBodyStrength +
                      child.lowerBodyStrength;
    
    return child;
}

void Mutacion(AlgoritmoGenético& samurai) {
    double mutationRate = 0.1;

    double random = static_cast<double>(std::rand()) / RAND_MAX;

    if(random < mutationRate) {
        samurai.age += std::rand() % 10 - 5;
        samurai.emotionalIntelligence += (std::rand() % 3) - 1;
        samurai.physicalCondition += std::rand() % 10 - 5;
        samurai.upperBodyStrength += (std::rand() % 3) - 1;
        samurai.lowerBodyStrength += (std::rand() % 3) - 1;
    }
}

void MutateChildren(std::vector<AlgoritmoGenético>& children) {
    std:srand(std::time(0));

    for(AlgoritmoGenético& child: children) {
        Mutacion(child);
    }
}

int cruce(){
    AlgoritmoGenético parent1;
    AlgoritmoGenético parent2;
    int populationSize = 5;
    std::vector<AlgoritmoGenético> population = InitializePopulation(populationSize);
    std::vector<AlgoritmoGenético> selectedPopulation = Selection(population);
    std::vector<AlgoritmoGenético> offspring;
    int maxID = -1;

    for (const AlgoritmoGenético& samurai : selectedPopulation) {
        if(samurai.id > maxID) {
            maxID = samurai.id;
        }
    }
    if (selectedPopulation.size() >= 2) {
        for (size_t i = 0; i < selectedPopulation.size(); i += 2) {
            AlgoritmoGenético parent1 = selectedPopulation[i];
            AlgoritmoGenético parent2 = selectedPopulation[i + 1];

            AlgoritmoGenético child = Crossover(parent1, parent2);

            child.id = maxID + 1;
            maxID++;

            offspring.push_back(child);
        }
    } else {
        std::cout << "No hay suficientes samurais seleccionados para el cruzamiento. " << std::endl;
    }
    for(const AlgoritmoGenético& child : offspring) {
        std::cout << "Descendiente - ID: " 
        << child.id << " - Edad: " 
        << child.age << " - SP: " 
        << child.survivalProbability << " - EG: " 
        << child.expectedGenerations << " - EI: " 
        << child.emotionalIntelligence << " - PC: "
        << child.physicalCondition << " - U: " 
        << child.upperBodyStrength << " - L: " 
        << child.lowerBodyStrength << " - Resistance: " 
        << child.endurance << std::endl;
    }
    MutateChildren(offspring);
    for(const AlgoritmoGenético& child : offspring) {
        std::cout << "Descendiente - ID: " 
        << child.id << " - Edad: " 
        << child.age << " - SP: " 
        << child.survivalProbability << " - EG: " 
        << child.expectedGenerations << " - EI: " 
        << child.emotionalIntelligence << " - PC: "
        << child.physicalCondition << " - U: " 
        << child.upperBodyStrength << " - L: " 
        << child.lowerBodyStrength << " - Resistance: " 
        << child.endurance << std::endl;
    }
}

int main() {
    int populationSize = 20;

    std::vector<AlgoritmoGenético> population = InitializePopulation(populationSize);

    for(const AlgoritmoGenético& samurai : population) {
        std::cout << "Samurai " 
        << samurai.id << " - Edad: " 
        << samurai.age << " - SP: " 
        << samurai.survivalProbability << " - EG: " 
        << samurai.expectedGenerations << " - EI: " 
        << samurai.emotionalIntelligence << " - PC: " 
        << samurai.physicalCondition << " - U: " 
        << samurai.upperBodyStrength << " - L: " 
        << samurai.lowerBodyStrength << " - Resistance: " 
        << samurai.endurance << std::endl;
    }
    std::vector<AlgoritmoGenético> selectedPopulation = Selection(population);
    std::cout << "Samurais seleccionados:" << std::endl;
    for (const AlgoritmoGenético& samurai : selectedPopulation) {
        std::cout << "Samurai " 
        << samurai.id << " - Edad: " 
        << samurai.age << " - SP: " 
        << samurai.survivalProbability << " - EG: " 
        << samurai.expectedGenerations << " - EI: " 
        << samurai.emotionalIntelligence << " - PC: " 
        << samurai.physicalCondition << " - U: " 
        << samurai.upperBodyStrength << " - L: " 
        << samurai.lowerBodyStrength << " - Resistance: " 
        << samurai.endurance << std::endl;
    }
    cruce();
}