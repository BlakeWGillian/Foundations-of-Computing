'''
Task 5: AI Player (BONUS 2 marks)

Create a function ai_player(board) that will play the game for you.

A move can be represented as a tuple containing a position and direction. For example, moving position (2, 1) to the right could be represented as follows: ((2, 1), 'r'). Your function should return a list of moves that, when executed, will eliminate all of the pieces on the board. If a sequence of moves exists that will win the game, your function should find it. If no such sequence exists, your function should return None.

Note that it is possible for there to be multiple different move sequences that can win the game from a given starting position. In this situation any such sequence will be accepted.
Example calls

print(ai_player([['A', 'A', 'B', 'B'], ['A', 'B', 'A', 'B']]))

[((1, 1), 'r')]

print(ai_player([['C', 'B', 'B', 'C'], ['C', 'A', 'D', 'A'], ['D', 'A', 'A', 'D'], ['D', 'B', 'B', 'C']]))

[((1, 2), 'r'), ((0, 1), 'r')]

'''


import math
import copy

def on_board(board, pos):
    '''
    INPUT: The board and a position
    OUTPUT: True if the position is on the board, else False
    '''
    Y = pos[0]
    X = pos[1]

    if Y in range(len(board)) and X in range(len(board[0])):
        return True
    return False

def adj_piece(board, pos):
    '''
    INPUT: The board and a position
    OUTPUT: True piece is next to one of its kind, else False
    '''
    for axis in [0, 1]:
        for dire in [-1, 1]:
            tpos = list(pos)
            # Check each axis in both directions if the piece there matches pos
            tpos[axis] = pos[axis] + dire
            if on_board(board, tpos) is True \
                and board[tpos[0]][tpos[1]] is board[pos[0]][pos[1]]:
                return True
    return False

def legal_move(board, position, direction):
    """
    IN: Game Board, Position to move, Direction to move
    OUTPUT: True if move is valid, False if not
    Checks that; Both pieces in the move are within the board
        At least one of the pieces is adjacent to one of its kind
        Neither of the pieces are "Z"
    """
    # Make sure that the original string is not changed
    board = copy.deepcopy(board)
    
    vecDirection = {"u": (-1, 0),
                 "d": (1, 0),
                 "l": (0, -1),
                 "r": (0, 1)}

    Y = position[0]
    X = position[1]

    # Calculate new X and new Y positions after move
    nY = Y + vecDirection[direction][0]
    nX = X + vecDirection[direction][1]

    # Check if pieces in the move are on the board
    if False in (on_board(board, (nY, nX)), on_board(board, (Y, X))):
        return False

    # Check if the new or old positions are blank
    if "Z" in (board[Y][X], board[nY][nX]):
        return False

    # Make the move (i.e. swap pieces at old and new positions)
    board[Y][X], board[nY][nX] = board[nY][nX], board[Y][X]

    # Check if either piece changed is adjacent to one of its kind.
    if adj_piece(board, (Y, X)) or adj_piece(board, (nY, nX)):
        return True
    return False

def position_valid(board, position):
    '''
    INPUT: The board, and a position
    OUTPUT: True if the position is on the board, else False
    '''
    if position[0] in range(len(board)) and position[1] in range(len(board[0])):
        if board[position[0]][position[1]].isupper():
            return True
    return False

def elim_four(board):
    '''
    INPUT: The Board
    OUTPUT: True if four pieces of a kind may be eliminated, else False.
        Modifies the mutable list Board to eliminate any such pieces
    '''
    #For each piece in the board
    for r in range(len(board)):
        for c in range(len(board[0])):
            # If the piece is the top left of a group of four
            if position_valid(board, (r + 1, c + 1)):
                toElim = True
                # For each piece in the group of four
                for i in [0, 1]:
                    for j in [0, 1]:
                        if board[r + i][c + j] != board[r][c] or \
                            board[r][c] == "Z":
                            # Eliminate this group of four
                            toElim = False
                if toElim == True:
                    for i in [0, 1]:
                        for j in [0, 1]:
                            board[r + i][c + j] = "Z"
                    return True
    return False

