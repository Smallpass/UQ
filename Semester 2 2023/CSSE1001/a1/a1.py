""" A fancy tic-tac-toe game for CSSE1001/7030 A1. """
from constants import *

Board = list[list[str]]
Pieces = list[int]
Move = tuple[int, int, int]


# 4.1 number of hours
def num_hours():
    '''
    This function returns the number of hours spent on the assignment.
    
    :returns: hours spent
    '''

    hours = 10.0
    #returns hours
    return hours


#4.2 generate_initial_pieces
def generate_initial_pieces(num_pieces: int)-> Pieces:
    '''
    This function returns a list of initial pieces from 1 to input num_pieces.
    
    :param num_pieces: This is the number of pieces initially wanted
    :return: The list from 1 to num_pieces
    '''
    pieces_avalible = list(range(1, num_pieces + 1))
    #returns a range of 1 to input num_pieces as a list
    return pieces_avalible


#4.3 initial_state
def initial_state() -> Board:
    '''
    This function sets the board to its initial state of EMPTY.
    
    :return: A board in initial state of 'EMPTY'
    '''
    #sets the board to an empty size 3 list of 3 lists
    board = [['  ','  ','  '],['  ','  ','  '],['  ','  ','  ']]
    return board


#4.4 place_piece
def place_piece(board: Board, player: str,
    pieces_available: Pieces, move: Move)->None:
    '''
    This function places a players piece onto the board using a given move
    in the form of a list, row, column, piece_size, and then removes
    the piece from pieces available.

    :param board: This is the updated board set prior to running the function
    :param player: This is the players whos turn it is, NAUGHT or CROSS
    :param pieces: This is the pieces avalible for the given player,
                    naught_pieces or cross_pieces
    :param move: This is the move played by the player, row, column, piece_size
    :return: None
    '''
    (row, column, piece_size) = move
    #sets NAUGHT to symbol 'O'
    if player == NAUGHT:
        symbol = 'O'
    #sets CROSS to symbol 'X'
    if player == CROSS:
        symbol = 'X'
    #Places piece on board
    board[row][column] = f"{symbol}{piece_size}"
    #removes pieces from available pieces
    pieces_available.remove(piece_size)

 



#4.5 print_game
def print_game(board: Board, naught_pieces: Pieces, cross_pieces: Pieces)->None:
    '''
    Prints both number of pieces available for teams naughts
    and crosses and the updated board.
    
    :param board: This is the updated board set prior to running the
                    function
    :param naught_pieces: This is the pieces available for naughts
    :param cross_pieces: This is the pieces available for crosses
    :return: None
    '''
    EMPTY = ' '
    #prints the list of naught pieces
    print("O has:", end=' ')
    print(*naught_pieces, sep=', ', end='\n')
    #prints the list of cross pieces
    print("X has:", end=' ')
    print(*cross_pieces, sep=', ', end='\n')
    #sets the rows as the boards idexes then prints the board
    row_1 = board[0]
    row_2 = board[1]
    row_3 = board[2]
    print("\n   1  2  3")
    print("  ---------")
    print(f"1|{'|'.join(row_1)}|")
    print("  ---------")
    print(f"2|{'|'.join(row_2)}|")
    print("  ---------")
    print(f"3|{'|'.join(row_3)}|")
    print("  ---------")


#4.6 process_move 
def process_move(move: str)->Move|None:
    '''
    This function processes the given move to ensure that its
    rows, column, piece_size and format are interpretable in other
    functions.

    :param move: This is any string given to the function
    :returns: The function either returns the move if it has satisfies
                the tests with row and column indexed to 0, if not it returns
                None
    '''
    #sets invalid messages
    parts = move.split()
    #checks for invalid format
    if len(parts) != 3:
        print(INVALID_FORMAT_MESSAGE)
        return None

    row_str, column_str, piece_size_str = parts
    #checks for invalid format
    if (sum(not item.isdigit() and len(item) != 1
        for item in (row_str, column_str, piece_size_str)) >= 2):
            print(INVALID_FORMAT_MESSAGE)
            return None
    #checks for invalid row
    if not row_str.isdigit() or len(row_str) != 1:
        print(INVALID_ROW_MESSAGE)
        return None
    #checks for invalid column
    if not column_str.isdigit() or len(column_str) != 1:
        print(INVALID_COLUMN_MESSAGE)
        return None
    #checks for invalid size
    if not piece_size_str.isdigit() or len(piece_size_str) != 1:
        print(INVALID_SIZE_MESSAGE)
        return None

    row, column, piece_size = map(int, parts)
    #checks for general invalid row 
    if not (1 <= row <= 3):
        print(INVALID_ROW_MESSAGE)
        return None
    #checks for general invalid column
    if not (1 <= column <= 3):
        print(INVALID_COLUMN_MESSAGE)
        return None
    #checks for general invalid size
    if not (1 <= piece_size):
        print(INVALID_SIZE_MESSAGE)
        return None
    #Converts move to 0-indexed row and column
    return (row - 1, column - 1, piece_size)
    #Convert to 0-indexed row and column



