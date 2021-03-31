from enum import Enum
from typing import Union, Tuple, List, Optional
import numpy as np

# class Piece(object):
    # def __init__(self) -> None:
        # self.value: np.uint8 = np.uint8()

    # def from_color_and_type(self, color: Color, piece_type: PieceType) -> None:
        # self.value = np.uint8(0 if color is Color.WHITE else 1 << 7)
        # self.value = self.value | np.uint8(piece_type.value)

    # def from_value(self, value: np.uint8) -> None:
        # if value >> 7 not in range(2) or value & np.uint8(8) in range(1,7):
            # raise ValueError('invalid piece value')
        # self.value = value

    # def from_unicode(self, char: str) -> None:
        # if char == '_':
            # return
        # conversion_table = {
            # 'P': (Color.WHITE, PieceType.PAWN),
            # 'R': (Color.WHITE, PieceType.ROOK),
            # 'N': (Color.WHITE, PieceType.KNIGHT),
            # 'B': (Color.WHITE, PieceType.BISHOP),
            # 'Q': (Color.WHITE, PieceType.QUEEN),
            # 'K': (Color.WHITE, PieceType.KING),
            # 'p': (Color.BLACK, PieceType.PAWN),
            # 'r': (Color.BLACK, PieceType.ROOK),
            # 'n': (Color.BLACK, PieceType.KNIGHT),
            # 'b': (Color.BLACK, PieceType.BISHOP),
            # 'q': (Color.BLACK, PieceType.QUEEN),
            # 'k': (Color.BLACK, PieceType.KING)
        # }
        # if char not in conversion_table:
            # raise ValueError('provided unicode is not a piece or null piece')
        # color, piece_type = conversion_table[char]
        # self.from_color_and_type(color, piece_type)

    # def get_color(self) -> Color:
        # return Color.WHITE if self.value << 7 == 0 else Color.BLACK

    # def get_type(self) -> PieceType:
        # return PieceType(self.value & np.uint8(8))

    # def __repr__(self) -> str:
        # piece_idx = self.value & np.uint8(8) - 1
        # if self.value >> 7 == 0:
            # return [' ', '♟', '♜', '♞', '♝', '♛', '♚'][piece_idx]
        # else:
            # return [' ', '♙', '♖', '♘', '♗', '♕', '♔'][piece_idx]


class Move(object):
    def __init__(self):
        self.from_square: np.uint8 = np.uint8()
        self.to_square: np.uint8 = np.uint8()
        self.promotion: Optional[PieceType] = None

# MOVE: Piece, To Square
# DISAMBIGUOUS: Piece, From Square, To Square
# CASTLING: lc, sc


WHITE_SHORT_CASTLE = 0
BLACK_SHORT_CASTLE = 1
WHITE_LONG_CASTLE = 2
BLACK_LONG_CASTLE = 3


BitBoard = np.uint64

class File(Enum):
    A = np.uint64(0x0101010101010101)
    B = np.uint64(0x0101010101010101 << 1)
    C = np.uint64(0x0101010101010101 << 2)
    D = np.uint64(0x0101010101010101 << 3)
    E = np.uint64(0x0101010101010101 << 4)
    F = np.uint64(0x0101010101010101 << 5)
    G = np.uint64(0x0101010101010101 << 6)
    H = np.uint64(0x0101010101010101 << 7)

class Rank(Enum):
    ONE   = np.uint64(0x00000000000000ff)
    TWO   = np.uint64(0x00000000000000ff << 8)
    THREE = np.uint64(0x00000000000000ff << 16)
    FOUR  = np.uint64(0x00000000000000ff << 24)
    FIVE  = np.uint64(0x00000000000000ff << 32)
    SIX   = np.uint64(0x00000000000000ff << 40)
    SEVEN = np.uint64(0x00000000000000ff << 48)
    EIGHT = np.uint64(0x00000000000000ff << 56)

# Constants
FILES = [FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H] = [0x0101010101010101 for i in range(8)]
RANKS = [RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8] = [0xff << (8*i) for i in range(8)]
DIAGONAL = 0x8040201008040201
ANTI_DIAGONAL = 0x0102040810204080
LIGHT_SQUARES = 0x55AA55AA55AA55AA
DARK_SQUARES  = 0xAA55AA55AA55AA55


Square = Enum('Square', ' '.join([chr(f+65) + str(r+1) for f in range(8) for r in range(8)]))


class Piece(IntEnum):
    PAWNS = 0
    KINGS = 1
    KNIGHTS = 2
    STRAIGHTS = 3
    DIAGONALS = 4

class Color(IntEnum):
    WHITE = 0
    BLACK = 1

class Board(object):

    # attributes
    pieces: List[BitBoard]
    color: List[BitBoard]

    def __init__(self) -> None:
        self.pieces = [np.uint64() for _ in range(5)]
        self.color = [np.uint64() for _ in range(2)]

    def get_colored_piece(self, piece: Piece, color: Color) -> BitBoard:
        return self.pieces[piece.value] & self.color[color.value]
    
    def get_piece(self, piece: Piece) -> BitBoard:
        return self.pieces[piece.value]

    def get_color(self, color: Color) -> BitBoard:
        return self.color[color.value]

    def get_white_pieces(self) -> BitBoard:
        return self.color[Color.WHITE.value]

    def get_black_pieces(self) -> BitBoard:
        return self.color[Color.BLACK.value]

    def get_occupied(self) -> BitBoard:
        return self.color[Color.WHITE.value] | self.color[Color.BLACK.value]

    def get_empty(self) -> BitBoard:
        return ~self.occupied()

    # def __repr__(self) -> str:
        # conversion_table = {
            # b'_': '_',
            # b'P': '♟',
            # b'R': '♜',
            # b'N': '♞',
            # b'B': '♝',
            # b'Q': '♛',
            # b'K': '♚',
            # b'p': '♙',
            # b'r': '♖',
            # b'n': '♘',
            # b'b': '♗',
            # b'q': '♕',
            # b'k': '♔'
        # }
        # drawn_board = np.char.chararray((8,8), unicode=True)
        # for i in range(8):
            # for j in range(8):
                # drawn_board[i,j] = conversion_table[self.board[i,j]]
        # return str(drawn_board) if self.turn == 1 else str(np.rot90(drawn_board, 2))


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

if __name__ == '__main__':
    chess = Board()