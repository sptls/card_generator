#include "src/gui.h"


int main(int argc, char **argv) 
{
   Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("card.maker");
   std::unique_ptr<Gui> g;
   if(argc > 1)
   {
      std::ifstream f(argv[1]);
      if(f.good())
         g.reset(new Gui(argv[1]));
      else
      {
         printf("File \"%s\" doesn't exist!\n", argv[1]);
         return -1;
      }
   }
   else
      g.reset(new Gui());
   
   return app->run(*g->windowMain);
}