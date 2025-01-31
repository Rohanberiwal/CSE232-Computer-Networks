from argon2 import PasswordHasher

from itertools import permutations

def preprocess_plaintext(plaintext):
    lower_text = plaintext.lower()
    filtered_text = ''.join(c for c in lower_text if 'a' <= c <= 'z')
    return filtered_text


def computation_hash(plaintext):
    ph = PasswordHasher()
    hashed_password = ph.hash(plaintext)
    return hashed_password

def calculate_rows(plaintext, key_length):
    total_chars = len(plaintext)
    full_rows = total_chars // key_length
    if total_chars % key_length != 0:
        full_rows += 1
    return full_rows

def makeMatrix(num_rows, num_cols):
    grid = []
    for row in range(num_rows):
        current_row = []
        for col in range(num_cols):
            current_row.append('')
        grid.append(current_row)
    return grid

def fill_Matrix(plaintext, key):
    num_cols = len(key)
    num_rows = calculate_rows(plaintext, num_cols)
    grid = makeMatrix(num_rows, num_cols)

    index = 0
    for row in range(num_rows):
        for col in range(num_cols):
            if index < len(plaintext):
                grid[row][col] = plaintext[index]
                index += 1
    
    return grid, num_rows

def encrypt_transposition(plaintext, key):
    plaintext = preprocess_plaintext(plaintext)
    hash_value = computation_hash(plaintext)
    plaintext_with_hash = plaintext + hash_value

    grid, num_rows = fill_Matrix(plaintext_with_hash, key)
    
    ciphertext = ''
    sorted_indices = sorted(range(len(key)), key=lambda x: key[x] - 1)
    
    for k in sorted_indices:
        for row in range(num_rows):
            if grid[row][k]:
                ciphertext += grid[row][k]

    return ciphertext


def fill_Matrix_decryption(ciphertext, key):
    num_cols = len(key)
    num_rows = calculate_rows(ciphertext, num_cols)
    grid = makeMatrix(num_rows, num_cols)

    full_cols = len(ciphertext) % num_cols
    col_heights = []
    for i in range(num_cols):
        if i < full_cols:
            col_heights.append(len(ciphertext) // num_cols + 1)
        else:
            col_heights.append(len(ciphertext) // num_cols)

    index = 0
    sorted_indices = sorted(range(len(key)), key=lambda x: key[x] - 1) 
    for k in sorted_indices:
        for row in range(col_heights[k]):
            grid[row][k] = ciphertext[index]
            index += 1
    
    return grid, num_rows, num_cols

def decrypt_transposition(ciphertext, key):
    grid, num_rows, num_cols = fill_Matrix_decryption(ciphertext, key)
    
    plaintext_with_hash = ''
    for row in range(num_rows):
        for col in range(num_cols):
            if grid[row][col]:
                plaintext_with_hash += grid[row][col]
    
    plaintext = plaintext_with_hash[:-64]
    extracted_hash = plaintext_with_hash[-64:]
    
    return plaintext, extracted_hash

def try_decrypt_with_key(ciphertext, key):
    try:
        decrypted_plaintext, decrypted_hash = decrypt_transposition(ciphertext, key)
        if computation_hash(decrypted_plaintext) == decrypted_hash:
            return decrypted_plaintext, key
    except:
        return None, None
    return None, None

def brute_force_attack(ciphertext, max_key_length=9):
    for key_length in range(1, max_key_length + 1):
        for key in permutations(range(1, key_length + 1)):
            decrypted_plaintext, found_key = try_decrypt_with_key(ciphertext, key)
            if found_key:
                print(f"Key found: {found_key}")
                print(f"Decrypted Plaintext: {decrypted_plaintext}")
                return found_key
    print("No valid key found.")
    return None


def main():
    test_cases = [
    ("helloworld", [1, 3, 4, 5, 2]),
    ("transpositioncipher", [3, 1, 2, 4]),
    ("thisisasecretmessage", [1, 2, 3, 4, 5, 6]),
    ("pythonprogramming", [1, 4, 2, 3, 5, 6, 9, 8, 7]),
    ("cryptographyisfun", [1, 2, 5, 4, 3]),
    ("thisisfortestingthus", [3, 1, 4, 2]),
    ("securecommunication", [2, 3, 1, 5, 4]),
    ("informationsecurity", [4, 1, 3, 2, 5]),
    ("confidentialmessage", [5, 3, 1, 4, 2]),
    ("dataprotection", [1, 3, 2, 4, 5]),
    ("encryptiondecryption", [2, 4, 3, 1, 5]),
    ("messageauthentication", [3, 5, 1, 2, 4]),
    ("blockciphermode", [1, 2, 3, 4]),
    ("transpositiontechnique", [4, 2, 1, 3]),
    ("symmetriccryptosystem", [3, 5, 2, 1, 4]),
    ("publickeyinfrastructure", [2, 1, 4, 3, 5]) , 
    ("networksecurity", [3, 1, 4, 2])  , 
    ("thisisfortesting", [3, 1, 4, 2])  ,
    ("secondtest", [3, 1, 4, 2]) 
    ]

    for i, (plaintext, key) in enumerate(test_cases):
        plaintext = preprocess_plaintext(plaintext)
        hash_value = computation_hash(plaintext)
   

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
        
        print("Launching brute-force attack...")
        brute_force_attack(ciphertext)
        print("-" * 50)

main()
