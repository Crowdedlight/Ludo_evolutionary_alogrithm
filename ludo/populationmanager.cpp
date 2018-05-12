#include "populationmanager.h"

PopulationManager::PopulationManager():
    id_counter(0),
    pop_size(20),
    specimen_weights_num(8),
    convergingPoint(500),
    rd(),
    gen(rd())
{
    //Determine to load or make new popualation randomly

    // Randomly generate new specimens
    for (auto i = 0; i < pop_size; i++)
    {
        //generate random weights from normal distribution
        std::vector<float> newW(specimen_weights_num, 0);
        for (auto i = 0; i < specimen_weights_num; i++)
        {
            std::uniform_real_distribution<> weight(0, 20);
            float value = weight(gen);
            newW[i] = value;
        }

        specimen sm;
        sm.id = id_counter;
        sm.weights = newW;
        sm.wins = 0;
        sm.trained = false;

        population.push_back(sm);

        id_counter++;
    }

    //assign first specimen to game and start training
    int index = findNextNotTrainedSpecimen();
    std::vector<float> weights = getWeightsFromSpecimenID(index);
    currentTrainingGame = 0;
    currentTrainingIDX = index;
    emit changeSpecimen(weights);
}

//Gets run after each game
void PopulationManager::update()
{
    // check if reached convergence and need to change specimen or generation
    if (currentTrainingGame == convergingPoint)
    {
        //reset training counters and set specimen to trained
        population[currentTrainingIDX].trained = true;
        currentTrainingGame = 0;

        //get next specimen for training, if none, do generationEvolve
        int nextSpecimen = findNextNotTrainedSpecimen();

        if (nextSpecimen == -1)
        {
            //evolve generation
        }
        else
        {
            //Start training next specimen
            currentTrainingIDX = nextSpecimen;
            std::vector<float> weights = getWeightsFromSpecimenID(nextSpecimen);
            emit changeSpecimen(weights);
        }
    }
    else
    {
        //keep training current specimen
        currentTrainingGame++;
        emit next_game(true);
    }
}

void PopulationManager::evolveGeneration()
{
    //Save current generation to yaml file

    //TODO...... evovle....All trained and need to make offspring and mutations from some strategy
}

void PopulationManager::specimenChanged()
{
    emit next_game(true);
}

void PopulationManager::get_winner(int color)
{
    //if winner is player1, increment win counter
    if (color == 0)
        population[currentTrainingIDX].wins++;

    //main update loop
    update();
}

int PopulationManager::findNextNotTrainedSpecimen()
{
    for (auto i = 0; i < population.size(); i++)
    {
        if (population[id].trained == false)
            return i;
    }
    return -1;
}

std::vector<float> PopulationManager::getWeightsFromSpecimenID(int idx)
{
    return population[idx].weights;
}

