#include "core/application.h"

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  Application app(SCR_WIDTH, SCR_HEIGHT, "OpenGL Cube");
  app.Run();

  return 0;
}
