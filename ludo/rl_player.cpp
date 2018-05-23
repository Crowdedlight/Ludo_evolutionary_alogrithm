#include "rl_player.h"

rl_player::rl_player():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
    printlists.states = {
        "State 0: Home",
        "State 1: Own Globe",
        "State 2: Enemy Globe",
        "State 3: Other Globe",
        "State 4: Star",
        "State 5: Home Run",
        "State 6: Safe Zone",
        "State 7: Normal",
        "State 8: Goal"
    };

    printlists.actions_strings = {
        "Action 0: Pawn is stuck",
        "Action 1: Move to start",
        "Action 2: Move to globe (other)",
        "Action 3: Movet to globe (opponent)",
        "Action 4: Move to star",
        "Action 5: Move to star and then goal",
        "Action 6: Move to buddy pawn (safe zone)",
        "Action 7: Eliminate opponent pawn",
        "Action 8: Normal move (very high danger - 3+)",
        "Action 9: Normal move (high danger - 2)",
        "Action 10: Normal move (danger - 1)",
        "Action 11: Normal move (no danger)",
        "Action 12: Move to home run",
        "Action 13: Loose own pawn",
        "Action 14: Move to Goal"
    };
    int NUM_ROWS_Q_TABLE = printlists.states.size();
    int NUM_COLS_Q_TABLE = printlists.actions_strings.size();
//    cout << "Q-Table initialized with size: [" << NUM_ROWS_Q_TABLE << "," <<
//                                                NUM_COLS_Q_TABLE << "]" << endl;

    Q = MatrixXd::Zero(NUM_ROWS_Q_TABLE,NUM_COLS_Q_TABLE);
    if(load_table)
      load_q_table(Q);

    //Q(0,2) = -2000;
    // for(int i = 0; i < NUM_ROWS_Q_TABLE; i++)
    //   for(int j = 0; j < NUM_COLS_Q_TABLE; j++){
    //     int random_num = (rand() % 20)-10; //
    //     Q(i,j) = random_num;
    //   }

    //cout << Q << endl;

    Pawn pawn;
    pawns.push_back(pawn);
    pawns.push_back(pawn);
    pawns.push_back(pawn);
    pawns.push_back(pawn);
    pawns[0].init_pawn(0);
    pawns[1].init_pawn(1);
    pawns[2].init_pawn(2);
    pawns[3].init_pawn(3);

    pawns[0].next_state = 0;
    pawns[1].next_state = 0;
    pawns[2].next_state = 0;
    pawns[3].next_state = 0;

    last_action = 0;
    last_state = 0;
    games_played = 0;
    EPSILON = get_probability_for_action_selections(games_played);
    win_rate_file.open(("/home/per/win_rate.txt"));
    q_table_elements.open(("/home/per/q_table_elements.txt"));

}

// method used to find current state and next state
int rl_player::get_state(int player, bool end_pos){
    int pos;
    if (end_pos)
        pos = pos_start_of_turn[player] + dice_roll;
    else
        pos = pos_start_of_turn[player];

    int state = -1;

    switch(pos){
      case -1:
        state = 0;
        break;
      case 0:
        state = 1;
        break;
      case 13: case 26: case 39:
        state = 2;
        break;
      case 8: case 21: case 34: case 47:
        state = 3;
        break;
      case 5: case 18: case 31: case 44: case 11: case 24: case 37: case 50:
        state = 4;
        break;
      case 51: case 52: case 53: case 54: case 55: case 57: case 58: case 59:
      case 60: case 61://should be optimized with range
        state = 5;
        break;
      case 56: case 99:
        state = 8;
        break;
      default:
        state = 7; // normal state
      }

    //is pawn safe with others
      if(is_pawn_safe(pos, player)){
          state = 6;
    }

    return state;
}

bool rl_player::can_reach_opponent(int own_position){
    for (int i = 4; i < 16; i++) {
      if (own_position == pos_start_of_turn[i] && own_position != -1 &&
          own_position != 99 && own_position < 51){
        return true;
      }
    }
    return false;
}

