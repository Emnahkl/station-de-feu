#pragma once
// Mini generateur de QR Code (Version 1 = 21x21, niveau de correction L, mode octet).
// Capacite : 17 caracteres maximum (largement suffisant pour un identifiant d'agent).
// Aucune dependance : C++ standard uniquement.
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

namespace SimpleQr {

inline std::vector<std::vector<bool>> encode(const std::string &text)
{
    const int N = 21, DATA_CW = 19, EC_CW = 7;
    std::string t = text.substr(0, 17);

    // 1) Flux de bits : mode octet (0100) + longueur + donnees
    std::vector<int> bits;
    auto put = [&](unsigned v, int n) { for (int i = n - 1; i >= 0; --i) bits.push_back((v >> i) & 1); };
    put(4, 4);
    put((unsigned)t.size(), 8);
    for (unsigned char c : t) put(c, 8);
    int term = std::min(4, DATA_CW * 8 - (int)bits.size());
    put(0, term);
    while (bits.size() % 8) bits.push_back(0);
    std::vector<int> cw;
    for (size_t i = 0; i < bits.size(); i += 8) {
        int b = 0;
        for (int k = 0; k < 8; ++k) b = (b << 1) | bits[i + k];
        cw.push_back(b);
    }
    for (bool alt = false; (int)cw.size() < DATA_CW; alt = !alt) cw.push_back(alt ? 0x11 : 0xEC);

    // 2) Reed-Solomon (GF(256), polynome 0x11D)
    int expT[512], logT[256];
    int x = 1;
    for (int i = 0; i < 255; ++i) { expT[i] = x; logT[x] = i; x <<= 1; if (x & 0x100) x ^= 0x11D; }
    for (int i = 255; i < 512; ++i) expT[i] = expT[i - 255];
    auto mul = [&](int a, int b) { return (a == 0 || b == 0) ? 0 : expT[logT[a] + logT[b]]; };
    std::vector<int> g{1};
    for (int i = 0; i < EC_CW; ++i) {
        std::vector<int> ng(g.size() + 1, 0);
        for (size_t j = 0; j < g.size(); ++j) { ng[j] ^= g[j]; ng[j + 1] ^= mul(g[j], expT[i]); }
        g = ng;
    }
    std::vector<int> rem(EC_CW, 0);
    for (int b : cw) {
        int f = b ^ rem[0];
        rem.erase(rem.begin()); rem.push_back(0);
        for (int k = 0; k < EC_CW; ++k) rem[k] ^= mul(g[k + 1], f);
    }
    for (int r : rem) cw.push_back(r);

    // 3) Matrice
    std::vector<std::vector<int>> m(N, std::vector<int>(N, 0));
    std::vector<std::vector<bool>> res(N, std::vector<bool>(N, false));
    auto setF = [&](int c, int r, bool dark) { m[r][c] = dark; res[r][c] = true; };

    auto finder = [&](int r0, int c0) {
        for (int dr = -1; dr <= 7; ++dr)
            for (int dc = -1; dc <= 7; ++dc) {
                int r = r0 + dr, c = c0 + dc;
                if (r < 0 || c < 0 || r >= N || c >= N) continue;
                bool in = dr >= 0 && dr <= 6 && dc >= 0 && dc <= 6;
                bool dark = in && (dr == 0 || dr == 6 || dc == 0 || dc == 6 || (dr >= 2 && dr <= 4 && dc >= 2 && dc <= 4));
                setF(c, r, dark);
            }
    };
    finder(0, 0); finder(0, N - 7); finder(N - 7, 0);
    for (int i = 8; i <= N - 9; ++i) { setF(i, 6, i % 2 == 0); setF(6, i, i % 2 == 0); }

    // Zones de format reservees
    for (int i = 0; i < 9; ++i) { res[8][i] = true; res[i][8] = true; }
    for (int i = 0; i < 8; ++i) { res[8][N - 1 - i] = true; res[N - 1 - i][8] = true; }
    setF(8, N - 8, true); // module sombre fixe

    // 4) Donnees en zigzag + masque 0
    int total = (int)cw.size() * 8, idx = 0;
    for (int right = N - 1; right >= 1; right -= 2) {
        if (right == 6) right = 5;
        for (int vert = 0; vert < N; ++vert)
            for (int j = 0; j < 2; ++j) {
                int c = right - j;
                bool upward = ((right + 1) & 2) == 0;
                int r = upward ? N - 1 - vert : vert;
                if (!res[r][c] && idx < total) {
                    int bit = (cw[idx >> 3] >> (7 - (idx & 7))) & 1;
                    if ((r + c) % 2 == 0) bit ^= 1;
                    m[r][c] = bit;
                    ++idx;
                }
            }
    }

    // 5) Informations de format (niveau L, masque 0)
    int data = (1 << 3) | 0;
    int rm = data;
    for (int i = 0; i < 10; ++i) rm = (rm << 1) ^ ((rm >> 9) * 0x537);
    int fb = ((data << 10) | rm) ^ 0x5412;
    auto gb = [&](int i) { return ((fb >> i) & 1) != 0; };
    for (int i = 0; i <= 5; ++i) setF(8, i, gb(i));
    setF(8, 7, gb(6)); setF(8, 8, gb(7)); setF(7, 8, gb(8));
    for (int i = 9; i < 15; ++i) setF(14 - i, 8, gb(i));
    for (int i = 0; i < 8; ++i) setF(N - 1 - i, 8, gb(i));
    for (int i = 8; i < 15; ++i) setF(8, N - 15 + i, gb(i));
    setF(8, N - 8, true);

    std::vector<std::vector<bool>> out(N, std::vector<bool>(N, false));
    for (int r = 0; r < N; ++r) for (int c = 0; c < N; ++c) out[r][c] = m[r][c] != 0;
    return out;
}

} // namespace SimpleQr
