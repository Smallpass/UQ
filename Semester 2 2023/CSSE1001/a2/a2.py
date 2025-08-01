from a2_support import *



class Tile():
    """ 
    A class to a represent a Tiles attributes.
    ...
    Attributes:
     - Blocking
     - Type
    """

    def __init__(self):
        """
        Passes no initial conditions for Tile.
        """
        pass
        
    def is_blocking(self) -> bool:
        """
        Returns Tiles as non-blocking.
        """
        return False

    def get_type(self) -> str:
        """
        Returns Tile type.
        """
        return str('Abstract Tile')
    
    def __str__(self) -> str: 
        """
        Returns get_type.
        """
        return self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()


class Floor(Tile):
    """ 
    A class to a represent a Floors attributes.
    ...
    Attributes:
     - Blocking
     - Type
    """

    def __init__(self):
        """
        Passes no initial conditions for Floor.
        """
        pass
        
    def is_blocking(self) -> bool:
        """
        Returns Floors as non-blocking.
        """
        return False

    def get_type(self) -> str:
        """
        Returns the type of Floor.
        """
        return str(' ')
    
    def __str__(self) -> str: 
        """
        Returns get_type.
        """
        return self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()


class Wall(Tile):
    """ 
    A class to a represent a Walls attributes.
    ...
    Attributes:
     - Blocking
     - Type
    """

    def __init__(self):
        """
        Passes no initial conditions for Wall.
        """
        pass
        
    def is_blocking(self) -> bool:
        """
        Returns Walls as blocking.
        """
        return True

    def get_type(self) -> str:
        """
        Returns type of wall.
        """
        return str('W')
    
    def __str__(self) -> str: 
        """
        Returns get_type.
        """
        return self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()


