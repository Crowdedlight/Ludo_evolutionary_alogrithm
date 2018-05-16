#ifndef LUDO_PLAYER_EVOL_H
#define LUDO_PLAYER_EVOL_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"

class ludo_player_evol : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    std::vector<float> weights;
    int dice_roll;

    //copy paste to check new moves positions
    int isStar(int index);
    bool isGlobe(int index);
    int isOccupied(int index); //see if it is occupied and return the piece number

    //Own functions
    int make_decision();

    //Parameter functions
    bool getAnyFriendlyOnPos(int pos, int piece);
    int getSafetyGain(int pos, int piece);
    int getSafetyLoss(int pos, int piece);
    int getMovedDistance(int oldPos, int newPos);
    int getEscapedHome(int pos);
    int getKilledPlayer(int pos);
    int getKilledByPlayer(int pos);
    int getCanMoveToGoal(int pos);
    int getRiskOfKill(int pos, int piece);
    int getCanMoveToFinalGoal(int newPos);
    int getMoveInWinLine(int currPos, int newPos);

public:
    ludo_player_evol();
signals:
    void select_piece(int);
    void turn_complete(bool);
    void SpecimenChanged();
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
    void changeSpecimen(std::vector<float> newSpecimen);
};

#endif // LUDO_PLAYER_EVOL_H
