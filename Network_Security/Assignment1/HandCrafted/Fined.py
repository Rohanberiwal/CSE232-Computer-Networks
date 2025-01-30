import hashlib
from itertools import permutations

def preprocess_plaintext(plaintext):
    """Remove non-alphabet characters and convert to lowercase."""
    return ''.join(c for c in plaintext.lower() if 'a' <= c <= 'z')

def calculate_hash(plaintext):
    """Compute SHA-256 hash of plaintext and return as a hex string."""
    return hashlib.sha256(plaintext.encode()).hexdigest()

def calculate_rows(plaintext, key_length):
    """Calculate the number of rows required for the transposition grid."""
    return (len(plaintext) + key_length - 1) // key_length

def initialize_grid(num_rows, num_cols):
    """Initialize an empty grid for transposition."""
    return [['' for _ in range(num_cols)] for _ in range(num_rows)]

def encrypt_transposition(plaintext, key):
    """Encrypt the plaintext using a transposition cipher with the given key."""
    plaintext = preprocess_plaintext(plaintext)
    hash_value = calculate_hash(plaintext)
    plaintext_with_hash = plaintext + hash_value  # Append hash

    num_cols = len(key)
    num_rows = calculate_rows(plaintext_with_hash, num_cols)
    grid = initialize_grid(num_rows, num_cols)

    index = 0
    for row in range(num_rows):
        for col in range(num_cols):
            if index < len(plaintext_with_hash):
                grid[row][col] = plaintext_with_hash[index]
                index += 1

    ciphertext = ''
    for k in sorted(range(len(key)), key=lambda x: key[x] - 1):  
        for row in range(num_rows):
            if grid[row][k]:
                ciphertext += grid[row][k]

    return ciphertext

def decrypt_transposition(ciphertext, key):
    """Decrypt the transposition cipher and validate the hash."""
    num_cols = len(key)
    num_rows = calculate_rows(ciphertext, num_cols)

    grid = initialize_grid(num_rows, num_cols)

    # Determine how many characters each column should have
    full_cols = len(ciphertext) % num_cols
    col_heights = [len(ciphertext) // num_cols + (i < full_cols) for i in range(num_cols)]
    
    index = 0
    for k in sorted(range(len(key)), key=lambda x: key[x] - 1): 
        for row in range(col_heights[k]):
            grid[row][k] = ciphertext[index]
            index += 1

    plaintext_with_hash = ''
    for row in range(num_rows):
        for col in range(num_cols):
            if grid[row][col]:
                plaintext_with_hash += grid[row][col]

    # Extract plaintext and hash
    plaintext = plaintext_with_hash[:-64]
    extracted_hash = plaintext_with_hash[-64:]

    return plaintext, extracted_hash

def brute_force_attack(ciphertext, max_key_length=9):
    """Perform brute-force attack to find the key."""
    for key_length in range(1, max_key_length + 1):
        print(f"Trying key length: {key_length}")
        for key in permutations(range(1, key_length + 1)):
            try:
                decrypted_plaintext, decrypted_hash = decrypt_transposition(ciphertext, key)
                if calculate_hash(decrypted_plaintext) == decrypted_hash:
                    print(f"Key found: {key}")
                    print(f"Decrypted Plaintext: {decrypted_plaintext}")
                    return key
            except:
                continue
    print("No valid key found.")
    return None

def run_test_cases():
    """Run test cases for encryption, decryption, and brute-force attack."""

    test_cases = [
        ("helloworld", [1, 3, 4, 5, 2]),
        ("transpositioncipher", [3, 1, 2, 4]),
        ("thisisasecretmessage", [1, 2, 3, 4, 5, 6]),
        ("pythonprogramming", [1, 4, 2, 3, 5, 6 , 9,8,7]),
        ("cryptographyisfun", [1, 2, 5, 4, 3]),
    ]


    for i, (plaintext, key) in enumerate(test_cases):
        plaintext = preprocess_plaintext(plaintext)
        hash_value = calculate_hash(plaintext)
        plaintext_with_hash = plaintext + hash_value
        
        ciphertext = encrypt_transposition(plaintext, key)
        decrypted_plaintext, decrypted_hash = decrypt_transposition(ciphertext, key)

        print(f"Test Case {i + 1}:")
        print(f"  Original Plaintext: {plaintext}")
        print(f"  Hash Generated: {hash_value}")
        print(f"  Ciphertext: {ciphertext}")
        print(f"  Decrypted Plaintext: {decrypted_plaintext}")
        print(f"  Decrypted Hash: {decrypted_hash}")
        print(f"  Test Passed: {plaintext == decrypted_plaintext and hash_value == decrypted_hash}")
        print("-" * 50)

        # Brute-force attack
        print("Launching brute-force attack...")
        brute_force_attack(ciphertext)
        print("-" * 50)

run_test_cases()