class Goal(Tile):
    """ 
    A class to a represent a Goals attributes.
    ...
    Attributes:
     - Initial conditions
     - Blocking
     - Type
     - Filled/not Filled
    """

    def __init__(self):
        """
        Defines initial state of Goal as unfilled.
        """
        self.is_filled_initial  = False
        pass

    def is_blocking(self) -> bool:
        """
        Returns True if blocking, False otherwise.
        """
        return self.is_filled_initial

    def is_filled(self) -> bool:
        """
        Returns a bool of if the Goal is filled or not.
        """
        return self.is_filled_initial

    def fill(self) -> None:
        """
        Sets the goal initial value to True and returns None.
        """
        self.is_filled_initial = True
        return None

    def get_type(self) -> str:
        """
        Returns the Goals type.
        """
        return str('G')
    
    def __str__(self) -> str: 
        """
        Returns the Goals type 'G' if empty and 'X' if filled.
        """
        return str('X') if self.is_filled() else self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns the Goals type 'G' if empty and 'X' if filled.
        """
        return str('X') if self.is_filled() else self.get_type()


class Entity():
    """
    Class to represent a Entities attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
    """

    def __init__(self):
        """
        Passes initial conditions for Entity.
        """
        self.is_movable_initial = False
        pass

    def get_type(self) -> str:
        """
        Returns the type of Entity.
        """
        return str('Abstract Entity')
    
    def is_movable(self) -> bool:
        """
        Returns a bool for if Entity is movable or not.
        """
        return self.is_movable_initial

    def __str__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()


class Crate(Entity):
    """
    Class to represent a Crates attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
     - Strength
    """

    def __init__(self, strength: int) -> None:
        """
        Passes initial conditions for Crate.

        Parameters:
            Strength: An integer value for entity strength.
        """
        self.is_movable_initial = True
        self.strength_value = strength
        pass

    def get_strength(self) -> int:
        """
        Returns str for strength value.
        """
        return int(self.strength_value)

    def get_type(self) -> str:
        """
        Returns the type of Crate.
        """
        return str('C')
    
    def is_movable(self) -> bool:
        """
        Returns a bool for if Crate is movable or not.
        """
        return self.is_movable_initial

    def __str__(self) -> str:
        """
        Returns strength_value.
        """
        return str(self.strength_value)
    
    def __repr__(self) -> str:
        """
        Returns strength_value.
        """
        return str(self.strength_value)


class Potion(Entity):
    """
    Class to represent a Potion attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
     - Effect
    """

    def __init__(self):
        """
        Passes initial conditions for Potion.
        """
        self.is_movable_initial = False
        pass

    def effect(self) -> dict[str, int]:
        """
        Returns a dictionary with a string and integer.
        """
        return {}

    def get_type(self) -> str:
        """
        Returns the type of Potion.
        """
        return str('Potion')
    
    def is_movable(self) -> bool:
        """
        Returns a bool for if Potion is movable or not.
        """
        return self.is_movable_initial

    def __str__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()


class StrengthPotion(Potion):
    """
    Class to represent a Strength Potion attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
     - Effect
    """

    def effect(self) -> dict[str, int]:
        """
        Returns the change in stength and move 
        number for a strength potion.
        """
        return {'strength': 2}

    def get_type(self) -> str:
        """
        Returns the type of Strength Potion.
        """
        return str('S')
    

class MovePotion(Potion):
    """
    Class to represent a Move Potion attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
     - Effect
    """

    def effect(self) -> dict[str, int]:
        """
        Returns the change in stength and move 
        number for a move potion.
        """
        return {'moves': 5}

    def get_type(self) -> str:
        """
        Returns the type of Move Potion.
        """
        return str('M')


class FancyPotion(Potion):
    """
    Class to represent a Fancy Potion attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
     - Effect
    """

    def effect(self) -> dict[str, int]:
        """
        Returns the change in stength and move 
        number for a fancy potion.
        """
        return {'strength': 2, 'moves': 2}

    def get_type(self) -> str:
        """
        Returns the type of Fancy Potion.
        """
        return str('F')


class Player(Entity):
    """
    Class to represent a Entities attributes.
    ...
    Attributes:
     - Initial conditions
     - Type
     - Movability
     - Strength
     - Number of moves
    """

    def __init__(self, start_strength: int, moves_remaining: int):
        """
        Passes initial conditions for Player.
        
        Parameters:
            start_strength: An integer for starting strength value.
            moves_remaining: An integer for starting number of moves.
        """
        self.initial_strength = start_strength
        self.initial_moves = moves_remaining
        self.is_movable_initial = True
        pass

    def get_strength(self) -> int:
        """
        Returns current strength of player.
        """
        return self.initial_strength
    
    def add_strength(self, amount: int) -> None:
        """
        Adds amount to self.initial_strength then returns None.

        Parameters:
            Amount: Integer value for amount of strength added. 
        """
        self.initial_strength += amount
        return None
    
    def get_moves_remaining(self) -> int:
        """
        Returns the number of moves remaining.
        """
        return self.initial_moves
    
    def add_moves_remaining(self, amount: int) -> None:
        """
        Adds amount to self.initial_moves then returns None.

        Parameters:
            Amount: Integer value for amount of strength added. 
        """
        self.initial_moves += amount
        return None
    
    def apply_effect(self, potion_effect: dict[str, int])-> None:
        """
        A function to apply a potions effect.
        
        Parameters:
            potion_effect: a dictionary for the characteristics of the potion.
            
        """
        for key, value in potion_effect.items():
            if key == 'moves':
                self.initial_moves += value
            elif key == 'strength':
                self.initial_strength += value

    def get_type(self) -> str:
        """
        Returns the type of Player.
        """
        return str('P')
    
    def is_movable(self) -> bool:
        """
        Returns a bool for if Player is movable or not.
        """
        return self.initial_moves > 0

    def __str__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()
    
    def __repr__(self) -> str:
        """
        Returns get_type.
        """
        return self.get_type()



def convert_maze(game: list[list[str]]) -> tuple[Grid, Entities, Position]:
    """
    A function to transcribe game txt files to usable data,
    namely the Grid, Entities and Positions.
    
    Parameters:
         - game: The game txt file.
    """
    tiles = []
    entities = {}
    player_position = None

    for i, row in enumerate(game):
        tiles_row = []
        for j, cell in enumerate(row):
            if cell == 'W':
                tiles_row.append(Wall())
            elif cell == 'G':
                tiles_row.append(Goal())
            elif cell == ' ':
                tiles_row.append(Floor())
            elif cell == 'P':
                player_position = (i, j)
                tiles_row.append(Floor())
            else:
                entities[(i, j)] = Crate(int(cell))
                tiles_row.append(Floor())
        tiles.append(tiles_row)

    return tiles, entities, player_position



class SobokanModel():
    """
    A class to represent the Sobokan game and its attributes.
    ...
    Attributes:
     - 
    """
    def __init__(self, maze_file: str) -> None:
        game = read_file(maze_file)
        self.initial_game = convert_maze(game)
        self.player = Player(self.initial_game[2][0], self.initial_game[2][1])
        self.crates = len(self.get_entities())
        
    def get_maze(self) -> Grid:
        return self.initial_game[0]

    def get_entities(self) -> Entities:
        return self.initial_game[1]
    
    def get_player_position(self) -> tuple[int, int]:
        return self.initial_game[2]
    
    def get_player_moves_remaining(self) -> int:
        return self.player.get_moves_remaining()
    
    def get_player_strength(self) -> int:
        return self.player.get_strength()