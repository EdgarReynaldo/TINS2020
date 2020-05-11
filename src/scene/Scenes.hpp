



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
   
   virtual ~Scene() {}
   
   STATUS status;
   
   virtual void Display(EagleGraphicsContext* win)=0;
   virtual STATUS HandleEvent(EagleEvent e)=0;
   virtual STATUS Update(double dt)=0;
   
   virtual bool Init()=0;
   virtual void Destroy()=0;
   
   
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




#endif // TINS2020_SCENES_HPP
