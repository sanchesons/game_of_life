#include "gol.h"
#include "gol_view.h"
#include <charconv>
#include <optional>

std::optional<gol::Cell> parse_cell(const std::string& str)
{
    auto cell = std::make_optional<gol::Cell>();
    const auto* end = str.c_str()+str.size();
    if (auto [next, error] = std::from_chars(str.c_str(), end, cell->x); error != std::errc()) {
        return {};
    } else if(auto error = std::from_chars(next+1, end, cell->y).ec; error != std::errc()) {
        return {};
    }
    return cell;
}

std::optional<gol::Game::Citizens> read_citizens()
{
    auto cell_list = std::make_optional<gol::Game::Citizens>();
    for (std::string line; std::getline(std::cin, line);) {
        if (auto cell = parse_cell(line); cell) {
            cell_list->emplace(*cell);
        } else {
            return {};
        }
    }
    return cell_list;
}

int main(int argc, const char* args[])
{
    auto citizens = read_citizens();
    if (!citizens) {
        std::cout << "bad input data format" << std::endl;
        return 1;
    }
    auto game = gol::Game(std::move(*citizens));
    auto view = gol::GameView(game, {{0,0}, {80,20}});
    for(auto i = uint64_t(0); i < 60 && game.next() == gol::Game::NextStatus::OK; ++i) {
        view.print();
    }

    return 0;
}
