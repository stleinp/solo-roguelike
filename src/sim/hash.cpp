#include "sim/hash.hpp"

namespace sim {

namespace {

class Fnv1a {
   public:
    void bytes(std::uint64_t v, int count) {
        for (int i = 0; i < count; ++i) {
            h_ ^= (v >> (8 * i)) & 0xffU;
            h_ *= 0x100000001b3ULL;
        }
    }
    void i32(int v) { bytes(static_cast<std::uint32_t>(v), 4); }
    void u64(std::uint64_t v) { bytes(v, 8); }
    std::uint64_t value() const { return h_; }

   private:
    std::uint64_t h_ = 0xcbf29ce484222325ULL;
};

}  // namespace

std::uint64_t hash(const State& s) {
    Fnv1a f;
    f.i32(s.width);
    f.i32(s.height);
    for (Tile t : s.tiles) f.bytes(static_cast<std::uint8_t>(t), 1);
    f.i32(s.player.x);
    f.i32(s.player.y);
    f.i32(s.player_hp);
    f.i32(s.enemy.x);
    f.i32(s.enemy.y);
    f.i32(s.enemy_hp);
    f.i32(s.turn);
    for (std::uint64_t w : s.rng_state) f.u64(w);
    return f.value();
}

}  // namespace sim
