#pragma once

struct SDL_Window;
union SDL_Event;

class DebugGui {
  public:
    DebugGui();
    ~DebugGui();

    void Init(SDL_Window *window);
    void Update();
    void GetInput(SDL_Event &e);

  private:
    SDL_Window *Window;
    bool my_tool_active;
};