#4.7 get_player_move
def get_player_move()-> Move:
    '''
    This function requests a players move and then passes it through process_
    move to ensure it satisfies the move formats. If 'H' or 'h' is pressed, it
    will return a help message.

    :returns: If no errors arise in process_move(), the resultant move is
                returned. Otherwise, the error message from process_move
                is returned, and get_player_move() is returned again

    '''
    #sets invalid messages
    HELP_MESSAGE ='Enter a row, column & piece size in the format: row col size'
    move = input('Enter your move: ')
    #prints ERROR_MESSAGE if input is h
    if move.lower() == 'h':
        print(HELP_MESSAGE)
        return get_player_move()
    #call process_move to check validity of move
    result = process_move(move)
    #returns get_player_move to ask for new move
    if result == None:
            return get_player_move()
    #returns the move if valid
    return result


#4.8 check_move
def check_move(board: Board, pieces_available: Pieces, move: Move)->bool:
    '''
    This function checks to ensure that the piece_size the player is placing is
    not smaller than a piece already on the board.

    :param board: This is the updated board set prior to running the function
    :param pieces_available: This parameter is the list of pieces available for
                                a given player
    :param move: This is the move list given in row, column, piece_size
    :returns: True if the move is legal, and False otherwise

    '''
    row, column, piece_size = move
    #checks to see that the board has a smaller piece or empty place.
    if 1 <= piece_size <= max(pieces_available):
        if (board[row][column] == '  ' or
        int(board[row][column][1]) < piece_size):
            return True
    #if not returns False
    else: False


#4.9 check_wind
def check_win(board: Board)-> str|None:
    '''
    This function checks the boards rows, columns and diagonals to check for a
    winnner, three in a row.

    :param board: The updated board
    :returns: X or O for a cross or naught win respectively, None if there is no
                win present
    '''
    for i in range(3):
        #checks rows for winner
        if (board[i][0][0] == board[i][1][0] == board[i][2][0] and
        board[i][0][0] != ' '):
            return 'O' if board[i][0][0] == 'O' else 'X'
        #checks columns for winner
        if (board[0][i][0] == board[1][i][0] == board[2][i][0] and
        board[0][i][0] != ' '):
            return 'O' if board[0][i][0] == 'O' else 'X'
    #checks diagonals for winner
    if (board[0][0][0] == board[1][1][0] == board[2][2][0] and
    board[0][0][0] != ' '):
        return 'O' if board[0][0][0] == 'O' else 'X'
    if (board[0][2][0] == board[1][1][0] == board[2][0][0] and
    board[0][2][0] != ' '):
        return 'O' if board[0][2][0] == 'O' else 'X'
    #returns None if no winner
    return None


#4.10 check_stalemate
def check_stalemate(board: Board, naught_pieces: Pieces, cross_pieces:Pieces)->bool:
    '''
    This function checks the boards for stalemates, a point where no moves
    can be made to continue the game but no winner is present.

    :param board: The updated board
    :param naught_pieces: The list of naught_pieces available
    :param cross_pieces: the list of cross_pieces available
    :returns: False if a move can be made (no stalemate) and True otherwise
    '''
    #checks for gap in the board
    for row in board:
        if '  ' in row:
            return False
    #checks for smaller pieces on board
    for row in board:
        for column in row:
            if column != '  ':
                size = int(column[1])
                for piece in naught_pieces + cross_pieces:  
                    if piece > size:
                        return False
    
    if not naught_pieces or not cross_pieces:
        return False
    #returns true if no stalemate 
    return True



def main()->None:
    '''
    This function is run as soon as the file is opened and loops until the game
    is no longer wished to be played. It combines all previous functions to make
    the game user friendly.

    :returns: Returns main() if the game is wished to be played again or None if
                the game is wanted to be terminated
    '''
    #defines initial states
    board = initial_state()
    generate_initial_pieces(PIECES_PER_PLAYER)
    naught_pieces = generate_initial_pieces(PIECES_PER_PLAYER)
    cross_pieces = generate_initial_pieces(PIECES_PER_PLAYER)
    print_game(board, naught_pieces, cross_pieces)
    #starts loop as Game=True
    Game = True
    while Game == True:
        #checks for stalemate
        if check_stalemate(board, naught_pieces, cross_pieces) == True:
            print('Stalemate!')
            response = input('Play again? ')
            #returns if not wanting to play again
            if response.lower() == 'n':
                return
            else:
                break

        #checks for winner
        if check_win(board) != None:
            winner = check_win(board)
            print(winner + ' wins!')
            response = input('Play again? ')
            #returns if not wanting to play again
            if response.lower() == 'n':
                return
            else:
                return main()
    
        
        #tests to see if it is crosses turn,
        #if it is, it runs all move validity tests
        if len(cross_pieces) > len(naught_pieces):
            print('\n'+'X turn to move' + '\n')
            move = get_player_move()
            if check_move(board, cross_pieces, move) == None:
                print(INVALID_MOVE_MESSAGE)

            else:            
                place_piece(board, CROSS, cross_pieces, move)
                print_game(board, naught_pieces, cross_pieces)
                
        #naughts turn and runs all move validity checks
        else:
            print('\n'+'O turn to move' +'\n')
            move = get_player_move()
            if check_move(board, naught_pieces, move) == None:
                print(INVALID_MOVE_MESSAGE)

            else:            
                place_piece(board, NAUGHT, naught_pieces, move)
                print_game(board, naught_pieces, cross_pieces)

    

    pass


if __name__ == '__main__':
    main()
