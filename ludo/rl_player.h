#ifndef RL_PLAYER_H
#define RL_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"
#include <Eigen/Dense>
#include <fstream>
#include <string>
#include "game.h"
#include <iomanip>

#define STATE_HOME 0
#define STATE_OWN_GLOBE 1
#define STATE_ENEMY_GLOBE 2
#define STATE_OTHER_GLOBE 3
#define STATE_STAR 4
#define STATE_HOME_RUN 5
#define STATE_SAFE_ZONE 6
#define STATE_NORMAL 7
#define STATE_GOAL 8

#define ACTION_PAWN_STUCK 0 //"Action 0: Pawn is stuck",
#define ACTION_MOVE_TO_START 1 //"Action 1: Move to start",
#define ACTION_MOVE_TO_OTHER_GLOBE 2//"Action 2: Move to globe (other)",
#define ACTION_MOVE_TO_OPPONENT_GLOBE 3 //"Action 3: Movet to globe (opponent)",
#define ACTION_MOVE_TO_STAR 4 //"Action 4: Move to star",
#define ACTION_MOVE_TO_STAR_AND_GOAL 5 //"Action 5: Move to star and then goal",
#define ACTION_SAFE_ZONE 6 //"Action 6: Move to buddy pawn (safe zone)",
#define ACTION_ELIMINATE_OPPONENT 7 //"Action 7: Eliminate opponent pawn",
#define ACTION_VERY_HIGH_DANGER_MOVE 8 //"Action 8: Normal move (very high danger - 3+)",
#define ACTION_HIGH_DANGER_MOVE 9 //"Action 9: Normal move (high danger - 2)",
#define ACTION_DANGER_MOVE 10 //"Action 10: Normal move (danger - 1)",
#define ACTION_NORMAL_MOVE 11 //"Action 11: Normal move (no danger)",
#define ACTION_HOME_RUN 12 //"Action 12: Move to home run",
#define ACTION_LOOSE_OWN_PAWN 13 //"Action 13: Loose own pawn",
#define ACTION_MOVE_TO_GOAL 14 //"Action 14: Move to Goal"


// #define NUM_COLS_Q_TABLE 4
// #define NUM_ROWS_Q_TABLE 4

using namespace std;
using namespace Eigen;

struct PrintLists {
    vector<string> states;
    vector<string> actions_strings;
};

struct Pawn{
    int pawn; // own pawns, either 0,1,2,3
    int end_pos;
    int cur_pos;
    vector<int> possible_actions;
    int selected_action;
    int next_state;
    int current_state;
    void init_pawn(int pawn_){
        pawn = pawn_;
        end_pos = 0;
        possible_actions = {};
        next_state = 0;
        current_state = 0;
    }
};

class rl_player : public QObject {
    Q_OBJECT
private:
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    std::random_device rd;
    std::mt19937 gen;
    bool isGlobe(int index);
    int isOccupied(int index); //see if it is occupied and return the piece number

    int make_decision();
    vector<int> player_actions();
    int get_state(int player, bool end_pos);
    bool is_pawn_safe(int position, int player);
    bool can_reach_opponent(int position);
    int danger_level_for_pawn(int position);
    Pawn select_action();
    Pawn select_action_highest_q_value(vector<Pawn> &pawns_that_can_move);
    Pawn select_action_random(vector<Pawn> pawns_that_can_move);
    void update_q_table(Pawn selected_pawn);
    double get_immediate_reward(Pawn selected_pawn);
    void load_q_table(MatrixXd &q_table);
    void save_q_table_excel();
    int get_probability_for_action_selections(int games_played);
    double get_new_alfa(int games_played);
    ofstream win_rate_file;
    ofstream q_table_elements;
    double win_counter;
    bool load_table = false;
    vector<string> states;
    vector<string> actions_strings;
    MatrixXd Q;
    vector<Pawn> pawns;
    PrintLists printlists;
    game g;
    // rate of decay for random selection
    const double RATE_OF_DECREASE = 0.000001;
    double EPSILON = 0; // probability for random choice
    //start with 100% random (100-MIN epsilon)
    const double INITIAL_EPSILON = 90; //
    const double MIN_EPSILON = 10;
    const double INITIAL_ALFA = 0.5;
    const double ALFA = 0.01  ; // learning rate
    const double ALFA_DECREASE_RATE = 0.01;
    const double GAMMA = 0.3; // discount factor
    int last_action;
    int last_state;
    int NUM_COLS_Q_TABLE;
    int NUM_ROWS_Q_TABLE;
    double exploration = 0;
    double explotation = 0;
    double counter = 0;

public:
    rl_player();
    void print_q_table();
    void reset(vector<double> result);
    void save_q_table();
    int games_played;

signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // RL_PLAYER_H
