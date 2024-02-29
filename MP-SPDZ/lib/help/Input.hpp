#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <functional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../Constants.hpp"
#include "Util.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace IR {

class Input {
  public:
    explicit Input(const std::string& path, const int& player_num, const int& cur_vec)
        : player_num(player_num), vec(cur_vec), cur(0) {
        int fd = open(path.c_str(), O_RDONLY);
        if (fd < 0) {
            log(Level::ERROR, "couldn 't open file \"", path, "\"");
        }

        struct stat stats;
        if (fstat(fd, &stats) == -1) {
            log(Level::ERROR, "couldn't get stats for \"", path, "\"");
        }

        size = stats.st_size;
        file = static_cast<char*>(mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0));

        close(fd);

        if (file == MAP_FAILED) {
            log(Level::ERROR, "map failed for \"", path, "\"");
        }
    }

    Input(const Input& other) = delete;
    Input(Input&& other) noexcept
        : player_num(std::move(other.player_num)), file(std::move(other.file)),
          size(std::move(other.size)), cur(std::move(other.cur)) {
        other.file = nullptr;
    }

    Input& operator=(const Input& other) = delete;
    Input& operator=(Input&& other) noexcept {
        if (&other == this)
            return *this;

        file = std::move(other.file);
        size = std::move(other.size);
        cur = std::move(other.cur);
        player_num = std::move(other.player_num);

        other.file = nullptr;
        return *this;
    }

    ~Input() {
        if (file) {
            munmap(file, size);
        }
    }

    template <class T>
    T next(std::function<T(const std::string&)> convert);

    inline int get_player() const { return player_num; };
    inline int get_vec_num() const { return vec; };

  private:
    int player_num;
    unsigned vec;
    char* file;
    size_t size;
    size_t cur;
};

template <class T>
T Input::next(std::function<T(const std::string&)> convert) {
    std::string_view content(file + cur, size - cur);
    size_t index = content.find_first_of(' ');

    if (index == std::string::npos) {
        cur = 0;
        return convert(std::string(content));
    }

    cur += index + 1;
    T res = convert(std::string(content.substr(0, index)));
    return res;
}

static std::vector<Input> inputs;

std::vector<Input>::iterator get_input_from(const int& player_num, const size_t& cur_vec) {
    auto it = inputs.begin();
    for (; it != inputs.end(); ++it) {
        if (it->get_player() == player_num && it->get_vec_num() == cur_vec)
            return it;
    }

    return it;
}

std::array<int, SIZE_VEC> next_input(const int& player_num, const int& thread_id) {
    std::array<int, SIZE_VEC> res;
    if (current_phase == PHASE_INIT || player_num != PARTY)
        return res;

    for (size_t cur = 0; cur < SIZE_VEC; cur++) {
        auto in = get_input_from(player_num, cur);
        if (in == inputs.end()) {
            inputs.emplace_back(ROOT_DIR + "/Input-P" + std::to_string(player_num) + "-" +
                                    std::to_string(thread_id) + "-" + std::to_string(cur),
                                player_num, cur);
            in = inputs.end() - 1;
        }

        int a = in->template next<int>(
            [](const std::string& s) -> int { return std::stoi(s.c_str(), nullptr, 10); });

        print("READ: %i\n", a);
        res[cur] = a;
    }
    return res;
}

float next_input_f(const int& player_num, const int& thread_id) {
    if (player_num != PARTY)
        return 1;

    auto in = get_input_from(player_num, 0);
    if (in == inputs.end()) {
        inputs.emplace_back(ROOT_DIR + "/Input-P" + std::to_string(player_num) + "-" +
                                std::to_string(thread_id) + "-0",
                            player_num, 0);
        in = inputs.end() - 1;
    }

    float a = in->template next<float>(
        [](const std::string& s) -> float { return std::stof(s.c_str(), nullptr); });

    return a;
}

uint8_t get_next_bit(const int& player_id) {
    if (player_id != PARTY)
        return 0;

    if (bit_queue.empty()) {
        auto input = next_input_f(player_id, 0);
        long in = std::round(input);

        for (size_t i = 0; i < BITLENGTH; i++) {
            bit_queue.push((in >> i) & 1);
        }
    }

    auto res = bit_queue.front();
    bit_queue.pop();
    return res;
}

} // namespace IR

#endif
