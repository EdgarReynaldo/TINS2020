


#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"



#include "scene/Scenes.hpp"



int main(int argc , char** argv) {
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   
   if (ret != EAGLE_FULL_SETUP) {
      EagleInfo() << "Warning, couldn't init everything requested." << std::endl;
      if (!(ret & (EAGLE_SYSTEM | EAGLE_MOUSE | EAGLE_KEYBOARD))) {
         EagleError() << "Failed to init basic eagle system." << std::endl;
         return -1;
      }
   }
   
   /// TODO : ADD CONFIG LOADING
   int ww = 1280;
   int wh = 800;
   int fsw = 1920;
   int fsh = 1080;
   int fullscreen = 0;
   int mode = EAGLE_WINDOWED | EAGLE_OPENGL;
   ConfigFile cfg;
   if (cfg.LoadFromFile("VictoryGarden.cfg")) {
      try {
         ww = std::stoi(cfg["Graphics"]["WWidth"]);
         wh = std::stoi(cfg["Graphics"]["WHeight"]);
         fsw = std::stoi(cfg["Graphics"]["FSWidth"]);
         fsh = std::stoi(cfg["Graphics"]["FSHeight"]);
         fullscreen = std::stoi(cfg["Graphics"]["fullscreen"]);
         if (fullscreen) {
            mode = EAGLE_FULLSCREEN_WINDOW | EAGLE_OPENGL;
         }
      } catch (std::exception& e) {
         EagleError() << e.what() << std::endl;
      }
   }
   
   int sw = fullscreen?fsw:ww;
   int sh = fullscreen?fsh:wh;
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Victory Garden" , sw , sh , mode);
   
   if (!(win && win->Valid())) {
      EagleError() << "Failed to create valid window at requested size of " << sw << " x " << sh << "." << std::endl;
      return -10;
   }
   
   ProgramTime::SyncTime(0.0);
   sys->GetSystemTimer()->Start();
   
   bool redraw = true;
   bool quit = false;
   
   SplashScene splash(win);
   if (!splash.Init()) {
      return 2;
   }
   
   while (!quit) {
      if (redraw) {
         
         win->Clear();
   
         splash.Display(win);
         
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            if (splash.Update(e.timer.eagle_timer_source->SPT()) == STATUS_COMPLETE) {
               quit = true;
            }
            redraw = true;
         }
         if (input_key_held(EAGLE_KEY_ANY_CTRL) && input_key_press(EAGLE_KEY_F1)) {
            fullscreen = !fullscreen;
            splash.Destroy();
            if (fullscreen) {
               sw = fsw;
               sh = fsh;
               mode = EAGLE_FULLSCREEN_WINDOW | EAGLE_OPENGL;
            }
            else {
               mode = EAGLE_WINDOWED | EAGLE_OPENGL;
               sw = ww;
               sh = wh;
            }
            win->Create(sw , sh , mode);
            splash.Init();
         }
      } while (!sys->UpToDate());
   }
   
   splash.Destroy();
   
   cfg["Graphics"]["WWidth"] = StringPrintF("%d" , ww);
   cfg["Graphics"]["WHeight"] = StringPrintF("%d" , wh);
   cfg["Graphics"]["FSWidth"] = StringPrintF("%d" , fsw);
   cfg["Graphics"]["FSHeight"] = StringPrintF("%d" , fsh);
   cfg["Graphics"]["fullscreen"] = fullscreen?"1":"0";
   
   cfg.SaveToFile("VictoryGarden.cfg");
   
   return 0;
   /*
   TileMap tmap;
   
   FarmGame farm;
   
   std::string start = "Farms/Beginner/BasicFarm.frm";
   if (!tmap.Load(win , start) {
      EagleError()( << "Failed to load " << start << " map." << std::endl;
      return 10;
   }
   
   sys->GetSystemTimer()->Start();
   
   return farm.Play(win , tmap);
   
   */
//   return 0;
}




