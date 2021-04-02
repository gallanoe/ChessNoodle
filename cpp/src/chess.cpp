#include <iostream>

typedef uint64_t U64;
typedef int32_t I32;
typedef uint32_t U32;
typedef int8_t I8;
typedef uint8_t U8;

/**** COLORS ****/

#define NUM_COLORS 2 
enum Color : I32 {
    kWhite, kBlack
};
constexpr Color operator~(Color c) { return Color(c ^ kBlack); }

/**** PIECE TYPES ****/ 

#define NUM_TYPES 5
enum Type : I32 {
    kPawn, kKnight, kKing, kStraight, kDiagonal
};

/**** SQUARES ****/

#define NUM_SQUARES 64
enum Square : I32 {
    kA8, kB8, kC8, kD8, kE8, kF8, kG8, kH8,
    kA7, kB7, kC7, kD7, kE7, kF7, kG7, kH7,
    kA6, kB6, kC6, kD6, kE6, kF6, kG6, kH6,
    kA5, kB5, kC5, kD5, kE5, kF5, kG5, kH5,
    kA4, kB4, kC4, kD4, kE4, kF4, kG4, kH4,
    kA3, kB3, kC3, kD3, kE3, kF3, kG3, kH3,
    kA2, kB2, kC2, kD2, kE2, kF2, kG2, kH2,
    kA1, kB1, kC1, kD1, kE1, kF1, kG1, kH1
};
constexpr U64 GetMask(Square s) { return 1ULL << s; }
const char* kSquareNames[] = {
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
    "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
    "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1"
};

/**** FILES ****/

#define NUM_FILES 8
enum File : I32 { 
    kFileA, kFileB, kFile, kFileD,
    kFileE, kFileF, kFileG, kFileH
};
constexpr U64 GetMask(File f) { return 0x0101010101010101ULL << f; }
constexpr File GetFile(Square s) { return (File)(s % 8); }

/**** RANKS ****/

#define NUM_RANKS 8
enum Rank : I32 {
    kRank1, kRank2, kRank3, kRank4,
    kRank5, kRank6, kRank7, kRank8
}; 
constexpr U64 GetMask(Rank r) { return 0xffULL << (7 - r) * 8; }
constexpr Rank GetRank(Square s) { return (Rank)(8 - (s / 8) + 1); }

/**** DIAGONALS ****/

#define NUM_DIAGONALS 15
enum Diagonal : I32 {
    kDiagonal0, kDiagonal1, kDiagonal2, kDiagonal3, kDiagonal4, 
    kDiagonal5, kDiagonal6, kDiagonal7, kDiagonal8, kDiagonal9,
    kDiagonal10, kDiagonal11, kDiagonal12, kDiagonal13, kDiagonal14,
};
constexpr U64 GetMask(Diagonal d) { 
    U64 masks[] = { 0x8000000000000000, 0x4080000000000000,
                    0x2040800000000000, 0x1020408000000000,
                    0x810204080000000, 0x408102040800000, 
                    0x204081020408000, 0x102040810204080, 
                    0x1020408102040, 0x10204081020, 
                    0x102040810, 0x1020408,
                    0x10204, 0x102, 0x1 };
    return masks[d];
}
constexpr Diagonal GetDiagonal(Square s) { return (Diagonal)(7 + GetRank(s) - GetFile(s)); }

/**** ANTIDIAGONAL-DIAGONALS ****/

#define NUM_ANTIDIAGONALS 15
enum Antidiagonal : I32 {
    kAntidiagonal0, kAntidiagonal1, kAntidiagonal2, kAntidiagonal3, kAntidiagonal4,
    kAntidiagonal5, kAntidiagonal6, kAntidiagonal7, kAntidiagonal8, kAntidiagonal9,
    kAntidiagonal10, kAntidiagonal11, kAntidiagonal12, kAntidiagonal13, kAntidiagonal14
};
constexpr U64 GetMask(Antidiagonal a) {
    U64 masks[] = { 0x100000000000000, 0x201000000000000,
                    0x402010000000000, 0x804020100000000,
                    0x1008040201000000, 0x2010080402010000,
                    0x4020100804020100, 0x8040201008040201,
                    0x80402010080402, 0x804020100804,
                    0x8040201008, 0x80402010, 
                    0x804020, 0x8040, 0x80 };
    return masks[a];
}
constexpr Antidiagonal GetAntidiagonal(Square s) { return (Antidiagonal)(GetRank(s) + GetFile(s)); }


