def remove_spaces(plaintext):
    return plaintext.replace(" ", "")

def calculate_rows(plaintext, key):
    quotient = len(plaintext) // key
    remainder = len(plaintext) % key
    num_rows = quotient + (remainder != 0)
    return num_rows

def initialize_grid(num_rows, key):
    return [['' for _ in range(key)] for _ in range(num_rows)]

def encrypt_transposition(plaintext, key):
    plaintext = remove_spaces(plaintext)
    num_rows = calculate_rows(plaintext, key)
    grid = initialize_grid(num_rows, key)

    index = 0
    for row in range(num_rows):
        for col in range(key):
            if index < len(plaintext):
                grid[row][col] = plaintext[index]
                index += 1

    ciphertext = ""
    for col in range(key):
        for row in range(num_rows):
            if grid[row][col] != '':
                ciphertext += grid[row][col]
    return ciphertext

def populate_grid_for_decryption(ciphertext, num_rows, key):
    grid = initialize_grid(num_rows, key)
    full_cols = len(ciphertext) % key
    col_heights = [len(ciphertext) // key + (i < full_cols) for i in range(key)]

    index = 0
    for col in range(key):
        for row in range(col_heights[col]):
            grid[row][col] = ciphertext[index]
            index += 1
    return grid

def decrypt_transposition(ciphertext, key):
    num_rows = calculate_rows(ciphertext, key)
    grid = populate_grid_for_decryption(ciphertext, num_rows, key)

    plaintext = ""
    for row in range(num_rows):
        for col in range(key):
            if grid[row][col] != '':
                plaintext += grid[row][col]
    return plaintext


plaintext = "Rohan beriwal"
key = 4
ciphertext = encrypt_transposition(plaintext, key)
print(f"Ciphertext: {ciphertext}")
decrypted_plaintext = decrypt_transposition(ciphertext, key)
print(f"Decrypted Plaintext: {decrypted_plaintext}")
