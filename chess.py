from enum import Enum
from typing import Union, Tuple, List
import numpy as np

# 8-bit schematic for piece representation
# COLOR | POINTLESS INFO | PIECE
# 0 | 0 0 0 0 | 0 0 0 

class Color(Enum):
    WHITE = 0 
    BLACK = 1

class Type(Enum):
    PAWN = 1
    ROOK = 2
    KNIGHT = 3
    BISHOP = 4
    QUEEN = 5
    KING = 6

class Piece(object):
    def __init__(self, color: Color, type: Type) -> None:
        self.value: np.uint8 = np.uint8(0 if color is Color.WHITE else 1 << 7)     
        self.value = self.value | np.uint8(type.value)

    def __repr__(self) -> str:
        piece_idx = self.value & np.uint8(8) - 1
        if self.value >> 7 == 0:
            return [' ', '♟', '♜', '♞', '♝', '♛', '♚'][piece_idx]
        else:
            return [' ', '♙', '♖', '♘', '♗', '♕', '♔'][piece_idx]
        


class Square(object):
    def __init__(self, col: str, row: int) -> None:
        # self.idx: Tuple[np.uint8, np.uint8] = (np.uint8(8-row), np.uint8())
        if col not in 'abcdefgh' or row not in range(1,9):
            raise ValueError('invalid square specified')
        self.value: str = col + str(row)    
    
    def __repr__(self) -> str:
        return self.value

class Action(object):
    def __init__(self, piece: Piece, x: np.uint8, y: np.uint8) -> None:
        pass

class Chess(object):
    def __init__(self) -> None:
        # set up the board
        self.turn: Color = Color.WHITE
        self.board: np.char.chararray = np.char.chararray((8,8), unicode=True)
        self.board[:] = '_'
        self.board[1,:] = '♟'
        self.board[0] = ['♜', '♞', '♝', '♛', '♚', '♝', '♞', '♜']
        self.board[6,:] = '♙'
        self.board[7] = ['♖', '♘', '♗', '♕', '♔', '♗', '♘', '♖']
            
    def validActions(self) -> List[str]:
        pass
    
    def action(self, action: Union[Action,str], player: Color) -> None:
        pass
        

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
    chess = Chess()
    print(chess.board)