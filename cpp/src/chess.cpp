#include <iostream>

typedef uint64_t U64;
typedef int32_t I32;
typedef uint32_t U32;
typedef int8_t I8;
typedef uint8_t U8;

/**** COLORS ****/

const U32 kNumColors = 2; 
enum Color : I32 {
    kWhite, kBlack
};
constexpr Color operator~(Color c) { return Color(c ^ Color::kBlack); }

/**** PIECE TYPES ****/ 

const U32 kNumTypes = 5;
enum Type : I32 {
    kPawn, kKnight, kKing, kStraight, kDiagonal
};

/**** SQUARES ****/

const U32 kNumSquares = 64;
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

const U32 kNumFiles = 8;
enum File : I32 { 
    kA, kB, kC, kD,
    kE, kF, kG, kH
};
constexpr U64 GetMask(File f) { return 0x0101010101010101ULL << f; }
constexpr File GetFile(Square s) { return (File)(s % 8); }

/**** RANKS ****/

const U32 kNumRanks = 8;
enum Rank : I32 {
    k1, k2, k3, k4,
    k5, k6, k7, k8
}; 
constexpr U64 GetMask(Rank r) { return 0xffULL << (7 - r) * 8; }
constexpr Rank GetRank(Square s) { return (Rank)(8 - (s / 8 + 1)); }

/**** DIAGONALS ****/

