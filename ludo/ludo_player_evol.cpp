#include "ludo_player_evol.h"

ludo_player_evol::ludo_player_evol():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0),
    weights(10)
{
    //test - Set by myself
//    weights[0] = 1;
//    weights[1] = 2;
//    weights[2] = 1.6;
//    weights[3] = 50;
//    weights[4] = 1.5;
//    weights[5] = 100;
//    weights[6] = 0.4;
//    weights[7] = 3.4;

    //pop50_100games_high_birth_10mutation_87winrate - New State space => random => 72% win => better_player => 58% (even with 40000 games)
    weights[0] = 0.9876136;
    weights[1] = 2.182484;
    weights[2] = 0.4729599;
    weights[3] = 16.68211;
    weights[4] = 8.582215;
    weights[5] = 18.25736;
    weights[6] = 1.579915;
    weights[7] = 2.771717;
    weights[8] = 2.537776;
    weights[9] = 9.984499;


    //pop50_250games_high_birth_5mutation_81winrate
//    weights[0] = 0.3336691;
//    weights[1] = -0.6562978;
//    weights[2] = 1.713695;
//    weights[3] = 17.35723;
//    weights[4] = 4.465443;
//    weights[5] = 7.512275;
//    weights[6] = 1.168676;
//    weights[7] = -0.8972499;
}

int ludo_player_evol::make_decision(){
    //init empty moves vector, all at 0
    std::vector<float> weighted_moves(4, 0);

    // loop though pieces
    for (int i = 0; i < 4; i++)
    {
        int currPos = pos_start_of_turn[i];
        int newPos = currPos + dice_roll;

        // if star update end position to actual moved position
        if (isStar(newPos) != 0)
            newPos += isStar(newPos);

        // if piece is in goal don't bother checking and set value to absolutly minimum
        if (currPos == 99)
        {
            weighted_moves[i] = -5000;
            continue;
        }

        // get parameters and save as weighted move
        weighted_moves[i] += weights[0] * getMovedDistance(currPos, newPos);
        weighted_moves[i] += weights[1] * getSafetyGain(newPos, i);
        weighted_moves[i] += weights[2] * getSafetyLoss(currPos, i);
        weighted_moves[i] += weights[3] * getEscapedHome(currPos);
        weighted_moves[i] += weights[4] * getKilledPlayer(newPos);
        weighted_moves[i] += weights[5] * getKilledByPlayer(newPos);
        weighted_moves[i] += weights[6] * getRiskOfKill(newPos, i);
        weighted_moves[i] += weights[7] * getCanMoveToGoal(newPos);
        weighted_moves[i] += weights[8] * getCanMoveToFinalGoal(newPos);
        weighted_moves[i] += weights[9] * getMoveInWinLine(currPos, newPos);
    }

    //select the index of the biggest value as the best move
    auto it = std::max_element(weighted_moves.begin(), weighted_moves.end());
    int chosenPiece = it - weighted_moves.begin();

    return chosenPiece; //dereference from iterator
}

int ludo_player_evol::getCanMoveToFinalGoal(int newPos)
{
    if (newPos == 56)
        return 1;
    else return 0;
}

int ludo_player_evol::getMoveInWinLine(int currPos, int newPos)
{
    if (currPos > 50 && newPos != 56 && currPos < 99)
        return -1;
    else
        return 0;
}

void ludo_player_evol::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player_evol::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}

int ludo_player_evol::getRiskOfKill(int pos, int piece)
{
    //check new pos and see if there is enemy players within 6 behind you. Or if standing on star if any is within the last stars range
    int amountEnemiesInRange = 0;
    for (auto i = pos; i != pos-7; i--)
    {
        // if wrap around and i is less than 0, then change to correct index by 51 offset. (51 being first step in goal stretch)
        int chPos = i;
        if (i < 0)
            chPos = 51+i;
        //add to sum of enemies in range to kill you
        amountEnemiesInRange += isOccupied(chPos);
    }

    // if star check past star
    if (isStar(pos) > 0)
    {
        //if on star, last star must be old pos + dice roll
        int lastStar = pos_start_of_turn[piece] + dice_roll;

        //check likewise back from that star
        for (auto i = lastStar; i != lastStar-7; i--)
        {
            // if wrap around and i is less than 0, then change to correct index by 51 offset. (51 being first step in goal stretch)
            int chPos = i;
            if (i < 0)
                chPos = 51+i;
            //add to sum of enemies in range to kill you
            amountEnemiesInRange += isOccupied(chPos);
        }
    }

    // if currently safe, disregard this function => globe, friendlyPlayer, goal stretch
    if (getAnyFriendlyOnPos(pos, piece) || isGlobe(pos) || pos > 50)
        return 0;
    else
        return -amountEnemiesInRange; //make negative as it is a risk and not a gain. Returns the number of pieces that can kill you
}

