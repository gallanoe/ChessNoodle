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

    Bitboard() : bb(0) {}
    Bitboard(uint32_t s) { set_bit(s); }
    Bitboard(uint64_t bb) : bb(bb) {}

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
    NORTH_RHS = 8, EAST_LHS = 1, WEST_RHS = 1, SOUTH_LHS = 8,
    NORTHEAST_RHS = 7, SOUTHEAST_LHS = 9, SOUTHWEST_LHS= 7, NORTHWEST_RHS = 9,
    NORTHWEST_VL_RHS = 17, NORTHEAST_VL_RHS = 15, NORHTWEST_HL_RHS = 10, NORTHEAST_HL_RHS = 6,
    SOUTHWEST_VL_LHS = 15, SOUTHEAST_VL_RHS = 17, SOUTHWEST_HL_RHS = 6, SOUTHEAST_HL_RHS = 10
    // WEST_21, EAST_21, WEST_12, EAST_12
};

/**** ATTACK TABLES ****/

Bitboard PAWN_ATTACK_TABLE[NCOLORS][NSQUARES]; 
Bitboard mask_pawn_attacks(uint8_t c, uint32_t s) {
    Bitboard piece_bb, attack_bb;
    piece_bb.set_bit(s);        
    if (piece_bb.bb & ~File::FILE_A) 
        attack_bb.bb |= (!c) ? piece_bb.bb >> Direction::NORTHWEST_RHS : piece_bb.bb << Direction::SOUTHWEST_LHS;
    if (piece_bb.bb & ~File::FILE_H) 
        attack_bb.bb |= (!c) ? piece_bb.bb >> Direction::NORTHEAST_RHS : piece_bb.bb << Direction::SOUTHEAST_LHS;
    return attack_bb;
}
void initialize_pawn_attack_table() { 
    // Pawns
    for (int c = 0; c < NCOLORS; c++) {
        for (int s = 0; s < NSQUARES; s++) {
            PAWN_ATTACK_TABLE[c][s] = mask_pawn_attacks(c, s);
        }
    }
}

Bitboard KNIGHT_ATTACK_TABLE[NSQUARES];
Bitboard mask_knight_attacks(uint32_t s) {
    Bitboard piece_bb, attack_bb;
    piece_bb.set_bit(s);
    if (piece_bb.bb & ~File::FILE_A) {
        attack_bb.bb |= piece_bb.bb >> Direction::NORTHWEST_VL_RHS;
        attack_bb.bb |= piece_bb.bb << Direction::SOUTHWEST_VL_LHS;
    }
    if (piece_bb.bb & ~(File::FILE_A | File::FILE_B)) {
        attack_bb.bb |= piece_bb.bb >> Direction::NORHTWEST_HL_RHS;
        attack_bb.bb |= piece_bb.bb << Direction::SOUTHWEST_HL_RHS;
    }
    if (piece_bb.bb & ~File::FILE_H) {
        attack_bb.bb |= piece_bb.bb >> Direction::NORTHEAST_VL_RHS;
        attack_bb.bb |= piece_bb.bb << Direction::SOUTHEAST_VL_RHS;
    }
    if (piece_bb.bb & ~(File::FILE_H | File::FILE_G)) {
        attack_bb.bb |= piece_bb.bb >> Direction::NORTHEAST_HL_RHS;
        attack_bb.bb |= piece_bb.bb << Direction::SOUTHEAST_HL_RHS;
    }
    return attack_bb;
}
void initialize_knight_attack_table() {
    for (int s = 0; s < NSQUARES; s++) 
        KNIGHT_ATTACK_TABLE[s] = mask_knight_attacks(s);
}

Bitboard KING_ATTACK_TABLE[NSQUARES];
Bitboard mask_king_attacks(uint32_t s) {
    Bitboard piece_bb, attack_bb;
    piece_bb.set_bit(s);
    attack_bb.bb |= piece_bb.bb >> Direction::NORTH_RHS;
    attack_bb.bb |= piece_bb.bb << Direction::SOUTH_LHS;
    if (piece_bb.bb & ~File::FILE_A) {
        attack_bb.bb |= piece_bb.bb >> Direction::WEST_RHS;
        attack_bb.bb |= piece_bb.bb >> Direction::NORTHWEST_RHS;
        attack_bb.bb |= piece_bb.bb << Direction::SOUTHWEST_LHS;
    }
    if (piece_bb.bb & ~File::FILE_H) {
        attack_bb.bb |= piece_bb.bb << Direction::EAST_LHS;
        attack_bb.bb |= piece_bb.bb >> Direction::NORTHEAST_RHS;
        attack_bb.bb |= piece_bb.bb << Direction::SOUTHEAST_LHS;
    }
    return attack_bb;
}
void initialize_king_attack_table() {
    for (int s = 0; s < NSQUARES; s++) 
        KING_ATTACK_TABLE[s] = mask_king_attacks(s);
}

void initialize_attack_tables() {
    initialize_pawn_attack_table();
    initialize_knight_attack_table();
    initialize_king_attack_table();
}

int main() {
    initialize_attack_tables();
    for (int s = 0; s < NSQUARES; s++) 
        KING_ATTACK_TABLE[s].print();
    return 0;
}