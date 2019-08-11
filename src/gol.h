#pragma once

#include <algorithm>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace gol
{

struct Position
{
    using Axis = int64_t;

    Axis x = 0;
    Axis y = 0;

    static constexpr Axis min_axis()
    {
        return std::numeric_limits<Axis>::min() + 1;
    }

    static constexpr Axis max_axis()
    {
        return std::numeric_limits<Axis>::max() - 1;
    }

    static bool is_axis_out_of_range(Axis axis)
    {
        return axis == min_axis() || axis == max_axis();
    }

    bool is_out_of_range() const
    {
        return is_axis_out_of_range(x) || is_axis_out_of_range(y);
    }

    bool operator==(const Position& pos) const
    {
        return x == pos.x && y == pos.y;
    }
};

}

namespace std
{

template<>
struct hash<gol::Position>
{
    template<typename T>
    static void hash_combine(size_t& seed, const T& v)
    {
        seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(const gol::Position& position) const noexcept
    {
        auto seed = std::size_t(0);
        hash_combine(seed, position.x);
        hash_combine(seed, position.y);
        return seed;
    }
};

}

namespace gol
{

using Cell = Position;

class Game
{
public:
    using Citizens = std::unordered_set<Cell>;

    enum NextStatus
    {
        OK              = 0,
        STILL_LIFES     = 1,
        ALL_DIE         = 2,
        OUT_OF_RANGE    = 3,
        GAME_OVER       = 4,
    };

    enum State
    {
        INIT = 0,
        PLAY = 1,
        STOP = 2,
    };

private:
    Citizens m_citizens;
    std::vector<Cell> m_generation_died_list;
    std::vector<Cell> m_generation_birth_list;
    State m_state;

public:

    Game(const Citizens& citizens) :
        m_citizens(citizens),
        m_state(INIT)
    {}

    Game(Citizens&& citizens) :
        m_citizens(std::forward<Citizens>(citizens)),
        m_state(INIT)
    {}

    NextStatus next()
    {
        if (m_state == State::STOP) {
            return NextStatus::GAME_OVER;
        }
        m_state = State::PLAY;

        const auto status = next_generation();
        if (status != NextStatus::OK) {
            m_state = State::STOP;
            return status;
        }
        return NextStatus::OK;
    }

    State state() const
    {
        return m_state;
    }

    uint64_t play(uint64_t number_iterations)
    {
        auto i = uint64_t(0);
        for(; i < number_iterations && next(); ++i) {
        }
        return i;
    }

    const Citizens& citizens() const
    {
        return m_citizens;
    }

private:

    static std::array<Cell, 8> get_around_cells(const Cell& cell)
    {
        return {{
            {cell.x - 1, cell.y + 1}, {cell.x, cell.y + 1}, {cell.x + 1, cell.y + 1},
            {cell.x - 1, cell.y    },                       {cell.x + 1, cell.y    },
            {cell.x - 1, cell.y -1 }, {cell.x, cell.y - 1}, {cell.x + 1, cell.y -1 }
        }};
    }

    size_t count_neighbors(const Cell& cell) const
    {
        const auto around_cells = get_around_cells(cell);
        return std::count_if(around_cells.begin(), around_cells.end(), [this](const auto& cell) {
            return m_citizens.count(cell) != 0;
        });
    }

    bool is_birth(const Cell& cell) const
    {
        return count_neighbors(cell) == 3;
    }

    void remove_died(const std::vector<Cell>& generation_died_list)
    {
        for (const auto& cell : generation_died_list) {
            m_citizens.erase(cell);
        }
    }

    void add_birth(const std::vector<Cell>& generation_birth_list)
    {
        for (const auto& cell : generation_birth_list) {
            m_citizens.emplace(cell);
        }
    }

    NextStatus next_generation()
    {
        m_generation_died_list.clear();
        m_generation_birth_list.clear();
        for (const auto& cell : m_citizens) {

            if (cell.is_out_of_range()) {
                return NextStatus::OUT_OF_RANGE;
            }

            const auto around_cells = get_around_cells(cell);
            for (const auto& cell : around_cells) {

                if (is_birth(cell)) {
                    m_generation_birth_list.push_back(cell);
                }
            }
            const auto amount_neighbors = count_neighbors(cell);
            if (!(amount_neighbors & 2)) { // cell will save if amount of neighbors 2 or 3
                m_generation_died_list.push_back(cell);
            }
        }
        remove_died(m_generation_died_list);
        add_birth(m_generation_birth_list);

        if (m_generation_died_list.empty() && m_generation_birth_list.empty()) {
            return NextStatus::STILL_LIFES;
        } else if (m_citizens.empty()) {
            return NextStatus::ALL_DIE;
        }
        return NextStatus::OK;
    }
};

}
