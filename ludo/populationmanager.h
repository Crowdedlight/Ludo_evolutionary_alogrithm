#ifndef POPULATIONMANAGER_H
#define POPULATIONMANAGER_H
#include <QObject>
#include <iostream>
#include <random>

struct specimen
{
    int wins;
    std::vector<float> weights;
    int id;
    bool trained;
    //no constructor as weights size is set as var
};

class PopulationManager : public QObject {
    Q_OBJECT
public:
    PopulationManager();

private:
    //population parameters
    int id_counter; //to have continous ids for every specimen for identification. OBS: if loading in last generation remember to update ID to next available ID.
    int pop_size;
    int specimen_weights_num;
    std::vector<specimen> population;
    int convergingPoint; //how many games is each specimen trained in
    float mutation_probability;
    float mutation_range;

    //book keeping
    int currentTrainingIDX;
    int currentTrainingGame;
    void update(); // Gets run after each game, main

    int findNextNotTrainedSpecimen();
    std::vector<float> getWeightsFromSpecimenID(int idx);

    //Functions for evolution
    void evolveGeneration();

    //Saving out generations and loading them


    // misc
    std::random_device rd;
    std::mt19937 gen;


signals:
    void next_game(bool);
    void changeSpecimen(std::vector<float>);
public slots:
    void get_winner(int color);
    void specimenChanged();
};

#endif // POPULATIONMANAGER_H