/**** BITBOARD AND BIT MANIPULATION ****/

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
constexpr bool GetBit(U64 bb, Square s) { return bb & GetMask(s); }
constexpr U64 SetBit(U64 bb, Square s) { return bb | GetMask(s); }
constexpr U64 PopBit(U64 bb, Square s) { return bb & ~GetMask(s); }
constexpr U32 CountBit(U64 x) {
    x = (x & 0x5555555555555555ULL) + ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x & 0x0f0f0f0f0f0f0f0fULL) + ((x >> 4) & 0x0f0f0f0f0f0f0f0fULL);
    x = (x & 0x00ff00ff00ff00ffULL) + ((x >> 8) & 0x00ff00ff00ff00ffULL);
    x = (x & 0x0000ffff0000ffffULL) + ((x >> 16) & 0x0000ffff0000ffffULL);
    x = (x & 0x00000000ffffffffULL) + ((x >> 32) & 0x00000000ffffffffULL);
    return (U32)x;
}
constexpr I32 ForwardScanBit(U64 x) { return (!x) ? 0 : CountBit((x & -x) - 1); }
void print_bitboard(U64 bb) {
    std::cout << '\n';
    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            Square s = r * 8 + f;
            if (!f) 
                std::cout << "  " << 8 - r << ' ';
            std::cout << ' ' << (GetBit(bb, s)) ? 1 : 0;
        }
        std::cout << '\n';
    }
    std::cout << "\n     a b c d e f g h\n";
    std::cout << "\nBitboard: " << bb << '\n';
}

/**** DIRECTIONS ****/

#define NDIRS 8
enum Direction : I8 {
    kNorth = 8, kEast = -1, kWest = 1, kSouth = -8,
    kNortheast = 7, kSoutheast = -9, kSouthwest= -7, kNorthwest = 9,
    kNorthwestVL = 17, kNortheastVL = 15, kNorthwestHL = 10, kNortheastHL = 6,
    kSouthwestVL = -15, kSoutheastVL = -17, kSouthwestHL = -6, kSoutheastHL = -10
};
constexpr U64 ShiftDirection(U64 bb, Direction d) {
    return (d >= 0) ? bb >> d : bb << -d;
}

/**** ATTACK TABLES ****/

U64 PAWN_ATTACK_TABLE[NUM_COLORS][NUM_SQUARES]; 
U64 MaskPawnAttacks(Color c, Square s) {
    U64 attack = 0ULL, piece = GetMask(s);
    if (piece & ~File::kFileA) 
        attack |= (!c) ? ShiftDirection(piece, Direction::kNorthwest) : ShiftDirection(piece, Direction::kSouthwest);
    if (piece & ~File::kFileH) 
        attack |= (!c) ? ShiftDirection(piece, Direction::kNortheast) : ShiftDirection(piece, Direction::kSoutheast);
    return attack;
}
void InitializePawnAttackTable() { 
    for (U8 c = 0; c < NUM_COLORS; c++) {
        for (U8 s = 0; s < NUM_SQUARES; s++) {
            PAWN_ATTACK_TABLE[c][s] = MaskPawnAttacks(s, c);
        }
    }
}

U64 KNIGHT_ATTACK_TABLE[NUM_SQUARES];
U64 MaskKnightAttacks(Square s) {
    U64 piece = GetMask(s), attack = 0ULL;
    if (piece & ~GetMask(File::kFileA))
        attack |= (ShiftDirection(piece, Direction::kNorthwestVL) | ShiftDirection(piece, Direction::kSouthwestVL));
    if (piece & ~(GetMask(File::kFileA) | GetMask(File::kFileB)))
        attack |= (ShiftDirection(piece, Direction::kNorthwestHL) | ShiftDirection(piece, Direction::kSouthwestHL));
    if (piece & ~GetMask(File::kFileH)) 
        attack |= (ShiftDirection(piece, Direction::kNortheastVL) | ShiftDirection(piece, Direction::kSoutheastVL));
    if (piece & ~(GetMask(File::kFileH) | GetMask(File::kFileG)))
        attack |= (ShiftDirection(piece, Direction::kNortheastHL) | ShiftDirection(piece, Direction::kSoutheastHL));
    return attack;
}
void InitializeKnightAttackTable() {
    for (int s = 0; s < NUM_SQUARES; s++) 
        KNIGHT_ATTACK_TABLE[s] = MaskKnightAttacks((Square)s);
}

