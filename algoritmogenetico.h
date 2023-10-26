#ifndef ALGORITMO_GENETICO_H
#define ALGORITMO_GENETICO_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <iostream>
//#include "algoritmogenetico.h"

// Atributos de este algoritmo genético
struct AlgoritmoGenético
{
    int id; // id del samurái
    int age; // Edad del samurái
    double survivalProbability; // Probabilidad de supervivencia del samurái
    int expectedGenerations; // Generación que se espera que sobreviva hasta
    double emotionalIntelligence; // Inteligencia emocional del samurái
    double physicalCondition; // Condición física del samurái
    int upperBodyStrength; // Fuerza de la parte superior del cuerpo del samurái
    int lowerBodyStrength; // Fuerza de la parte inferior del cuerpo del samurái
    double endurance; // Resistencia del samurái
};

// Vector que guardará a la primera población de samuráis
std::vector<AlgoritmoGenético> InitializePopulation(int populationSize);

// Vector que guardará a los samuráis elegidos por selección natural
std::vector<AlgoritmoGenético> Selection(const std::vector<AlgoritmoGenético>& population);

// Algoritmo para el cruce de samuráis
AlgoritmoGenético Crossover(const AlgoritmoGenético& parent1, const AlgoritmoGenético& parent2);

// Función para mutar los genes
void Mutacion(AlgoritmoGenético& samurai);

// Función para mutar los genes de los niños samuráis
void MutateChildren(std::vector<AlgoritmoGenético>& children);

// Función que almacena el promedio de resistencia de la primera población
double avance1(const std::vector<AlgoritmoGenético>& samurais);

// Función que almacena el promedio de resistencia de la población de hijos
double avance2(const std::vector<AlgoritmoGenético>& children);

// Función que muestra al samurái con mayor resistencia entre los niños
void elegido(const std::vector<AlgoritmoGenético>& children);

// Función para generar la nueva generación
void cruce(int x);

int generador();

#endif // ALGORITMO_GENETICO_H
