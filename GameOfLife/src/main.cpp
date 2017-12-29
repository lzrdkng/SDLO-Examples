#include <cstdlib>

#include <string>
#include <vector>

#include <Cobra/SDL.hpp>

#include "Conway.hpp"

/**
 * @brief Procedure that draw all the cells from the conway game to the renderer
 * @param render The renderer instance
 * @param game The Conway game instance
 * @param size The window current size
 */ 
void draw(SDL::Renderer& render, const Conway& game, const SDL::Pair<int>& size);

int main(int argc, char** argv)
{
  double p = 0.2;
  int width  = 512;
  int height = 512;
  
  SDL::init(SDL::InitVideo);
  
  SDL::Window window("Conway Game of Life");
  window.setResizable(true);

  SDL::Pair<int> size = window.getSize();

  SDL::Renderer render(window, SDL::RendererAccelerated);

  Conway game(p, height, width);

  window.setMinimumSize(width, height);

  SDL_Event event;

  bool stop = false;
  
  do
  {
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT)
      break;

    if (event.type == SDL_WINDOWEVENT)
    {
      switch (event.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
      case SDL_WINDOWEVENT_SIZE_CHANGED:
	size = window.getSize();
	draw(render, game , size);
	break;
      default:
	break;
      }
    }

    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_SPACE:
	stop = !stop;
	break;
      default:
	break;
      }

      SDL_FlushEvent(SDL_KEYDOWN);
    }

    if (!stop)
    {
      draw(render, game, size);
      game.step();      
    }
    
  } while(true);

  SDL::quit();
  
  return 0;
}



void draw(SDL::Renderer& render, const Conway& game, const SDL::Pair<int>& size)
{
  const std::vector<std::vector<int>>& cells = game.getCells();

  std::vector<SDL::Rect> rects;

  Uint16 width  = static_cast<Uint16>(std::ceil(
					size.first /
					static_cast<double>(cells[0].size())
					));
  
  Uint16 height = static_cast<Uint16>(std::ceil(
					size.second /
					static_cast<double>(cells.size())
					));  
  for (uint i=0; i<cells.size(); ++i)
    for (uint j=0; j<cells[0].size(); ++j)
      if (cells[i][j])
	rects.push_back({static_cast<Sint16>(width  * j),
			 static_cast<Sint16>(height * i),
			 width,
			 height});

  render.setDrawColor(0);
  render.clear();

  render.setDrawColor(UINT32_MAX);
  render.drawRects(rects);
  
  render.present();
}
