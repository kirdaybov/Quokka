#pragma once
#include <string>

namespace quokka
{
  struct SApplicationSettings
  {
    int width = 1024;
    int height = 768;
    bool fullscreen = false;

    std::string creature_texture = "ship";
    std::string bacground_texture = "background";
  };

  class QApplication
  {
  public:
    virtual void Init() = 0;
    virtual void Run() = 0;
    virtual void Shutdown() = 0;

	  virtual void Quit() = 0;

	  virtual float Width() = 0;
	  virtual float Height() = 0;

    virtual std::string GetExePath() = 0;

    SApplicationSettings Settings;
  };

	QApplication* GApp();
}