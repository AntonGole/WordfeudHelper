from PIL import Image
import os
import pytesseract
import numpy as np
import unicodedata


def crop_image():
    im = Image.open(r"data/IMG_7333 copy.JPG")
    
    os.makedirs("data/crops", exist_ok=True)
    
    left_origin = 26
    top_origin = 124
    small_tile = 68
    large_tile = 69
    
    for y in range(15):
        top = top_origin + sum((large_tile if i % 2 else small_tile) for i in range(y))
        tile_height = large_tile if y % 2 else small_tile

        for x in range(15):
            left = left_origin + sum((large_tile if i % 2 else small_tile) for i in range(x))
            tile_width = large_tile if x % 2 else small_tile

            right = left + tile_width
            bottom = top + tile_height
            im1 = im.crop((left, top, right, bottom))

            im1.save(f"data/crops/{x}_{y}.jpg")
            
def read_tile(tile_img: Image.Image, tile_name: str = None) -> str:
    # Convert to RGB and NumPy array
    img_rgb = tile_img.convert("RGB")
    arr = np.array(img_rgb)

    # Average color
    avg_color = np.mean(arr.reshape(-1, 3), axis=0)
    r, g, b = avg_color

    def is_recent_highlight():
        return (
            214 <= r <= 255 and
            200 <= g <= 255 and
            120 <= b <= 180
        )

    def is_red():
        return 97 <= r <= 147 and 31 <= g <= 81 and 35 <= b <= 85

    def is_orange():
        return 156 <= r <= 206 and 90 <= g <= 140 and 6 <= b <= 56

    def is_green():
        return g > 120 and r < 120 and b < 120

    def is_blue():
        return b > 150 and r < 120 and g < 120

    def is_gray():
        return abs(r - g) < 15 and abs(r - b) < 15 and r < 150

    if is_recent_highlight():
        pass

    elif is_gray():
        return ' '

    elif is_green():
        return '0'
    elif is_blue():
        return '1'
    elif is_orange():
        return '2'
    elif is_red():
        return '3'

    letter_crop = tile_img.crop((15, 0, 43, 63)).convert("L").resize((66, 106))

    # Threshold to binary
    threshold = 100
    binarized = letter_crop.point(lambda p: 0 if p < threshold else 255, mode='1')

    # Save processed image
    if tile_name:
        os.makedirs("data/converted", exist_ok=True)
        binarized.save(f"data/converted/{tile_name}")

    # OCR
    text = pytesseract.image_to_string(
        binarized,
        lang='swe',
        config='--psm 8 -c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ'
    )
    text = unicodedata.normalize('NFC', text.strip().upper())
    return text.strip().lower() or ' '
            
def read_tiles():
    board = []
    tile_outputs = []

    for y in range(15):
        row = []
        for x in range(15):
            path = f"data/crops/{x}_{y}.jpg"
            img = Image.open(path)
            char = read_tile(img, tile_name=f"{x}_{y}.jpg")
            row.append(char)
            tile_outputs.append((f"{x}_{y}.jpg", char))
        board.append(row)
        
    return board
    
def print_board(board):
    col_headers = "    " + "".join(f"{i:^3}" for i in range(15))
    print(col_headers)
    print("   +" + "---" * 15 + "+")

    for y, row in enumerate(board):
        row_str = "".join(f"{(c if c != ' ' else '.'):^3}" for c in row)
        print(f"{y:2} |{row_str}|")

    print("   +" + "---" * 15 + "+")
    
def save_board(board):
    with open("output/board.txt", "w", encoding="utf-8") as f:
        for row in board:
            f.write("".join(row) + "\n")
    

if __name__ == '__main__':
    board = read_tiles()
    save_board(board)
    #print_board(board)