def reorder(board, below):
    '''
    INPUT: The board, if the pieces below a Z have already been reordered
    OUTPUT: False if the board was not changed.
        Modifies the mutable list Board to reorder along the X or Y axis.
        Raises all non Z pieces into the spot of Z, then moves them from right
    '''
    for r in range(len(board)):
        for c in range(len(board[0])):
            if board[r][c] == "Z":
                # For each piece below the Z
                for i in range(r, len(board)):
                    if board[i][c] != "Z" and below == True:
                        # Swap the pieces
                        board[i][c], board[r][c] = board[r][c], board[i][c]
                        break
                # For each piece to the right of the Z
                for j in range(c, len(board[0])):
                    if board[r][j] != "Z" and below == False:
                        # Swap the pieces
                        board[r][j], board[r][c] = board[r][c], board[r][j]
                        break
    return False

def make_move(board, position, direction):
    '''
    INPUT: The proposed move, and the board
    OUTPUT: The board after the move had been made
    '''
    # Make sure that the original string is not changed
    board = copy.deepcopy(board)
    
    # The change to each position required by the given direction
    positionChange = {"u": (-1, 0),
                      "d": (1, 0),
                      "l": (0, -1),
                      "r": (0, 1)}
    positionVal = positionChange[direction]
    newPosition = (position[0] + positionVal[0], position[1] + positionVal[1])
    
    # Make the move by swaping pieces at the old and new directions
    board[newPosition[0]][newPosition[1]], board[position[0]][position[1]] = \
    board[position[0]][position[1]], board[newPosition[0]][newPosition[1]]
    
    # While groups of four are being eliminated, reorder the pieces
    while elim_four(board):
        reorder(board, True)
        reorder(board, False)
    return board

# CODE NOT FROM PREVIOUS SECTIONS
def heuristic(board):
    '''
    INPUT: The board.
    OUTPUT: A float evaluation of how close a solution is. This is the sum of 
    each piece's distance from the average of its kind.
    '''
    heur = {}
    # Add the position of each piece to the entry for its type in heur
    for y in range(len(board)):
        for x, v in enumerate(board[y]):
            if v in heur:
                heur[v] += [[y, x]]
            else:
                heur[v] = [[y, x]]

    total = 0
    av = [0, 0]
    # For each piece kind in heur
    for piece in heur:
        if piece != "Z":
            # Calculate the average row position of a type
            av[0] = sum([pos[0] for pos in heur[piece]])/len(heur[piece])
            # Calculate the average column position of a type
            av[1] = sum([pos[1] for pos in heur[piece]])/len(heur[piece])
            # Add the sum of all distances of each piece from the average to total
            # Not: this way is bad, as big outliers have significant influence.
            # Still works though
            total += sum([abs(pos[0]-av[0]) for pos in heur[piece]]) +\
                sum([abs(pos[1]-av[1]) for pos in heur[piece]])
    return total

def ai_player(board, boardList = []):
    '''
    INPUT: The board, and a list of boards that shouldn't be made for the solut.
    OUTPUT: The list of moves to solve the board, else NoneTrilogy
    '''
    boardList += [board]
    pos = []
    # Add each legal move from every position on the board to pos
    for y, row in enumerate(board):
        for x, piece in enumerate(row):
            pos += [[(x, y), direction] for direction in ['u', 'l', 'd', 'r'] if\
                    legal_move(board, (x, y), direction)]
    # Sort pos by the heuristic of each board created by the moves
    pos = sorted(pos, key=lambda pos: heuristic(make_move(board, pos[0], pos[1])))
    for move in pos:
        nBoard = make_move(board, move[0], move[1])
        # If the board is the solved board
        if nBoard == [["Z" for x in row] for row in board]:
            return [tuple(move)]
        # If the board has not already been seen (to not go in circles)
        elif nBoard not in boardList:
            # Make it evaluate other options if it hits max recursion depth
            try:
                sol = ai_player(nBoard, boardList)
                # When the solution is found
                if sol != None:
                    return [tuple(move)] + sol
            except:
                pass
    return None
