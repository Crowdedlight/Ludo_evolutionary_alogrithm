#include "populationmanager.h"

PopulationManager::PopulationManager():
    id_counter(0),
    generationID(0),
    pop_size(20),//was 50 before
    specimen_weights_num(10),
    convergingPoint(1000),
    tournementSize(4), //was 5 before
    mutation_probability(2), //was 15 before
    mutation_range(2), //was 0.5% before
    loadPrevGeneration(false),
    generationSaveLocation("../generations/"),
    rd(),
    gen(rd())
{
}

void PopulationManager::init()
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
            std::uniform_real_distribution<> weight(0, 60);
            float value = weight(gen);
            newW[i] = value;
        }

        specimen sm;
        sm.id = id_counter;
        sm.weights = newW;
        sm.wins = 0;
        sm.winrate = 0;
        sm.trained = false;

        population.push_back(sm);

        id_counter++;
    }

    //assign first specimen to game and start training
    int index = findNextNotTrainedSpecimen();
    std::vector<float> weights = getWeightsFromSpecimenIDX(index);
    currentTrainingGame = 0;
    currentTrainingIDX = index;
    std::cout << "init_population()" << std::endl;
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

            //std::cout << "Training generation: " << std::to_string(generationID) << std::endl;

            //start new training for new generation
            nextSpecimen = findNextNotTrainedSpecimen();
            currentTrainingIDX = nextSpecimen;
            std::vector<float> weights = getWeightsFromSpecimenIDX(nextSpecimen);
            emit changeSpecimen(weights);
        }
        else
        {
            //Start training next specimen
            currentTrainingIDX = nextSpecimen;
            std::vector<float> weights = getWeightsFromSpecimenIDX(nextSpecimen);
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
    //std::cout << "Evolving Generation " << generationID << std::endl;

    //Save current generation to yaml file
    saveCurrentGeneration();

    //Determine amount of offsprings => 20%-50% new offspring per generation
    std::uniform_int_distribution<> offspringNum(pop_size/5, pop_size/2);
    int offspring_amount = offspringNum(gen);

    //SELECTION - Tournement style - Returns ID and not index of winners
    std::vector<int> parents = makeTournement(offspring_amount);

    //CROSSOVER - random uniform selection - Returns the new offsprings
    std::vector<specimen> offsprings = makeCrossOver(parents, offspring_amount);

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
    YAML::Node config;

    //sort population with biggest winrate first
    std::sort(population.begin(),
              population.end(),
              [](const specimen& lhs, const specimen& rhs)
    {
        return lhs.winrate > rhs.winrate;
    });

    //top node save vector of winrates for specimen for quick overview
    std::vector<float> winrates_specimens;
    //lambda expression of transform to get vector of only winrates
    std::transform(population.begin(), population.end(), std::back_inserter(winrates_specimens),
                   [](specimen const& x) { return x.winrate; });

    config["win-rate"] = winrates_specimens;


    //output best winrate in generation for progress monitoring
    std::cout << "Generation " << std::to_string(generationID) << " : " << std::to_string(population.front().winrate) << " -- " << std::to_string(population.back().winrate) << std::endl;

    //save every specimen
    for (auto i = 0; i < population.size(); i++)
    {
        config["population"].push_back(population[i]);
    }

    std::string filename = generationSaveLocation + "Generation" + std::to_string(generationID) + ".yaml";
    std::ofstream fout(filename);
    fout << config;

    std::cout << "saved generation " << std::to_string(generationID) << std::endl;
}

std::vector<specimen> PopulationManager::makeMutation(std::vector<specimen> offsprings)
{
    std::uniform_int_distribution<> mutateRoll(0, 100);
    std::uniform_real_distribution<> mutateAmount(-mutation_range, mutation_range);

    //go though each child and each child weight
    for(auto & child : offsprings)
    {
        for (auto & weight : child.weights)
        {
            //check if mutation
            int mutate = mutateRoll(gen);
            if (mutate <= 10)
            {
                //mutate
                float mutation_value = mutateAmount(gen);
                weight += mutation_value;
            }
        }
    }
    return offsprings;
}

std::vector<specimen> PopulationManager::makeCrossOver(std::vector<int> parents, int amount)
{
    //cross every parent with every parent then randomly select childs until enough offspring is produced
    std::vector<specimen> childs;
    for(auto i = 0; i < parents.size(); i++)
    {
        //dad Id
        int dad_id = parents[i];
        for (auto j = 0; j < parents.size(); j++)
        {
            // can't cross myself
            if (dad_id == parents[j])
                continue;

            specimen child = makeChild(dad_id, parents[j]);
            childs.push_back(child);
        }
    }

    //randomly selects childs until offspring vector is full => Not all childs survive
    std::vector<specimen> outChilds(amount);
    for (auto i = 0; i < outChilds.size(); i++)
    {
        std::uniform_int_distribution<> childSelect(0, childs.size()-1);
        int index = childSelect(gen);

        //save child as offspring and remove from possibe list
        outChilds[i] = childs[index];
        childs.erase(childs.begin() + index);
    }

    return outChilds;
}

specimen PopulationManager::makeChild(int dad_id, int mum_id)
{
    std::vector<float> dad_w = getWeightsFromSpecimenID(dad_id);
    std::vector<float> mum_w = getWeightsFromSpecimenID(mum_id);
    std::uniform_int_distribution<> who(0,1);

    std::vector<float> child_w(specimen_weights_num);

    for (auto i = 0; i < child_w.size(); i++)
    {
        //who to take from? 1 == dad, 0 == mum
        int parent = who(gen);
        if (parent == 1)
            child_w[i] = dad_w[i];
        else
            child_w[i] = mum_w[i];
    }

    specimen child;
    child.id = id_counter;
    child.trained = false;
    child.weights = child_w;
    child.wins = 0;
    child.winrate = 0;

    id_counter++;

    return child;
}

std::vector<int> PopulationManager::makeTournement(int offsprings)
{
    //make as many tournements as offsprings +2.
    std::vector<int> winners;
    std::vector<specimen> specimenList = population;

    //always have at least 2 parents
    for (auto i = 0; i < offsprings+2; i++)
    {
        //select randoms for tournement
        std::uniform_int_distribution<> tournement_select(0, specimenList.size()-1);
        std::vector<specimen> tourneList;

        //pick random people to participate in tournement
        for (auto j = 0; j < tournementSize; j++)
        {
            int index = tournement_select(gen);
            tourneList.push_back(specimenList[index]);
        }

        //find winner from tournement
        int winner = -1;
        int winner_idx = -1;
        int winFitValue = -1;
        for (auto n = 0; n < tourneList.size(); n++)
        {
            if (tourneList[n].wins > winFitValue)
            {
                winner = tourneList[n].id;
                winner_idx = n;
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
    {
        population[currentTrainingIDX].wins++;
        int wins = population[currentTrainingIDX].wins;
        population[currentTrainingIDX].winrate = (float)wins/(float) convergingPoint;
    }

    //main update loop
    update();
}

int PopulationManager::findNextNotTrainedSpecimen()
{
    for (auto i = 0; i < population.size(); i++)
    {
        if (population[i].trained == false)
            return i;
    }
    return -1;
}

std::vector<float> PopulationManager::getWeightsFromSpecimenID(int id)
{
    for (auto i = 0; i < population.size(); i++)
    {
        if (population[i].id == id)
            return population[i].weights;
    }
}

std::vector<float> PopulationManager::getWeightsFromSpecimenIDX(int idx)
{
    return population[idx].weights;
}

