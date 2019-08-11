#include "gol.h"
#include <exception>
#include <iostream>

void test_all_die()
{
    auto game = gol::Game(gol::Game::Citizens{
        {2,2},
        {1,3}
    });

    if (game.next() != gol::Game::NextStatus::ALL_DIE) {
        throw std::logic_error("test_all_die: game was not stopped by all die");
    }
    if (!game.citizens().empty()) {
        throw std::logic_error("test_all_die");
    }
}

void test_still_lifes()
{
    auto citizens = gol::Game::Citizens{
        {2,2},
        {3,2},
        {4,3},
        {3,4},
        {2,4},
        {1,3}
    };

    auto game = gol::Game(citizens);
    if (game.next() != gol::Game::NextStatus::STILL_LIFES) {
        throw std::logic_error("test_still_lifes: game was not stopped");
    }

    if (game.citizens() != citizens) {
        throw std::logic_error("test_still_lifes: citizens changed");
    }
}

void test_out_of_rang()
{
    auto pos = gol::Position{gol::Position::max_axis() - 3, gol::Position::max_axis() - 3};
    auto game = gol::Game(gol::Game::Citizens{
        {pos.x + 1, pos.y + 0},
        {pos.x + 2, pos.y + 1},
        {pos.x + 2, pos.y + 2},
        {pos.x + 1, pos.y + 2},
        {pos.x + 0, pos.y + 2}
    });

    game.next();
    if (game.next() != gol::Game::NextStatus::OUT_OF_RANGE) {
        throw std::logic_error("test_out_of_rang: game was not stopped by out of rang");
    }
}

void test_glider()
{
    auto game = gol::Game(gol::Game::Citizens{
        {2,1},
        {3,2},
        {1,3},
        {2,3},
        {3,3}
    });

    if (game.next() != gol::Game::NextStatus::OK) {
        throw std::logic_error("test_glider");
    }

    if (game.citizens() != gol::Game::Citizens{
        {1,2},
        {3,2},
        {2,3},
        {3,3},
        {2,4}
    }) {
        throw std::logic_error("test_glider");
    }
}

int main(int argc, const char* args[])
{
    try {
        test_all_die();
        test_still_lifes();
        test_out_of_rang();
        test_glider();
    } catch (const std::logic_error& e) {
        std::cout << "FILED: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "PASSED" << std::endl;

    return 0;
}