U64 KING_ATTACK_TABLE[NUM_SQUARES];
U64 MaskKingAttacks(Square s) {
    U64 piece = GetMask(s), attack = 0ULL;
    attack |= ShiftDirection(piece, Direction::kNorth);
    attack |= ShiftDirection(piece, Direction::kSouth);
    if (piece & ~GetMask(File::kFileA))
        attack |= ShiftDirection(piece, Direction::kWest) 
                | ShiftDirection(piece, Direction::kNorthwest) 
                | ShiftDirection(piece, Direction::kSouthwest);
    if (piece & ~GetMask(File::kFileH))
        attack |= ShiftDirection(piece, Direction::kEast) 
                | ShiftDirection(piece, Direction::kNortheast) 
                | ShiftDirection(piece, Direction::kSoutheast);
    return attack;
}
void InitializeKingAttackTable() {
    for (int s = 0; s < NUM_SQUARES; s++) 
        KING_ATTACK_TABLE[s] = MaskKingAttacks((Square)s);
}


U64 MaskDiagonalAttacks(Square s) {
    U64 attack = 0ULL;
    U64 edges = GetMask(File::kFileA) | GetMask(File::kFileH)
              | GetMask(Rank::kRank1) | GetMask(Rank::kRank8);
    attack |= (GetMask(GetDiagonal(s)) | GetMask(GetAntidiagonal(s))) & ~(edges | GetMask(s));
    return attack;
}
U64 GenerateDiagonalAttacks(Square s, U64 occupied) {
    U64 attack;
    int tr = s / 8, tf = s % 8;
    int r, f;
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attack |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & occupied) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attack |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & occupied) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attack |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & occupied) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attack |= (1ULL << (r * 8 + f));
        if (1ULL << (r * 8 + f) & occupied) break;
    }
    return attack;
}


U64 MaskStraightAttacks(Square s) {
    U64 attack = 0ULL;
    attack |= GetMask(GetRank(s)) & ~(GetMask(File::kFileA) | GetMask(File::kFileH));
    attack |= GetMask(GetFile(s)) & ~(GetMask(Rank::kRank1) | GetMask(Rank::kRank8));
    attack &= ~GetMask(s);
    return attack;
} 
U64 GenerateStraightAttacks(Square s, U64 occupied) {
    U64 attack;
    int tr = s / 8, tf = s % 8;
    int r, f;
    for (r = tr + 1; r <= 7; r++) {
        attack |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & occupied) break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attack |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & occupied) break;
    }
    for (f = tf + 1; f <= 7; f++) {
        attack |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & occupied) break;
    }
    for (f = tf - 1; f >= 0; f--) {
        attack |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & occupied) break;
    }
    return attack;
}

U64 SetOccupancy(U32 index, U32 count, U64 attack_mask) {
    U64 occupancy = 0ULL;
    return occupancy;
}

void InitializeAttackTables() {
    InitializePawnAttackTable();
    InitializeKnightAttackTable();
    InitializeKingAttackTable();
}

int main() {
    // initialize_attack_tables();

    // Bitboard board;
    // for (int i=0; i<8; i++) {
        // for (int j=0; j<8; j++) {
            // int s = i * 8 + j;
            // board = mask_diagonal_attacks(s);
            // board.print();
        // }
    // }

    // U64 board = GetMask(File::kFileF) | GetMask(Rank::kRank3);
    U64 board = GetMask(Square::kD4) | GetMask(Square::kB2);
    print_bitboard(board);
    std::cout << "Count: " << CountBit(board) << '\n';
    std::cout << "Least significant bit: " << kSquareNames[ForwardScanBit(board)] << '\n';
    return 0;
}