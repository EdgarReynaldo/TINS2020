



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
   
///   win->DrawShadedQuad(20 , 20 , EagleColor(0,0,255) , 20 , 200 , EagleColor(255,0,0) , 200 , 200 , EagleColor(0,255,0) , 200 , 20 , EagleColor(255,255,255));
   
      
   double pct = banner.GetNormalizedPercent();
   win->DrawTextString(splashfont , "Victory Garden" , win->Width() - (pct*(win->Width() + splashfont->Width("Victory Garden"))) , win->Height()/2 , EagleColor(0,192,64) , HALIGN_LEFT , VALIGN_CENTER);
   
   win->DrawTextString(win->DefaultFont() , StringPrintF("%2.4lf" , pct) , 10 , 10 , EagleColor(255,255,255));
}



STATUS SplashScene::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_KEY_DOWN) {
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



/// ------------------------- Story scene  -------------------------------



StoryScene::StoryScene(EagleGraphicsContext* window) :
   win(window),
   master(),
   font(0),
   fontsm(0),
   intime(1.0/6.0),
   dtime(1.0/2.0),
   outtime(1.0/3.0),
   wait_for_key(false)
{
   EAGLE_ASSERT(window);
}



StoryScene::~StoryScene() {
   Destroy();
}



void StoryScene::Display(EagleGraphicsContext* win) {
   double pct = master.GetNormalizedPercent();
   if (pct > 1.0) {
      pct = 2.0 - pct;
   }
   double alpha = -1.0;
   if (pct <= intime) {
      pct = pct/intime;
      // fadein
      alpha = pct;
   }
   else if (pct <= intime + dtime) {
      pct = 1.0;
      // display
      alpha = 1.0;
   }
   else if (pct <= 1.0) {
      pct = 1.0 - (pct - (intime + dtime))/outtime;
      // fadeout
      alpha = pct;
   }
   
   static const std::string continu = {"Press Any Key"};
   static const std::string title = {
      "Victory Garden\n"
      " the true story of\n"
      "  American Gothic\n"
   };
   static const std::string story = {
      "Jack and Jill were forced to marry\n"
      " each other and move into this old farmhouse\n"
      "Jill wants a divorce, and she wants the house too\n"
      "She challenged Jack to a flower growing contest\n"
      "...whoever wins... ...gets the farm...\n"
      "...and the loser has to leave...\n"
   };
   
   Pos2D center(win->Width()/3.0 , win->Height()/2.0);

   Translate2 transl1(Pos2D(0 - font->Width(title) , center.Y() - 10) , Pos2D(center.X() , center.Y() - 10));
   Translate2 transr1(Pos2D(win->Width() , center.Y() + 10) , Pos2D(center.X() , center.Y() + 10));
   Translate2 transl2(transl1.dpt , transl1.spt);
   Translate2 transr2(transr1.dpt , transr1.spt);
   
   transl1.Recalc(pct);
   transr1.Recalc(pct);
   transl2.Recalc(pct);
   transr2.Recalc(pct);
   
   Translate2* tl = (master.GetAnimationPercent() > 1.0)?&transl1:&transl2;
   Translate2* tr = (master.GetAnimationPercent() > 1.0)?&transr1:&transr2;
   
   tl = &transl1;
   tr = &transr1;
   
//   if (!wait_for_key) {
      win->DrawMultiLineTextString(font , title , tl->cpt.X() , tl->cpt.Y() , EagleColor(0.2f,0.8f,0.4f,alpha) , 2 , HALIGN_LEFT , VALIGN_BOTTOM);
      win->DrawMultiLineTextString(fontsm , story , tr->cpt.X() , tr->cpt.Y() , EagleColor(0.4f,0.9f , 0.6f , alpha) , 8 , HALIGN_LEFT , VALIGN_TOP);
//   }
   if (master.GetAnimationPercent() > intime + dtime) {
      if (!wait_for_key) {
         wait_for_key = true;
      }
      float falpha = 1.0 - alpha;
      win->DrawTextString(font , continu , win->Width()/2.0 , win->Height()/2.0 , EagleColor(falpha,falpha,falpha,falpha) , HALIGN_CENTER , VALIGN_CENTER);
   }
}



STATUS StoryScene::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      return STATUS_QUIT;
   }
   if (wait_for_key) {
      if (e.type == EAGLE_EVENT_KEY_DOWN) {// && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         return STATUS_COMPLETE;
      }
   }
   return STATUS_RUNNING;
}



