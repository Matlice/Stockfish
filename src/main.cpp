/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2020 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NOMAIN
#include <iostream>
#include <interface/interface.h>

#include "bitboard.h"
#include "endgame.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "tt.h"
#include "uci.h"
#include "syzygy/tbprobe.h"


int main(int argc, char *argv[]) {

    std::cout << engine_info() << std::endl;

    EngineInterface *iface = EngineInterface::getInstance();
    iface->setPosition(StartFEN);
    std::cout << *iface << std::endl;

    iface->makeMove("e2e4");
    iface->flip();
    std::cout << *iface << std::endl;

    Search::LimitsType limits;
    limits.depth = 14;

    for (int i = 0; i < 10; ++i) {
        Move m = *iface->bestMove(limits, false);
        iface->makeMove(m);
        std::cout << *iface << std::endl;
        iface->flip();
    }

    iface->newGame();

    iface->makeMove("a2a3");
    iface->flip();
    std::cout << *iface << std::endl;

    for (int i = 0; i < 10; ++i) {
        Move m = *iface->bestMove(limits, false);
        iface->makeMove(m);
        std::cout << *iface << std::endl;
        iface->flip();
    }

    Threads.set(0);
    return 0;
}
#endif