int ludo_player_evol::getCanMoveToGoal(int pos)
{
    // first step in goal stretch is pos 51
    if (pos > 50)
        return 1;
    else
        return 0;
}

// TODO both getKilled and SaftyLoss is negative if it happens and returns -1. Instead of having negative weights on that. Is that good or bad?
int ludo_player_evol::getKilledByPlayer(int pos)
{
    //return -1 if moving into a suicide
    if (isGlobe(pos) && isOccupied(pos) > 0)
        return -1;
    else if (isOccupied(pos) > 1)
        return -1;
    else return 0;
}

int ludo_player_evol::getKilledPlayer(int pos)
{
    // if newpos has a single player we kill it
    if (isOccupied(pos) == 1)
        return 1;
    else
        return 0;
}

int ludo_player_evol::getEscapedHome(int pos)
{
    if (pos == -1 && dice_roll == 6)
        return 1;
    else
        return 0;
}

int ludo_player_evol::getSafetyLoss(int pos, int piece)
{
    //check is current position was safe, in which case we loose safety. Should not care about if new pos is safe as the safetyGain should talk for that part
    //if we leave safty this parameter subtracts value, if we weren't safe it doesn't get taken into account as it provides no information
    if (isGlobe(pos))
        return -1;
    else if (getAnyFriendlyOnPos(pos, piece))
        return -1;
    else
        return 0;
}

int ludo_player_evol::getSafetyGain(int pos, int piece)
{
    //TODO should only be safety if no suicide aswell? Or does the other parameter take care of that?
    if (isGlobe(pos))
        return 1;
    else if (getAnyFriendlyOnPos(pos, piece))
        return 1;
    else
        return 0;
}

// See if there is friendly players on this pos, not counting yourself.
bool ludo_player_evol::getAnyFriendlyOnPos(int pos, int piece)
{
    for (auto i = 0; i < 4; i++)
    {
        //skip yourself
        if (i == piece)
            continue;

        if (pos_start_of_turn[i] == pos)
            return true;
    }
    //nope
    return false;
}

int ludo_player_evol::getMovedDistance(int oldPos, int newPos)
{
    //if piece is in home and dice is not a 6, movement is set to zero
    if (oldPos == -1)
    {
        // if in home and dice is 6, movement is set to one
        if (dice_roll == 6)
            return 1;
        else
            return 0;
    }
    // else just return dice roll
    else
        return abs(newPos-oldPos);
}

void ludo_player_evol::changeSpecimen(std::vector<float> newSpecimen)
{
    //replace weights and emit signal that it is changed
    //populationManagers responsibility to know what specimen is testing currently
    weights.clear();
    weights = newSpecimen;
    emit SpecimenChanged();
}

int ludo_player_evol::isStar(int index){
    if(index == 5  ||
       index == 18 ||
       index == 31 ||
       index == 44){
        return 6;
    } else if(index == 11 ||
              index == 24 ||
              index == 37 ||
              index == 50){
        return 7;
    }
    return 0;
}

int ludo_player_evol::isOccupied(int index){ //returns number of people of another color
    int number_of_people = 0;

    if(index != 99){
        for(auto i = 4; i < pos_start_of_turn.size(); i++){
                if(pos_start_of_turn[i] == index){
                    ++number_of_people;
                }
            }
        }
    return number_of_people;
}

bool ludo_player_evol::isGlobe(int index){
    if(index < 52){     //check only the indexes on the board, not in the home streak
        if(index % 13 == 0 || (index - 8) % 13 == 0 || isOccupied(index) > 1){  //if more people of the same team stand on the same spot it counts as globe
            return true;
        }
    }
    return false;
}
