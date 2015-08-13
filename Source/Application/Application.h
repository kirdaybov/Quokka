#pragma once

namespace quokka
{
  class Application
  {
  public:
    virtual void Init() = 0;
    virtual void Run() = 0;
    virtual void Shutdown() = 0;

	  virtual void Quit() = 0;

	  virtual float Width() = 0;
	  virtual float Height() = 0;
  };

	Application* GApp();
}