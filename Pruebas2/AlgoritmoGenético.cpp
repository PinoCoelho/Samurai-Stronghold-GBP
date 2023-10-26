#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>


//Atributos de este algoritmo genético
struct AlgoritmoGenético
{
    int id; //id del samurái
    int age; //Edad del samurái
    double survivalProbability; //Probabilidad de supervivencia del samurái
    int expectedGenerations; //Generación que se espera que sobreviva hasta
    double emotionalIntelligence; //Inteligencia emocional del samurái
    double physicalCondition; //Condición física del samurái
    int upperBodyStrength; //Fuerza de la parte superior del cuerpo del samurái
    int lowerBodyStrength; //FUerza de la parte inferior del cuerpo del samurái
    double endurance; //Resistencia del samurái
};
//Vector que guardará a la primera población de samurái
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
//Vector que guardará a los samurái elegidos por selección natural
std::vector<AlgoritmoGenético> Selection(const std::vector<AlgoritmoGenético>& population) {
    std::vector<AlgoritmoGenético> selectedSamurais;

    for(const AlgoritmoGenético& samurai : population) {
        if(samurai.endurance >= 100) {
            selectedSamurais.push_back(samurai);
        }
    }

    return selectedSamurais;
}
//Algoritmo para el cruce de samuráis
AlgoritmoGenético Crossover(const AlgoritmoGenético& parent1, const AlgoritmoGenético& parent2) {
    AlgoritmoGenético child;
     
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
//Función para mutar los genes
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
//Función para mutar los genes de los niños samuráis
void MutateChildren(std::vector<AlgoritmoGenético>& children) {
    std:srand(std::time(0));

    for(AlgoritmoGenético& child: children) {
        Mutacion(child);
    }
}
//Función que almacena el promedio de resistencia de la primera población
int avance1(const std::vector<AlgoritmoGenético>& samurais){
    std::vector<double> resistenciaSamurais;
    for(const AlgoritmoGenético& samurai : samurais) {
        double resistencia = samurai.endurance;
        resistenciaSamurais.push_back(resistencia);
    }
    double promedioResistenciaSamurais = std::accumulate(resistenciaSamurais.begin(), resistenciaSamurais.end(), 0.0) / resistenciaSamurais.size();
    std::cout << "Promedio Resistencia Samurais: " << promedioResistenciaSamurais << std::endl;
}
//Función que almacena el promedio de resistencia de la población de hijos
int avance2(const std::vector<AlgoritmoGenético>& children) {
    std::vector<double> resistenciaNinos;
    for(const AlgoritmoGenético& nino : children) {
        double resistencia = nino.endurance;
        resistenciaNinos.push_back(resistencia);
    }   
    double promedioResistenciaNinos = std::accumulate(resistenciaNinos.begin(), resistenciaNinos.end(), 0.0) / resistenciaNinos.size();
    std::cout << "Promedio Resistencia Niños: " << promedioResistenciaNinos << std::endl;
}
int elegido(const std::vector<AlgoritmoGenético>& children) {
    if(!children.empty()) {
        AlgoritmoGenético ResistenciaMayor = children[0];

        for(const AlgoritmoGenético& nino : children) {
            if(nino.endurance > ResistenciaMayor.endurance) {
                ResistenciaMayor = nino;
            }
        }
        std::cout << "Samurai con mayor resistencia: ID "
        << ResistenciaMayor.id << " - Resistencia: " 
        << ResistenciaMayor.endurance <<std::endl;
    } else {
        std::cout << "La lista de niños está vacía." << std::endl;
    }
}
//Función para generar la nueva generación
int cruce(int x){
    int populationSize = x;
    AlgoritmoGenético parent1;
    AlgoritmoGenético parent2;
    std::vector<AlgoritmoGenético> population = InitializePopulation(populationSize);
    std::vector<AlgoritmoGenético> selectedPopulation = Selection(population);
    std::vector<AlgoritmoGenético> offspring;
    int maxID = -1;

    for (const AlgoritmoGenético& samurai : population) {
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
    avance1(population);
    avance2(offspring);
    elegido(offspring);
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
    cruce(populationSize);
}