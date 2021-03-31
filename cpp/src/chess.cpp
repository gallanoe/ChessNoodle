#include <iostream>

/**** COLORS ****/

enum Color : uint8_t {
    WHITE, BLACK
};

const size_t NCOLORS = 2;
constexpr Color operator~(Color c) {
    return Color(c ^ BLACK);
}

/**** PIECE TYPES ****/ 

const size_t NTYPES = 5;
enum Type : uint8_t {
    PAWN, KNIGHT, KING, DIAGONALS, STRAIGHTS 
};

/**** SQUARES ****/

const size_t NSQUARES = 64;
enum Square : uint32_t {
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1
};

/**** FILES ****/

const size_t NFILES = 8;
enum File : uint64_t { 
    FILE_A = 72340172838076673ULL,
    FILE_B = 72340172838076673ULL << 1,
    FILE_C = 72340172838076673ULL << 2,
    FILE_D = 72340172838076673ULL << 3, 
    FILE_E = 72340172838076673ULL << 4,
    FILE_F = 72340172838076673ULL << 5,
    FILE_G = 72340172838076673ULL << 6,
    FILE_H = 72340172838076673ULL << 7
};

const size_t NRANKS = 8;
enum Rank : uint64_t {
    RANK_1 = 255ULL << 56,
    RANK_2 = 255ULL << 48,
    RANK_3 = 255ULL << 40,
    RANK_4 = 255ULL << 32,
    RANK_5 = 255ULL << 24,
    RANK_6 = 255ULL << 16,
    RANK_7 = 255ULL << 8,
    RANK_8 = 255ULL
};

/**** BITBOARD ****/

/**
  8  0 0 0 0 0 0 0 0
  7  0 0 0 0 0 0 0 0
  6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0
  2  0 0 0 0 0 0 0 0
  1  0 0 0 0 0 0 0 0

     a b c d e f g h
**/


typedef struct Bitboard {
    uint64_t bb;

    Bitboard() : bb(0) {};
    Bitboard(uint64_t bb) : bb(bb) {};

    void set_bit(uint32_t s) {
        bb |= (1ULL << s);
    }

    void set_bit(uint32_t r, uint32_t f) {
        set_bit(r * 8 + f);
    }

    bool get_bit(uint32_t s) const {
        return bb & (1ULL << s);
    }

    bool get_bit(uint32_t r, uint32_t f) const {
        return get_bit(r * 8 + f);
    }

    void pop_bit(uint32_t s) {
        bb &= (0xffffffffffffffff & ~(1ULL << s));
    }

    void pop_bit(uint32_t r, uint32_t f) { 
        pop_bit(r * 8 + f);
    }

    void print() {
        std::cout << '\n';
        for (int r = 0; r < 8; r++) {
            for (int f = 0; f < 8; f++) {
                if (!f) 
                    std::cout << "  " << 8 - r << ' ';
                std::cout << ' ' << (get_bit(r, f) ? 1 : 0);
            }
            std::cout << '\n';
        }
        std::cout << "\n     a b c d e f g h\n";
        std::cout << "\nBitboard: " << bb << '\n';
    }
} Bitboard;

/**** DIRECTIONS ****/

const size_t NDIRS = 8;
enum Direction : int {
    NORTH_RHS = 8, EAST_RHS = 1, WEST_LHS = 1, SOUTH_LHS = 8,
    NORTHEAST_RHS = 9, SOUTHEAST_LHS = 7, SOUTHWEST_LHS= 9, NORTHWEST_RHS = 7
};

/**** ATTACK TABLES ****/

Bitboard PAWN_ATTACKS[NCOLORS][NSQUARES]; 
Bitboard mask_pawn_attacks(uint8_t c, uint32_t s) {
    Bitboard piece_bb, attack_bb;
    piece_bb.set_bit(s);        
    if (c == Color::WHITE) {
        if ((piece_bb.bb >> Direction::NORTHWEST_RHS) & (~File::FILE_A)) 
            attack_bb.bb |= (piece_bb.bb >> Direction::NORTHWEST_RHS);
        if ((piece_bb.bb >> Direction::NORTHEAST_RHS) & (~File::FILE_H))
            attack_bb.bb |= (piece_bb.bb >> Direction::NORTHEAST_RHS);
    } else {
        if ((piece_bb.bb << Direction::SOUTHWEST_LHS) & (~File::FILE_A))
            attack_bb.bb |= (piece_bb.bb << Direction::SOUTHWEST_LHS);
        if ((piece_bb.bb << Direction::SOUTHEAST_LHS) & (~File::FILE_H))
            attack_bb.bb |= (piece_bb.bb << Direction::SOUTHEAST_LHS);
    }
    return attack_bb;
}
void initialize_pawn_attacks() {
    // Initialize white pawn attacks, starting from rank 2
    for (uint32_t s = Square::A2; s <= Square::H8; s++)
        PAWN_ATTACKS[Color::WHITE][s] = mask_pawn_attacks(Color::WHITE, s);

    // Initialize black pawn attacks, ending at rank 7
    for (uint32_t s = Square::A1; s <= Square::H7; s++) 
        PAWN_ATTACKS[Color::BLACK][s] = mask_pawn_attacks(Color::BLACK, s);
}


int main() {
    Bitboard pawn_attacks = mask_pawn_attacks(Color::BLACK, Square::H4);
    pawn_attacks.print();
    return 0;
}