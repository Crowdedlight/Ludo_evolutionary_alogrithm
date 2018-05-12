#include "populationmanager.h"

PopulationManager::PopulationManager():
    id_counter(0),
    generationID(0),
    pop_size(20),
    specimen_weights_num(8),
    convergingPoint(500),
    tournementSize(6),
    mutation_probability(0.1),
    mutation_range(0.5),
    loadLastGeneration(false),
    generationSaveLocation("../../generations/"),
    rd(),
    gen(rd())
{
    //Determine to load or make new population randomly
    // TODO LOAD OR GENERATE

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
            evolveGeneration();
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
    saveCurrentGeneration();

    //SELECTION - Tournement style - Returns ID and not index of winners
    std::vector<int> parents = makeTournement();

    //CROSSOVER - random uniform selection - Returns the new offsprings
    std::vector<specimen> offsprings = makeCrossOver(parents);

    //MUTATION - random mutations on new offsprings
    offsprings = makeMutation(offsprings);

    //REPLACE WEAKEST WITH OFFSPRING
    for(auto os : offsprings)
    {
        //find index of weakest
        int weakest_idx = findWeakestSpecimen();

        //replace weakest with offspring
        population[weakest_idx] = os;
    }

    generationID++;

}

int PopulationManager::findWeakestSpecimen()
{
    //preselect with first element for if-sentence
    int value = population[0].wins;
    int idx = 0;
    for (auto i = 1; i < population.size(); i++)
    {
        if (population[i].wins < value)
            idx = i;
    }

    return idx;
}

void PopulationManager::saveCurrentGeneration()
{
    //Save to yaml file with current genereationID
}

std::vector<specimen> PopulationManager::makeMutation(std::vector<specimen> offsprings)
{

}

std::vector<specimen> PopulationManager::makeCrossOver(std::vector<int> parents)
{

}

std::vector<int> PopulationManager::makeTournement()
{
    //steady state crossover, 10%-20% new offspring per generation
    std::uniform_int_distribution<> offspringNum(pop_size/10, pop_size/5);
    int offsprings = offspringNum(gen);

    //make as many tournements as offsprings
    std::vector<int> winners;
    std::vector<specimen> specimenList = population;

    for (auto i = 0; i < offsprings; i++)
    {
        //select randoms for tournement
        std::uniform_int_distribution<> tournement_select(0, specimenList.size());
        std::vector<specimen> tourneList;

        //pick random people to participate in tournement
        for (auto i = 0; i < tournementSize; i++)
        {
            int index = tournement_select(gen);
            tourneList.push_back(specimenList[index]);
        }

        //find winner from tournement
        int winner = -1;
        int winner_idx = -1;
        int winFitValue = -1;
        for (auto i = 0; i < tourneList.size(); i++)
        {
            if (tourneList[i].wins > winFitValue)
            {
                winner = tourneList[i].id;
                winner_idx = i;
            }
        }

        //save winner
        winners.push_back(winner);
        //remove winner from  specimenlist
        specimenList.erase(specimenList.begin() + winner_idx);
    }

    return winners;
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

