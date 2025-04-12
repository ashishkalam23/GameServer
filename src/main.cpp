#include "State/Game.h"
#include "CachupaServer.h"
#include <optional>
#include <unistd.h>
#include <ranges>

// #define startOrJoinGameUserPrompt "Please enter the invite code to join an already existing game or create a new game(type !newgm):"
// #define newGameCreated "Your game was created successfully. Please use the invite code given below to invite players to your game :)"

using std::optional;

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  const unsigned short port = std::stoi(argv[1]);
  CachupaServer srvr(port,argv[2]);
  return 0;
}
