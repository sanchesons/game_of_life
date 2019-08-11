#include "gol.h"
#include "gol_view.h"
#include <charconv>

auto parse_cell(const std::string& str)
{
    gol::Cell cell;
    const auto* end = str.c_str()+str.size();
    if (auto [next, error] = std::from_chars(str.c_str(), end, cell.x); error != std::errc()) {
        throw std::runtime_error("Bad input");
    } else if(auto error = std::from_chars(next+1, end, cell.y).ec; error != std::errc()) {
        throw std::runtime_error("Bad input");
    }
    return cell;
}

int main(int argc, const char* args[])
{
    gol::Game::Citizens cell_list;
    for (std::string line; std::getline(std::cin, line);) {
        cell_list.emplace(parse_cell(line));
    }

    auto game = gol::Game(std::move(cell_list));
    auto view = gol::GameView(game, {{0,0}, {80,20}});
    for(auto i = uint64_t(0); i < 60 && game.next() == gol::Game::NextStatus::OK; ++i) {
        view.print();
    }

    return 0;
}
