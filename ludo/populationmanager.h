#ifndef POPULATIONMANAGER_H
#define POPULATIONMANAGER_H
#include <QObject>
#include <iostream>
#include <fstream>
#include <random>
#include <yaml-cpp/yaml.h>

struct specimen
{
    int wins;
    float winrate;
    std::vector<float> weights;
    int id;
    bool trained;


    bool operator==(specimen a) const {
          if(a.id==id)
             return true;
          else
             return false;
    }
};




namespace YAML {
template<>
struct convert<specimen> {
  static Node encode(const specimen& rhs) {
    Node node;
    node["id"] = rhs.id;
    node["wins"] = rhs.wins;
    node["win_rate"] = rhs.winrate;
    node["weights"] = rhs.weights;
    return node;
  }

  static bool decode(const Node& node, specimen& rhs) {
    if(!node.IsSequence() || node.size() != 4) {
      return false;
    }

    rhs.id = node["id"].as<int>();
    rhs.wins = node["wins"].as<int>();
    rhs.winrate = node["win_rate"].as<float>();
    rhs.weights = node["weights"].as<std::vector<float>>();
    return true;
  }
};
}


class PopulationManager : public QObject {
    Q_OBJECT
public:
    PopulationManager();
    void init();

private:
    //population parameters
    int id_counter; //to have continous ids for every specimen for identification. OBS: if loading in last generation remember to update ID to next available ID.
    int pop_size;
    int specimen_weights_num;
    std::vector<specimen> population;
    int convergingPoint; //how many games is each specimen trained in
    int tournementSize;
    int mutation_probability;
    float mutation_range;

    //book keeping
    int currentTrainingIDX;
    int currentTrainingGame;
    int generationID;
    bool loadPrevGeneration;
    std::string generationSaveLocation;
    void update(); // Gets run after each game, main

    int findNextNotTrainedSpecimen();
    std::vector<float> getWeightsFromSpecimenIDX(int idx);
    std::vector<float> getWeightsFromSpecimenID(int id);

    //Functions for evolution
    void evolveGeneration();
    std::vector<int> makeTournement(int offsprings); //does tournements and returns winners
    std::vector<specimen> makeCrossOver(std::vector<int> parents, int amount);
    std::vector<specimen> makeMutation(std::vector<specimen> offsprings);
    int findWeakestSpecimen();
    specimen makeChild(int dad_id, int mum_id);


    //Saving out generations and loading them
    void loadLastGeneration();
    void saveCurrentGeneration();

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
