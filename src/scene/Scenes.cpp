



#include "Scenes.hpp"


SplashScene::SplashScene(EagleGraphicsContext* window) :
      gothic(0),
      win(window),
      banner(),
      splashfont(0),
      time(-1.0),
      fadeintime(3.0),
      fadeouttime(3.0),
      displaytime(3.0)
{
   EAGLE_ASSERT(window);
}



void SplashScene::Display(EagleGraphicsContext* win) {
   if (time < 0.0) {return;}
   if (time > fadeintime + displaytime + fadeouttime) {
      return;
   }
   
   if (time < fadeintime) {
      double pct = time/fadeintime;
      win->DrawImageFit(gothic , Rectangle(0,0,win->Width() , win->Height()) , EagleColor(1.0f , 1.0f , 1.0f , pct));
   }
   else if (time < (fadeintime + displaytime)) {
      win->DrawImageFit(gothic , Rectangle(0,0,win->Width() , win->Height()));
   }
   else if (time < (fadeintime + displaytime + fadeouttime)) {
      double pct = (time - (fadeintime + displaytime))/(fadeouttime);
      win->DrawImageFit(gothic , Rectangle(0,0,win->Width() , win->Height()) , EagleColor(1.0f , 1.0f , 1.0f , 1.0 - pct));
   }
   
   win->DrawShadedQuad(20 , 20 , EagleColor(0,0,255) , 20 , 200 , EagleColor(255,0,0) , 200 , 200 , EagleColor(0,255,0) , 200 , 20 , EagleColor(255,255,255));
   
      
   double pct = banner.GetNormalizedPercent();
   win->DrawTextString(splashfont , "Victory Garden" , win->Width() - (pct*(win->Width() + splashfont->Width("Victory Garden"))) , win->Height()/2 , EagleColor(0,192,64) , HALIGN_LEFT , VALIGN_CENTER);
   
   win->DrawTextString(win->DefaultFont() , StringPrintF("%2.4lf" , pct) , 10 , 10 , EagleColor(255,255,255));
}



STATUS SplashScene::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
      return STATUS_COMPLETE;
   }
   if (time < 0.0) {
      return STATUS_INVALID;
   }
   else if (time < fadeintime + displaytime + fadeouttime) {
      return STATUS_RUNNING;
   }
   return STATUS_COMPLETE;
}



STATUS SplashScene::Update(double dt) {
   time += dt;
   banner.AdvanceAnimationTime(dt);
   if (time > fadeintime + displaytime + fadeouttime) {
      return STATUS_COMPLETE;
   }
   return STATUS_RUNNING;
}



bool SplashScene::Init() {
   Destroy();
   time = 0;
   gothic = win->LoadImageFromFile("assets/GrantWood800tall.png");
   splashfont = win->LoadFont("assets/WatersGothic.ttf" , -360);
   EAGLE_ASSERT(gothic && gothic->Valid());
   EAGLE_ASSERT(splashfont && splashfont->Valid());
   
   banner.Init(1 , 1 , (fadeintime + displaytime + fadeouttime)/2.0 , ANIMATION_REPEAT_FORWARDS_AND_BACKWARDS);
   
   return gothic && splashfont;
}



void SplashScene::Destroy() {
   if (gothic) {
      win->FreeImage(gothic);
      gothic = 0;
   }
   if (splashfont) {
      win->FreeFont(splashfont);
      splashfont = 0;
   }
   time = -1;
}

