import tkinter as tk
from tkinter import messagebox, filedialog
from typing import Callable
from model import SokobanModel, Tile, Entity
from a2_support import *
from a3_support import *


class FancyGameView(AbstractGrid):
    def __init__(self, master: tk.Frame | tk.Tk, dimensions: tuple[int, int], size: tuple[int, int], **kwargs) -> None:
        super().__init__(master, dimensions, size, **kwargs)
        self.size = self.get_cell_size()
        self.image_cache = {}
        self.dimensions = dimensions

    def __init__(self, master: tk.Frame | tk.Tk, dimensions: tuple[int, int], size: tuple[int, int], **kwargs) -> None:
        super().__init__(master, dimensions, size, **kwargs)
        self.size = self.get_cell_size()
        self.image_cache = {}
        self.dimensions = dimensions

    def display(self, maze: Grid, entities: Entities, player_position: Position):
        self.clear()

        i_rows, i_cols = self.dimensions


        for row in range(i_rows):
            for column in range(i_cols):
                position = (row, column)
                tile = maze[row][column]

                if position == player_position:
                    entity_type = 'P'
                elif position in entities:
                    entity = entities[position]
                    entity_type = entity.get_type()

                    if entity_type in range(0,100):
                        self.annotate_position(position=position, text=entities[position], font=CRATE_FONT)
                else:
                    entity_type = None


                if tile.get_type() == FLOOR:
                    tile_image_name = 'Floor'
                elif tile.get_type() == WALL:
                    tile_image_name = tile.get_type()
                elif tile.is_filled() == True:
                    tile_image_name = 'X'
                elif tile.get_type() == GOAL:
                    tile_image_name = tile.get_type()



                tile_image = get_image(f"images/{tile_image_name}.png", 
                                    self.size, self.image_cache)
                midpoint = self.get_midpoint((row, column))
                self.create_image(midpoint, image=tile_image)

                if entity_type is not None:
                    entity_image = get_image(f"images/{entity_type}.png", 
                                            self.size, self.image_cache)
                    self.create_image(midpoint, image=entity_image)



class FancyStatsView(AbstractGrid):

    def __init__(self, master: tk.Tk | tk.Frame) -> None:
        super().__init__(master, dimensions=(3, 3), size=(SHOP_WIDTH + MAZE_SIZE, STATS_HEIGHT))
        
        
    def draw_stats(self, moves_remaining: int, strength: int, money: int) -> None:
        """Clears and redraws the FancyStatsView with the provided stats."""
        self.clear()

        main_title = "Player Stats"
        self.annotate_position(position=(0,1), text=main_title,font=TITLE_FONT)

        stat_titles = ["Moves remaining:", "Strength:", "Money:"]
        for i, title in enumerate(stat_titles):
            self.annotate_position(position=(1, i), text=title)

        stats = [moves_remaining, strength, f"${money}"]
        for i, item in enumerate(stats):
            self.annotate_position(position=(2, i), text=item)


class Shop(tk.Frame):
    def __init__(self, master: tk.Frame) -> None:
        super().__init__(master)
        self.item_frames = [] 


        title_label = tk.Label(self, text="Shop", width = SHOP_WIDTH, font=TITLE_FONT)
        title_label.pack()

    def create_buyable_item(self, item: str, amount: int, callback: Callable[[], None]) -> None:
        item_frame = tk.Frame(self)
        item_frame.pack(fill=tk.X)

        item_label = tk.Label(item_frame, text=f"{item}: ${amount}")
        item_label.pack(side=tk.LEFT)

        buy_button = tk.Button(item_frame, text="Buy", command=callback)
        buy_button.pack(side=tk.RIGHT)

        self.item_frames.append(item_frame)

