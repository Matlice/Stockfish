//
// Created by stefano on 1/8/21.
//

#include "interface/interface.h"
#include <memory>
#include "bitboard.h"
#include "endgame.h"
#include "position.h"
#include "search.h"
#include "thread.h"
#include "uci.h"

EngineInterface *EngineInterface::getInstance() {
    return EngineInterface::getInstance(false, nullptr);
}

EngineInterface *EngineInterface::getInstance(bool isCess960,
                                              std::map<std::string, UCI::Option, UCI::CaseInsensitiveLess> *startingOptions) {
    static EngineInterface instance = EngineInterface(isCess960, startingOptions);
    return &instance;
}

EngineInterface::EngineInterface(bool isChess960,
                                 std::map<std::string, UCI::Option, UCI::CaseInsensitiveLess> *startingOptions) {
//    CommandLine::init(argc, argv);
    UCI::init(Options);
    Tune::init();
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Endgames::init();
    Threads.set(size_t(Options["Threads"]), &(this->_bestMove));
    Search::clear(); // After threads are up
    Eval::NNUE::init();

    this->bis_960 = isChess960;

    this->renewState();
    this->pos.set(StartFEN, isChess960, &states->back(), Threads.main());
    if (startingOptions != nullptr)
        for (auto &startingOption : *startingOptions)
            this->setOption(startingOption.first, startingOption.second);

    this->_bestMove = MOVE_NULL;

    //Threads.set(0);
}

bool EngineInterface::setOption(const std::string &name, const std::string &value) {
    if (Options.count(name)) {
        Options[name] = value;
        return true;
    }
    return false;
}

void EngineInterface::setPosition(Position &pos) {
    this->setPosition(pos.fen());
}

void EngineInterface::setPosition(const std::string &fen) {
    this->renewState();
    this->pos.set(fen, Options["UCI_Chess960"], &states->back(), Threads.main());
}

bool EngineInterface::makeMove(Move move) {
    this->renewState();
    if (move != MOVE_NULL && is_ok(move)) {
        this->pos.do_move(move, states->back());
        return true;
    }
    return false;
}

bool EngineInterface::makeMove(std::string move) {
    return this->makeMove(UCI::to_move(this->pos, move));
}

void EngineInterface::newGame() {
    this->setPosition(StartFEN);
    Search::clear();
}

void EngineInterface::bestMoveAsync(Search::LimitsType& limits, bool ponder) {
    this->is_searching = true;
    this->_bestMove = MOVE_NULL;
    Threads.start_thinking(this->pos, this->states, limits, ponder);
}

__attribute__((optimize("O0")))
Move * EngineInterface::getBestMove() {
    assert(this->is_searching);
    while(this->_bestMove == MOVE_NULL)
       ;//std::cout << "waiting " << std::endl;
    this->is_searching = false;
    return &this->_bestMove;
}

std::ostream& operator<<(std::ostream& os, const EngineInterface& iface){
    os << iface.pos;
    return os;
}

Move * EngineInterface::bestMove(Search::LimitsType &limits, bool ponder) {
    bestMoveAsync(limits, ponder);
    return getBestMove();
}

void EngineInterface::flip() {
    this->pos.flip();
}

void EngineInterface::renewState() {
    this-> states = std::make_unique<std::deque<StateInfo>>(1);
}

float EngineInterface::getScore(bool print){
    float f;
    Eval::int_trace(this->pos, print, &f);
    return f;
}