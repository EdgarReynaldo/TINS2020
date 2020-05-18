



#ifndef TINS2020_SCENES_HPP
#define TINS2020_SCENES_HPP


#include "Eagle.hpp"


enum STATUS {
   STATUS_INVALID  = 0,
   STATUS_READY    = 1,
   STATUS_RUNNING  = 2,
   STATUS_COMPLETE = 3,
   STATUS_QUIT     = 4
};


class Scene {
   
public :
   Scene* next;
   
   virtual ~Scene() {}

   virtual void Display(EagleGraphicsContext* win)=0;
   virtual STATUS HandleEvent(EagleEvent e)=0;
   virtual STATUS Update(double dt)=0;
   
   virtual bool Init()=0;
   virtual void Destroy()=0;
   
   Scene* Next() {return next;}
};

class SplashScene : public Scene {
   EagleImage* gothic;
   EagleGraphicsContext* win;
   
   AnimationBase banner;
   EagleFont* splashfont;
   
   
   double time;
   double fadeintime;
   double fadeouttime;
   double displaytime;
   
public :
   SplashScene(EagleGraphicsContext* window);
   ~SplashScene() {Destroy();}
   
   virtual void Display(EagleGraphicsContext* win);
   virtual STATUS HandleEvent(EagleEvent e);
   virtual STATUS Update(double dt);
   
   virtual bool Init();
   virtual void Destroy();
};

class StoryScene : public Scene {

   EagleGraphicsContext* win;

   AnimationBase master;
   
   EagleFont* font;
   EagleFont* fontsm;


   double intime;
   double dtime;
   double outtime;
   
   bool wait_for_key;
   
public :
   StoryScene(EagleGraphicsContext* window);
   ~StoryScene();
   
   virtual void Display(EagleGraphicsContext* win);
   virtual STATUS HandleEvent(EagleEvent e);
   virtual STATUS Update(double dt);
   
   virtual bool Init();
   virtual void Destroy();
};


class CharacterScene : public Scene {
   
   std::string choice;
   
   EagleGraphicsContext* win;
   
   EagleImage* jack240;
   EagleImage* jill240;
   
   WidgetHandler gui;
   RelativeLayout rlayout;
   IconButton jackbtn;
   IconButton jillbtn;
   
public :
   CharacterScene(EagleGraphicsContext* window);
   virtual ~CharacterScene();
   
   STATUS status;
   
   virtual void Display(EagleGraphicsContext* win);
   virtual STATUS HandleEvent(EagleEvent e);
   virtual STATUS Update(double dt);
   
   virtual bool Init();
   virtual void Destroy();
   
   std::string CharacterChoice() {return choice;}
};


#endif // TINS2020_SCENES_HPP