// test if 2 pawns are in the same position (not own globe or home or home run)
bool rl_player::is_pawn_safe(int position, int player){
    bool is_safe = false;
    if(position != -1 && position != 0 && position < 51 && position != 13 &&
       position != 26 && position != 39){
      if (pos_start_of_turn[0] == position && player != 0)
          is_safe = true;
      if (pos_start_of_turn[1] == position && player != 1)
          is_safe = true;
      if (pos_start_of_turn[2] == position && player != 2)
          is_safe = true;
      if (pos_start_of_turn[3] == position && player != 3)
          is_safe = true;
    }
    return is_safe;
}

// not working correct yet
int rl_player::danger_level_for_pawn(int own_position){
  // if not save then check how many is behind me
  // if on a star - check 6-12 steps behind
   int number_of_pawns_close_behind = 0;
  for(int i = 4; i < 16; i++)
    for(int j = 1; j < 7; j++)
      if(own_position-j == pos_start_of_turn[i])
        number_of_pawns_close_behind++;
    return number_of_pawns_close_behind;
}

// collect all possible actions
vector<int> rl_player::player_actions(){
    vector<int> dummy;

    for (int i = 0; i < 4; i++) {
        vector<int> player_actions;
        // int action = -1;
        int start_pos = pos_start_of_turn[i];
        int end_pos = pos_start_of_turn[i] + dice_roll;

        // Check if pawn is still in home position
        if (start_pos == -1 && dice_roll == 6) {
            //"Action 1: Move to start",
            player_actions.push_back(1);
        } else if (start_pos != -1 && start_pos != 99) {
            // here is the main part of possible actions, between start and Goal

            //"Action 2: Move to globe (other)",
            if(end_pos == 8 || end_pos == 21 || end_pos == 34 || end_pos == 47)
                player_actions.push_back(2);

            //"Action 3: Movet to globe (opponent)",
            if(end_pos == 13 || end_pos == 26 || end_pos == 39)
                player_actions.push_back(3);

            //"Action 4: Move to star",
            if(end_pos == 5 || end_pos == 18 || end_pos == 31 || end_pos == 44
              || end_pos == 11 || end_pos == 24 || end_pos == 37)
                player_actions.push_back(4);

            //"Action 5: Move to star and then goal",
            if(end_pos == 50)
              player_actions.push_back(5);

            //"Action 6: Move to buddy pawn (safe zone)",
            if(is_pawn_safe(end_pos, i))
              player_actions.push_back(6);

            if(can_reach_opponent(end_pos)){
              if(isGlobe(end_pos)){
                //"Action 13: Loose own pawn",
                player_actions.push_back(13);
              }else{
                //"Action 7: Eliminate opponent pawn",
                player_actions.push_back(7);
              }
            }

            //danger_level_for_pawn(end_pos);
            //"Action 8: Normal move (very high danger - 3+)",
            //"Action 9: Normal move (high danger - 2)",
            //"Action 10: Normal move (danger - 1)",

            //"Action 12: Move to home run",
            if(end_pos >= 51)
              player_actions.push_back(12);


            //Action 14: Move to goal
            if(end_pos == 99 || end_pos == 56)
              player_actions.push_back(14);

            //"Action 11: Normal move (no danger)",
            if(player_actions.size() == 0)
              player_actions.push_back(11);

        }else {
          //"Action 0: Pawn is stuck",
          player_actions.push_back(0);
        }
        pawns[i].end_pos = end_pos;
        pawns[i].possible_actions = player_actions;
    }
    return dummy;
}

