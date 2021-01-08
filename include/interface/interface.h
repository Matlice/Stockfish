//
// Created by stefano on 1/8/21.
//

#ifndef STOCKFISH_INTERFACE_H
#define STOCKFISH_INTERFACE_H

#include <position.h>
#include <uci.h>
#include <search.h>

#define StartFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

namespace PSQT {
    void init();
}

class EngineInterface {
private:
    EngineInterface(bool, std::map<std::string, UCI::Option, UCI::CaseInsensitiveLess> * startingOptions);
    EngineInterface(){initialized = false;}
    bool initialized;
    Position pos;
    StateListPtr states;
    bool is_searching;
    Move _bestMove;
    void renewState();
    bool bis_960;
public:
    friend std::ostream & operator<<(std::ostream& os, const EngineInterface& iface);
    static EngineInterface *getInstance();
    static EngineInterface *getInstance(bool, std::map<std::string, UCI::Option, UCI::CaseInsensitiveLess>*);
    void operator=(const EngineInterface &) = delete;
    static bool setOption(const std::string& name, const std::string& value);
    void setPosition(Position &pos);
    void setPosition(const std::string& fen);
    bool makeMove(Move move);
    bool makeMove(std::string move);
    void bestMoveAsync(Search::LimitsType &limits, bool ponder);
    Move * getBestMove();
    Move * bestMove(Search::LimitsType &limits, bool ponder);
    void flip();
    void newGame();
    [[nodiscard]] bool is_960() const{return this->bis_960;}
    float getScore(bool print=false);
};

#endif //STOCKFISH_INTERFACE_H