class FancySokobanView(tk.Toplevel):
    def __init__(self, master: tk.Tk, dimensions: tuple[int,int], size: tuple[int,int]) -> None:
        self.master = master

        self.image_cache = {}


        top_frame = tk.Frame(master, width=650, height=BANNER_HEIGHT)
        top_frame.pack(side=tk.TOP)

        banner_image = get_image("images/banner.png", (650, BANNER_HEIGHT), self.image_cache)
        banner = tk.Label(top_frame, image=banner_image)
        banner.image = banner_image
        banner.pack(side=tk.LEFT)




        middle_frame = tk.Frame(master)
        middle_frame.pack(side = tk.TOP)


        self.game = FancyGameView(middle_frame, dimensions, (MAZE_SIZE, MAZE_SIZE))
        self.game.pack(side=tk.LEFT)



        shop_frame = tk.Frame(middle_frame, height=MAZE_SIZE, width=SHOP_WIDTH)
        shop_frame.pack(side=tk.TOP)

        self.shop = Shop(shop_frame)
        self.shop.pack(side = tk.TOP)

        stats_frame = tk.Frame(master, height=STATS_HEIGHT, width=SHOP_WIDTH + MAZE_SIZE)
        stats_frame.pack(side=tk.TOP)

        self.stats = FancyStatsView(stats_frame)
        self.stats.pack()


    def create_shop_items(self, shop_items: dict[str, int], button_callback: Callable[[str], None]) -> None:
        self.shop.create_buyable_item("Strength Potion", 5, button_callback('S'))
        self.shop.create_buyable_item("Move Potion", 5, button_callback('M'))
        self.shop.create_buyable_item("Fancy Potion", 10, button_callback('F'))


    def display_game(self, maze: Grid, entities: Entities, player_position: Position) -> None:
        self.game.display(maze, entities, player_position)
        return None
    
    def display_stats(self, moves: int, strength: int, money: int) -> None:
        self.stats.draw_stats(moves, strength, money)
        return None


class ExtraFancySokoban(tk.Toplevel):
    def __init__(self, root: tk.Tk, maze_file: str) -> None:
        self.root = root
        self._model = SokobanModel(maze_file)

        root.geometry("650x600")
        root.title("Extra Fancy Sokoban")

 
        dimensions=self._model.get_dimensions()
        size=(650, 75)

        self._view = FancySokobanView(root, dimensions, size)

        shop_items = self._model.get_shop_items()
        self._view.create_shop_items(shop_items, self.button_callback)
       
        
        self.root.bind("<Key>", self.handle_keypress)
        self.root.bind("<Button>", self._model.attempt_purchase)
        self.redraw()
        return None
    
    def redraw(self) -> None:
        self.maze = self._model.get_maze()
        self.entities = self._model.get_entities()
        self.player_position = self._model.get_player_position()
        self.moves = self._model.get_player_moves_remaining()
        self.strength = self._model.get_player_strength()
        self.money = self._model.get_player_money() 
        
        self._view.display_game(self.maze, self.entities, self.player_position)
        self._view.display_stats(self.moves, self.strength, self.money)
        

        return None
    
    def button_callback(self, item_id):
        def callback():
            self._model.attempt_purchase(item_id)
            self.redraw()
        return callback
    
    def handle_keypress(self, event: tk.Event) -> None:

        self._model.attempt_move(event.char)
        self.redraw()


        if self._model.has_won() == True:
            answer = messagebox.askyesno(title = None, message = "You won! Play again?") 
            if answer == True:
                self._model.reset()
                self.redraw()
                return None
            
            else:
                return None
            
        if self._model.get_player_moves_remaining() <= 0:
            answer = messagebox.askyesno(title = None, message = "You lost! Play again?") 
            if answer == True:
                self._model.reset()
                self.redraw()
                return None
            else:
                return None
                
        self.redraw()

        return None

def play_game(root: tk.Tk, maze_file: str) -> None:
    ExtraFancySokoban(root, maze_file)
    root.mainloop()
    return None
    
    


def main() -> None:
    """ The main function. """
    root = tk.Tk()
    maze_file = "maze_files/maze1.txt"
    play_game(root, maze_file)
    pass  # Write your main code here


if __name__ == "__main__":
    main()