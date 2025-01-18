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

def run_test_cases():
    test_cases = [
        ("HELLO WORLD", 4),
        ("TRANSPOSITION CIPHER", 5),
        ("THIS IS A SECRET MESSAGE", 6),
        ("PYTHON PROGRAMMING", 3),
        ("CRYPTOGRAPHY IS FUN", 7),
        ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 5),
        ("A SHORT TEST", 2),
        ("EXAMPLE WITH ODD LENGTH", 4),
        ("SYMMETRIC ENCRYPTION", 8),
        ("LONGER STRING TEST CASE", 9),
        ("EVEN MORE COMPLEX CASE", 6),
        ("SPACES IN BETWEEN", 3)
    ]

    for i, (plaintext, key) in enumerate(test_cases):
        try:
            if not isinstance(plaintext, str):
                raise ValueError(f"Test Case {i + 1}: Plaintext must be a string. Found: {type(plaintext)}")
            if not isinstance(key, int) or key <= 0:
                raise ValueError(f"Test Case {i + 1}: Key must be a positive integer. Found: {key}")
            if len(plaintext) == 0:
                raise ValueError(f"Test Case {i + 1}: Plaintext cannot be empty.")

            ciphertext = encrypt_transposition(plaintext, key)
            decrypted_plaintext = decrypt_transposition(ciphertext, key)

            print(f"Test Case {i + 1}:")
            print(f"  Plaintext: {plaintext}")
            print(f"  Key: {key}")
            print(f"  Ciphertext: {ciphertext}")
            print(f"  Decrypted Plaintext: {decrypted_plaintext}")
            print(f"  Test Passed: {remove_spaces(plaintext) == decrypted_plaintext}")
        except ValueError as e:
            print(f"Test Case {i + 1} Error: {e}")
        except Exception as e:
            print(f"Test Case {i + 1} Unexpected Error: {e}")
        finally:
            print("-" * 50)


run_test_cases()