double rl_player::get_immediate_reward(Pawn selected_pawn){
  double reward = 0;

  switch(selected_pawn.selected_action){
    case ACTION_PAWN_STUCK:
      reward += 0;
      break;
    case ACTION_MOVE_TO_START:
      reward += 20;
      break;
    case ACTION_MOVE_TO_OTHER_GLOBE:
      reward += 15;
      break;
    case ACTION_MOVE_TO_OPPONENT_GLOBE:
      reward -= 10;
      break;
    case ACTION_MOVE_TO_STAR:
      reward += 5;
      break;
    case ACTION_MOVE_TO_STAR_AND_GOAL:
      reward += 10;
      break;
    case ACTION_SAFE_ZONE:
      reward += 25;
      break;
    case ACTION_ELIMINATE_OPPONENT:
      reward += 40;
      break;
    case ACTION_VERY_HIGH_DANGER_MOVE:
      reward -= 15;
      break;
    case ACTION_HIGH_DANGER_MOVE:
      reward -= 10;
      break;
    case ACTION_DANGER_MOVE:
      reward -= 5;
      break;
    case ACTION_NORMAL_MOVE:
      reward -= 1;
      break;
    case ACTION_HOME_RUN:
      reward += 5;
      if(selected_pawn.current_state == STATE_HOME_RUN)
        reward -= 30;
      break;
    case ACTION_LOOSE_OWN_PAWN:
      reward -= 30;
      break;
    case ACTION_MOVE_TO_GOAL:
      reward += 5;
      break;
    default:
      reward += 0;
  }
  return reward;
}

bool rl_player::isGlobe(int index){
    if(index < 52){     //check only the indexes on the board, not in the home streak
        if(index % 13 == 0 || (index - 8) % 13 == 0 || isOccupied(index) > 1){  //if more people of the same team stand on the same spot it counts as globe
            return true;
        }
    }
    return false;
}

