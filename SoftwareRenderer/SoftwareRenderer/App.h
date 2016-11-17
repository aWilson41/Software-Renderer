#pragma once
#include "Engine.h"

class App : public Engine
{
private:
   Scene scene;

protected:
   void Initialize();
   void UnLoad();
   void Load();
   void Update();
   void Draw();
};