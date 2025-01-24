import hashlib

def preprocess_plaintext(plaintext):
    return ''.join(c for c in plaintext.lower() if 'a' <= c <= 'z')

def calculate_rows(plaintext, key):
    return (len(plaintext) + key - 1) // key

def initialize_grid(num_rows, key):
    return [['' for _ in range(key)] for _ in range(num_rows)]

def encrypt_transposition(plaintext, key):
    plaintext = preprocess_plaintext(plaintext)
    num_rows = calculate_rows(plaintext, key)
    grid = initialize_grid(num_rows, key)

    index = 0
    for row in range(num_rows):
        for col in range(key):
            if index < len(plaintext):
                grid[row][col] = plaintext[index]
                index += 1

    return ''.join(grid[row][col] for col in range(key) for row in range(num_rows) if grid[row][col] != '')

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

    return ''.join(grid[row][col] for row in range(num_rows) for col in range(key) if grid[row][col] != '')

def calculate_hash(plaintext):
    return hashlib.sha256(plaintext.encode()).hexdigest()

def brute_force_attack(ciphertext, original_hash):
    for key in range(1, 10):  # Keys 1 to 9
        decrypted_plaintext = decrypt_transposition(ciphertext, key)
        if calculate_hash(decrypted_plaintext) == original_hash:
            return key, decrypted_plaintext
    return None, None

def run_test_cases():
    test_cases = [
        ("hello world", 3),
        ("transposition cipher", 4),
        ("this is a secret message", 5),
        ("python programming", 6),
        ("cryptography is fun", 7),
    ]

    for i, (plaintext, key) in enumerate(test_cases):
        plaintext = preprocess_plaintext(plaintext)
        ciphertext = encrypt_transposition(plaintext, key)
        decrypted_plaintext = decrypt_transposition(ciphertext, key)

        original_hash = calculate_hash(plaintext)
        brute_force_key, brute_force_plaintext = brute_force_attack(ciphertext, original_hash)

        print(f"Test Case {i + 1}:")
        print(f"  Plaintext: {plaintext}")
        print(f"  Key: {key}")
        print(f"  Ciphertext: {ciphertext}")
        print(f"  Decrypted Plaintext: {decrypted_plaintext}")
        print(f"  Original Hash: {original_hash}")
        print(f"  Brute-Force Key: {brute_force_key}")
        print(f"  Brute-Force Plaintext: {brute_force_plaintext}")
        print(f"  Test Passed: {plaintext == decrypted_plaintext and plaintext == brute_force_plaintext}")
        print("-" * 50)

run_test_cases()