STATUS StoryScene::Update(double dt) {
   if (master.GetTotalPercent() < 1.0) {
      master.AdvanceAnimationTime(dt);
   }
   else {
      (void)0;
   }
   return STATUS_RUNNING;
}



bool StoryScene::Init() {
   Destroy();
   master.Init(1,1,16.0 , ANIMATION_ONCE_FORWARDS_AND_BACKWARDS);
   font = win->LoadFont("assets/WatersGothic.ttf" , -120);
   fontsm = win->LoadFont("assets/WatersGothic.ttf" , -48);
   EAGLE_ASSERT(font && font->Valid() && fontsm && fontsm->Valid());
   return font && fontsm && font->Valid() && fontsm->Valid();
}



void StoryScene::Destroy() {
   if (font) {
      win->FreeFont(font);
      font = 0;
   }
   if (fontsm) {
      win->FreeFont(fontsm);
      fontsm = 0;
   }
}



/// ------------------------     Character choice selection screen     -------------------------------------



CharacterScene::CharacterScene(EagleGraphicsContext* window) :
      Scene(),
      choice("None"),
      win(window),
      jack240(0),
      jill240(0),
      gui(0 , "Selection screen"),
      rlayout(),
      jackbtn(),
      jillbtn()
{}



CharacterScene::~CharacterScene() 
{
   Destroy();
}


void CharacterScene::Display(EagleGraphicsContext* win) {
   gui.Display(win , 0 , 0);
}



STATUS CharacterScene::HandleEvent(EagleEvent e) {
   gui.HandleEvent(e);
   while (gui.HasMessages()) {
      WidgetMsg msg = gui.TakeNextMessage();
      if (msg == WidgetMsg(&jackbtn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED)) {
         choice = "Jack";
      }
      if (msg == WidgetMsg(&jillbtn , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED)) {
         choice = "Jill";
         return STATUS_COMPLETE;
      }
   }
   return STATUS_RUNNING;
}



STATUS CharacterScene::Update(double dt) {
   int s = gui.Update(dt);
   (void)s;
   return STATUS_RUNNING;
}



bool CharacterScene::Init() {
   Destroy();
   
   jack240 = win->LoadImageFromFile("assets/Jack240.png");
   jill240 = win->LoadImageFromFile("assets/Jill240.png");
   
   jackbtn.SetImages(jack240 , jack240 , jack240 , jack240);
   jillbtn.SetImages(jill240 , jill240 , jill240 , jill240);
   
   
   bool setup = gui.SetupBuffer(win->Width() , win->Height() , win);
   rlayout.Resize(2);
   rlayout.PlaceWidget(&jillbtn , 0 , LayoutRectangle(0.2 , 0.2 , 0.2 , 0.6));
   rlayout.PlaceWidget(&jackbtn , 1 , LayoutRectangle(0.6 , 0.2 , 0.2 , 0.6));
   gui.ClearLayout();
   gui.SetRootLayout(&rlayout);
   
   return setup && jack240 && jill240 && jack240->Valid() && jill240->Valid();
   
}


void CharacterScene::Destroy() {
   if (jack240) {
      win->FreeImage(jack240);
      jack240 = 0;
   }
   if (jill240) {
      win->FreeImage(jill240);
      jill240 = 0;
   }
   gui.FreeImageBuffers();
}
