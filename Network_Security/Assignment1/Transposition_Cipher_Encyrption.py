def remove_spaces(plaintext):
    return plaintext.replace(" ", "")

def calculate_rows(plaintext, key):
    quotient = len(plaintext) // key
    remainder = len(plaintext) % key
    num_rows = quotient + (remainder != 0)
    return num_rows

def initialize_grid(key):
    grid = ['' for _ in range(key)]
    return grid

def populate_grid(plaintext, num_rows, key):
    grid = initialize_grid(key)
    for i in range(len(plaintext)):
        row = i % num_rows
        grid[row] += plaintext[i]
    return grid

def generate_ciphertext(grid):
    ciphertext = ''.join(grid)
    return ciphertext

def encrypt_transposition(plaintext, key):
    plaintext = remove_spaces(plaintext)
    num_rows = calculate_rows(plaintext, key)
    grid = populate_grid(plaintext, num_rows, key)
    ciphertext = generate_ciphertext(grid)
    return ciphertext

plaintext = "HELLO WORLD"
key = 4
ciphertext = encrypt_transposition(plaintext, key)
print(ciphertext)
