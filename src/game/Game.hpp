



#ifndef TINS_2020_GAME_HPP
#define TINS_2020_GAME_HPP



class Player {
   int score;
   std::string name;
   bool home;
   
   virtual void Act()=0;
};

class AIPlayer : public Player {
   virtual void Act();
};

class Game {
   Player* human;
   Player* ai;
   double timeofday;
   
   
   Scene* currentscene;
   
   
   
};

#endif // TINS_2020_GAME_HPP
