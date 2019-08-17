#include "gol.h"
#include <iostream>
#include <ostream>

namespace gol
{

struct ViewPort
{
    Position pos1;
    Position pos2;
};

class GameView
{
private:
    const ViewPort m_view_port;
    const Game& m_game;

public:
    GameView(const Game& game, const ViewPort& view_port) :
        m_game(game),
        m_view_port(view_port)
    {}

    void print()
    {
        std::cout << *this << std::endl;
        std::cout.flush();
    }

    friend std::ostream& operator<<(std::ostream& out, const GameView& view)
    {
        const Game::Citizens& citizens = view.m_game.citizens();
        const Position& pos1 = view.m_view_port.pos1;
        const Position& pos2 = view.m_view_port.pos2;
        for (auto i = pos1.y; i < pos2.y; ++i) {
            for (auto j = pos1.x; j < pos2.x; ++j) {

                if (citizens.count({j,i})) {
                    out << '*';
                } else {
                    out << '-';
                }
            }
            out << std::endl;
        }
        return out;
    }
};

}