const U32 kNumDiagonals = 15;
enum Diagonal : I32 {
    kDiag0, kDiag1, kDiag2, kDiag3, kDiag4, 
    kDiag5, kDiag6, kDiag7, kDiag8, kDiag9,
    kDiag10, kDiag11, kDiag12, kDiag13, kDiag14,
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

const U32 kNAntidiagonals = 15;
enum Antidiagonal : I32 {
    kAnti0, kAnti1, kAnti2, kAnti3, kAnti4,
    kAnti5, kAnti6, kAnti7, kAnti8, kAnti9,
    kAnti10, kAnti11, kAnti12, kAnti13, kAnti14
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
void PrintBitboard(U64 bb) {
    std::cout << '\n';
    for (I32 r = 0; r < 8; r++) {
        for (I32 f = 0; f < 8; f++) {
            Square s = (Square)(r * 8 + f);
            if (!f) 
                std::cout << "  " << 8 - r << ' ';
            std::cout << ' ' << (GetBit(bb, s)) ? 1 : 0;
        }
        std::cout << '\n';
    }
    std::cout << "\n     a b c d e f g h\n";
    std::cout << "\nBitboard: " << bb << '\n';
}

/**** PSEUDO-RANDOM NUMBER GENERATION ****/

typedef struct State { U64 a; U64 b; U64 c; U64 d; } State;
State kRandomContext;
constexpr U64 RandomRotate(U64 a, U64 b) { return (a << b) | (a >> (64 - b)); }
U64 RandomValue(State &x) {
    U64 e = x.a - RandomRotate(x.b, 7);
    x.a = x.b ^ RandomRotate(x.c, 13);
    x.b = x.c + RandomRotate(x.d, 37);
    x.c = x.d + e;
    x.d = e + x.a;
    return x.d;
}
void RandomInit(State &x, U64 seed) {
    x.a = 0xf1ea5eed, x.b = x.c = x.d = seed;
    for (int i = 0; i < 20; i++) RandomValue(x);
}

/**** DIRECTIONS ****/

const U32 kNDirections = 16;
enum Direction : I32 {
    kNorth = 8, kEast = -1, kWest = 1, kSouth = -8,
    kNortheast = 7, kSoutheast = -9, kSouthwest= -7, kNorthwest = 9,
    kNorthwestVL = 17, kNortheastVL = 15, kNorthwestHL = 10, kNortheastHL = 6,
    kSouthwestVL = -15, kSoutheastVL = -17, kSouthwestHL = -6, kSoutheastHL = -10
};
constexpr U64 ShiftDirection(U64 bb, Direction d) {
    return (d >= 0) ? bb >> d : bb << -d;
}

/**** ATTACK TABLES ****/

U64 kPawnAttackTable[kNumColors][kNumSquares]; 
U64 MaskPawnAttacks(Color c, Square s) {
    U64 attack = 0ULL, piece = GetMask(s);
    if (piece & ~File::kA) 
        attack |= (!c) ? ShiftDirection(piece, Direction::kNorthwest) : ShiftDirection(piece, Direction::kSouthwest);
    if (piece & ~File::kH) 
        attack |= (!c) ? ShiftDirection(piece, Direction::kNortheast) : ShiftDirection(piece, Direction::kSoutheast);
    return attack;
}
void InitializePawnAttackTable() { 
    for (U8 c = 0; c < kNumColors; c++) {
        for (U8 s = 0; s < kNumSquares; s++) {
            kPawnAttackTable[c][s] = MaskPawnAttacks((Color)c, (Square)s);
        }
    }
}

U64 kKnightAttackTable[kNumSquares];
U64 MaskKnightAttacks(Square s) {
    U64 piece = GetMask(s), attack = 0ULL;
    if (piece & ~GetMask(File::kA))
        attack |= (ShiftDirection(piece, Direction::kNorthwestVL) | ShiftDirection(piece, Direction::kSouthwestVL));
    if (piece & ~(GetMask(File::kA) | GetMask(File::kB)))
        attack |= (ShiftDirection(piece, Direction::kNorthwestHL) | ShiftDirection(piece, Direction::kSouthwestHL));
    if (piece & ~GetMask(File::kH)) 
        attack |= (ShiftDirection(piece, Direction::kNortheastVL) | ShiftDirection(piece, Direction::kSoutheastVL));
    if (piece & ~(GetMask(File::kH) | GetMask(File::kG)))
        attack |= (ShiftDirection(piece, Direction::kNortheastHL) | ShiftDirection(piece, Direction::kSoutheastHL));
    return attack;
}
void InitializeKnightAttackTable() {
    for (int s = 0; s < kNumSquares; s++) 
        kKnightAttackTable[s] = MaskKnightAttacks((Square)s);
}

U64 kKingAttackTable[kNumSquares];
U64 MaskKingAttacks(Square s) {
    U64 piece = GetMask(s), attack = 0ULL;
    attack |= ShiftDirection(piece, Direction::kNorth);
    attack |= ShiftDirection(piece, Direction::kSouth);
    if (piece & ~GetMask(File::kA))
        attack |= ShiftDirection(piece, Direction::kWest) 
                | ShiftDirection(piece, Direction::kNorthwest) 
                | ShiftDirection(piece, Direction::kSouthwest);
    if (piece & ~GetMask(File::kH))
        attack |= ShiftDirection(piece, Direction::kEast) 
                | ShiftDirection(piece, Direction::kNortheast) 
                | ShiftDirection(piece, Direction::kSoutheast);
    return attack;
}
void InitializeKingAttackTable() {
    for (int s = 0; s < kNumSquares; s++) 
        kKingAttackTable[s] = MaskKingAttacks((Square)s);
}

const I32 kDiagonalRelevantBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};
U64 MaskDiagonalAttacks(Square s) {
    U64 attack = 0ULL;
    U64 edges = GetMask(File::kA) | GetMask(File::kH) | GetMask(Rank::k1) | GetMask(Rank::k8);
    attack |= (GetMask(GetDiagonal(s)) | GetMask(GetAntidiagonal(s))) & ~(edges | GetMask(s));
    return attack;
}
U64 GenerateDiagonalAttacks(Square s, U64 occupied) {
    U64 attack;
    I32 tr = s / 8, tf = s % 8;
    I32 r, f;
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

const I32 kStraightRelevantBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12, 
};
U64 MaskStraightAttacks(Square s) {
    U64 attack = 0ULL;
    attack |= GetMask(GetRank(s)) & ~(GetMask(File::kA) | GetMask(File::kH));
    attack |= GetMask(GetFile(s)) & ~(GetMask(Rank::k1) | GetMask(Rank::k8));
    attack &= ~GetMask(s);
    return attack;
} 
U64 GenerateStraightAttacks(Square s, U64 occupied) {
    U64 attack;
    I32 tr = s / 8, tf = s % 8;
    I32 r, f;
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


U64 SetOccupancy(U32 index, U64 attack_mask) {
    U64 occupancy = 0ULL;
    I32 mask_count = CountBit(attack_mask);
    for (I32 c = 0; c < mask_count; c++) {
        Square s = (Square)ForwardScanBit(attack_mask);
        attack_mask = PopBit(attack_mask, s);
        if (index & (1 << c)) 
            occupancy |= (1ULL << s);
    }
    return occupancy;
}

void InitializeAttackTables() {
    InitializePawnAttackTable();
    InitializeKnightAttackTable();
    InitializeKingAttackTable();
}

int main() {

    InitializeAttackTables();

    for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            Square s = (Square)(r * 8 + f);
            std::cout << CountBit(MaskStraightAttacks(s)) << ", ";
        }
        std::cout << '\n';
    }
    return 0;
}