int rl_player::isOccupied(int index){ //returns number of people of another color
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

void rl_player::update_q_table(Pawn selected_pawn){
  double immediate_reward = get_immediate_reward(selected_pawn);
  int current_state = selected_pawn.current_state;
  int action_with_highest_score = selected_pawn.selected_action;

  if(selected_pawn.current_state > 20 || selected_pawn.current_state < 0)
        current_state = 0;

  if(last_state > 20 || last_state < 0)
    last_state = 0;

  if(last_action > 20 || last_action < 0)
    last_action = 0;

    double q_old = Q(last_state,last_action);
    double q_new = Q(current_state,action_with_highest_score);

    Q(last_state,last_action) = q_old + ALFA * (immediate_reward + GAMMA *
                                                q_new - q_old);

}

Pawn rl_player::select_action(){
  int selected_action = 0;
  Pawn selected_pawn;
  selected_pawn.pawn = -1;
  // choose random action the first ~500 games, and then use best action
  vector<Pawn> pawns_that_can_move;
  for(int i = 0; i < 4; ++i){
    if(pawns[i].possible_actions.size() == 1){
      if(pawns[i].possible_actions[0] != 0){
        pawns_that_can_move.push_back(pawns[i]);
      }else{
        selected_pawn.selected_action = selected_action;
      }
    }else if(pawns[i].possible_actions.size() > 1)
      pawns_that_can_move.push_back(pawns[i]);
  }

  if(pawns_that_can_move.size() > 0){

    // note rand() does only provide pseudo random number
    int random_num = rand() % 100 + 1; //
    if(random_num < EPSILON){
      selected_pawn = select_action_random(pawns_that_can_move);
      exploration++;
    }
    else{
      selected_pawn = select_action_highest_q_value(pawns_that_can_move);
      explotation++;
    }
  }
  return selected_pawn;
}

Pawn rl_player::select_action_highest_q_value(vector<Pawn>
                                              &pawns_that_can_move){
  double highest_action_value = -100000; // very "low" number
  Pawn selected_pawn;//
  for(auto pawn : pawns_that_can_move){
      for(auto action : pawn.possible_actions){
        int state_action_value = Q(pawn.next_state, action);
        if(state_action_value > highest_action_value){
          highest_action_value = state_action_value;
          selected_pawn = pawn;
          selected_pawn.selected_action = action;
        }
      }
  }
  return selected_pawn;
}

Pawn rl_player::select_action_random(vector<Pawn> pawns_that_can_move){
  int selected_action = 0;
  int pick_random_player = rand() % pawns_that_can_move.size();
  Pawn selected_pawn = pawns_that_can_move[pick_random_player];
  int random_action = rand() % selected_pawn.possible_actions.size();
  selected_action = selected_pawn.possible_actions[random_action];
  selected_pawn.selected_action = selected_action;
  return selected_pawn;
}

int rl_player::get_probability_for_action_selections(int it){
    return INITIAL_EPSILON*pow((1-RATE_OF_DECREASE), it) + MIN_EPSILON;
}

double rl_player::get_new_alfa(int it){
  return INITIAL_ALFA*pow((1-ALFA_DECREASE_RATE), it) + 0.00001;
}

int rl_player::make_decision(){
    for(int i = 0; i < 4; ++i){
        pawns[i].cur_pos = pos_start_of_turn[i];
        pawns[i].current_state = get_state(i, false);
        pawns[i].next_state = get_state(i, true);

    }
    player_actions();

    Pawn selected_pawn = select_action();

    //
    update_q_table(selected_pawn);

    //update variables
    last_action = selected_pawn.selected_action;
    last_state = selected_pawn.current_state ;

    if(selected_pawn.pawn == -1)
      counter++;

    return selected_pawn.pawn;
}

void rl_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void rl_player::post_game_analysis(std::vector<int> relative_pos){

    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    if(game_complete)
      win_counter++;
    emit turn_complete(game_complete);
}

//if game is complete reinitialize variables
void rl_player::reset(vector<double> result){

  win_rate_file << (result[0]/games_played)*100  << ";"
                << (result[1]/games_played)*100 << ";"
                << (result[2]/games_played)*100  << ";"
                << (result[3]/games_played)*100 << endl;
  last_action = 0;
  last_state = 0;
  games_played++;
  EPSILON = get_probability_for_action_selections(games_played);

  for(int i = 0; i < Q.rows(); i++){
    for(int j = 0; j < Q.cols(); j++){
        q_table_elements << Q(i,j) << ";";
      }
      q_table_elements << endl;
    }
}

void rl_player::print_q_table(){
  int w = 9;
  cout << setw(5) <<"0" << setw(w)<< "1" << setw(w) << "2" << setw(w)<< "3" <<
      setw(w) << "4"<< setw(w) << "5"<< setw(w) << "6" << setw(w)<< "7" <<
      setw(w) << "8"<< setw(w) << "9"<< setw(w) << "10" << setw(w)<< "11"<<
      setw(w) << "12"<< setw(w) << "13" << setw(w) << "14" <<endl;
  cout << Q << endl;
}

void rl_player::load_q_table(MatrixXd &q_table){
  ifstream file("/home/per/q_table.txt");
  string line;
  if(file.is_open()){
    for(int i = 0; i < q_table.rows(); i++)
      for(int j = 0; j < q_table.cols(); j++){
        getline(file, line);
        string::size_type sz;
        double num = stod(line, &sz);
        q_table(i,j) = num;
      }
  }
}

void rl_player::save_q_table(){
  ofstream file("/home/per/q_table.txt");
  if(file.is_open()){
    for(int i = 0; i < Q.rows(); i++){
      for(int j = 0; j < Q.cols(); j++){
          file << Q(i,j) << endl;
        }
      }
      file.close();
      //cout << "Q-table saved" << endl;
  }else{
    //cout << "Error saving Q-table" << endl;
  }
  save_q_table_excel();
  win_rate_file.close();
  q_table_elements.close();
}

void rl_player::save_q_table_excel(){

  ofstream file("/home/per/q_table_excel.txt");;

  if(file.is_open()){
    for(int i = 0; i < Q.rows(); i++){
      for(int j = 0; j < Q.cols(); j++){
          file << Q(i,j) << ";";
        }
        file << endl ;
      }
      file.close();

      //cout << "percent of games won: " <<(win_counter/games_played)*100 << endl;
  }else{
    //cout << "Error saving Q-table" << endl;
  }
}
