#include "ScoreBoard.hpp"
#include "CommandController.hpp"

int main() {
  ScoreBoard scoreBoard("Ma Long", "Fan Zhendong");

  CommandController cmdCtl{"Enter command:", FrameHeight + 2};

  while (true) {
    auto cmd = cmdCtl.receiveCommand();
    if (cmd == "q" || cmd == "quit")
      break;
    if (cmd == "l+")
      scoreBoard.leftInc();
    else if (cmd == "l-")
      scoreBoard.leftDec();
    else if (cmd == "r+")
      scoreBoard.rightInc();
    else if (cmd == "r-")
      scoreBoard.rightDec();
    else if (cmd == "r0")
      scoreBoard.rightToZero();
    else if (cmd == "l0")
      scoreBoard.leftToZero();
    else if (cmd == "ro+")
      scoreBoard.rightOverAllInc();
    else if (cmd == "lo+")
      scoreBoard.leftOverAllInc();
    else if (cmd == "ro-")
      scoreBoard.rightOverAllDec();
    else if (cmd == "lo-")
      scoreBoard.leftOverAllDec();
  }

  return 0;
}