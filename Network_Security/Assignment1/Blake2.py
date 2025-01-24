import hashlib
import itertools

def preprocess_plaintext(plaintext):
    return ''.join(c for c in plaintext.lower() if 'a' <= c <= 'z')

def calculate_rows(plaintext, key_length):
    return (len(plaintext) + key_length - 1) // key_length

def initialize_grid(num_rows, num_cols):
    return [['' for _ in range(num_cols)] for _ in range(num_rows)]

def encrypt_transposition(plaintext, key):
    plaintext = preprocess_plaintext(plaintext)
    num_cols = len(key)
    num_rows = calculate_rows(plaintext, num_cols)
    grid = initialize_grid(num_rows, num_cols)
    index = 0
    for row in range(num_rows):
        for col in range(num_cols):
            if index < len(plaintext):
                grid[row][col] = plaintext[index]
                index += 1
    ciphertext = ''
    for k in sorted(range(len(key)), key=lambda x: key[x] - 1):  
        for row in range(num_rows):
            if grid[row][k]:
                ciphertext += grid[row][k]
    
    return ciphertext

def decrypt_transposition(ciphertext, key):
    num_cols = len(key)
    num_rows = calculate_rows(ciphertext, num_cols)

    grid = initialize_grid(num_rows, num_cols)

    full_cols = len(ciphertext) % num_cols
    col_heights = [len(ciphertext) // num_cols + (i < full_cols) for i in range(num_cols)]
    
    index = 0
    for k in sorted(range(len(key)), key=lambda x: key[x] - 1): 
        for row in range(col_heights[k]):
            grid[row][k] = ciphertext[index]
            index += 1

    plaintext = ''
    for row in range(num_rows):
        for col in range(num_cols):
            if grid[row][col]:
                plaintext += grid[row][col]
    
    return plaintext


def blake2_hash(plaintext):
    blake2b_hash = hashlib.blake2b(plaintext.encode())
    return blake2b_hash.hexdigest()

def brute_force_attack(ciphertext, original_hash, key_length):
    possible_keys = itertools.permutations(range(1, key_length + 1))
    
    for key in possible_keys:
        decrypted_text = decrypt_transposition(ciphertext, key)
        decrypted_hash = blake2_hash(decrypted_text)
        if decrypted_hash == original_hash:
            return key
    return None

def validate_key(key):
    if len(set(key)) != len(key):
        raise ValueError("Key contains duplicate values")
    
    expected_key_set = set(range(1, len(key) + 1))
    key_set = set(key)
    missing_numbers = expected_key_set - key_set
    if missing_numbers:
        raise ValueError(f"Missing numbers in the key: {', '.join(map(str, missing_numbers))}")



def run_test_cases():
    test_cases = [
        ("helloworld", [1, 3, 4, 5, 2]),
        ("transpositioncipher", [3, 1, 2, 4]),
        ("thisisasecretmessage", [1, 2, 3, 4, 5, 6]),
        ("pythonprogramming", [1, 4, 2, 3, 5, 6, 9, 8, 7]),
        ("cryptographyisfun", [1, 2, 5, 4, 3]),
    ]
    for i, (plaintext, key) in enumerate(test_cases):
        validate_key(key)
    
    for i, (plaintext, key) in enumerate(test_cases):
        original_plaintext = preprocess_plaintext(plaintext)
        ciphertext = encrypt_transposition(original_plaintext, key)
        decrypted_plaintext = decrypt_transposition(ciphertext, key)

        original_hash = blake2_hash(original_plaintext)

        print(f"Test Case {i + 1}:")
        print(f"  Plaintext: {plaintext}")
        print(f"  Key: {key}")
        print(f"  Ciphertext: {ciphertext}")
        print(f"  Decrypted Plaintext: {decrypted_plaintext}")
        print(f"  Original Hash: {original_hash}")
        print(f"  Test Passed: {original_plaintext == decrypted_plaintext}")
        print("-" * 50)

        brute_force_key = brute_force_attack(ciphertext, original_hash, len(key))
        if brute_force_key:
            print(f"  Brute-force Found Key: {brute_force_key}")
        else:
            print(f"  Brute-force Attack Failed to Find the Key")

        print("=" * 50)

run_test_cases()
