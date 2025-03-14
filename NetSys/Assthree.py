import math
import random
import datetime

def miller_rabin(n, k=5):
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2
    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_prime_candidate(bit_length):
    p = random.getrandbits(bit_length)
    p |= (1 << (bit_length - 1)) | 1
    return p

def is_prime(n):
    return miller_rabin(n, k=5)

def generate_prime(bit_length):
    p = generate_prime_candidate(bit_length)
    while not is_prime(p):
        p = generate_prime_candidate(bit_length)
    return p

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    gcd, x1, y1 = egcd(b % a, a)
    return (gcd, y1 - (b // a) * x1, x1)

def modinv(a, m):
    gcd, x, _ = egcd(a, m)
    if gcd != 1:
        raise Exception("Modular inverse does not exist")
    return x % m

def generate_keys(bit_length=32):
    p = generate_prime(bit_length)
    q = generate_prime(bit_length)
    while q == p:
        q = generate_prime(bit_length)
    n = p * q
    phi = (p - 1) * (q - 1)
    e = 3
    while math.gcd(e, phi) != 1:
        e += 2
    d = modinv(e, phi)
    return (n, e, d, p, q)

def rsa_encrypt(message, pub_key):
    e, n = pub_key
    m_int = int.from_bytes(message.encode("utf-8"), "big")
    c = pow(m_int, e, n)
    return c

def rsa_decrypt(ciphertext, private_key):
    d, n = private_key
    m_int = pow(ciphertext, d, n)
    num_bytes = (n.bit_length() + 7) // 8
    m_bytes = m_int.to_bytes(num_bytes, "big").lstrip(b'\x00')
    try:
        return m_bytes.decode("utf-8")
    except UnicodeDecodeError:
        return repr(m_bytes)

def create_ca(ca_id, bit_length=32):
    n, e, d, p, q = generate_keys(bit_length)
    ca = {
        'id': ca_id,
        'n': n,
        'e': e,
        'd': d,
        'public_key': (e, n),
        'private_key': (d, n),
        'client_db': {}
    }
    return ca

def register_client(ca, client_id, client_public_key):
    ca['client_db'][client_id] = client_public_key

def issue_certificate(ca, client_id):
    if client_id not in ca['client_db']:
        raise Exception("Client not registered with CA")
    client_public_key = ca['client_db'][client_id]
    TA = datetime.datetime.now().isoformat()
    DURA = "3600"
    data = f"{client_id}|{client_public_key}|{TA}|{DURA}|{ca['id']}"
    m_int = int.from_bytes(data.encode("utf-8"), "big")
    signature = pow(m_int, ca['d'], ca['n'])
    certificate = (data, signature)
    return certificate

def get_client_public_key(ca, client_id):
    return ca['client_db'].get(client_id, None)

def create_client(client_id, bit_length=32):
    n, e, d, p, q = generate_keys(bit_length)
    client = {
        'id': client_id,
        'n': n,
        'e': e,
        'd': d,
        'public_key': (e, n),
        'private_key': (d, n)
    }
    return client

def send_message(receiver_public_key, message):
    return rsa_encrypt(message, receiver_public_key)

def receive_message(private_key, ciphertext):
    return rsa_decrypt(ciphertext, private_key)

def main():
    bit_length = 32
    ca = create_ca("CA1", bit_length)
    print("CA Public Key:", ca['public_key'])
    
    clientA = create_client("A", bit_length)
    clientB = create_client("B", bit_length)
    
    register_client(ca, clientA['id'], clientA['public_key'])
    register_client(ca, clientB['id'], clientB['public_key'])
    
    certA = issue_certificate(ca, clientA['id'])
    certB = issue_certificate(ca, clientB['id'])
    print("\nCertificate for Client A:")
    print(certA)
    print("\nCertificate for Client B:")
    print(certB)
    
    certB_from_CA = issue_certificate(ca, "B")
    dataB, signatureB = certB_from_CA
    fieldsB = dataB.split("|")
    B_public_key = eval(fieldsB[1])
    print("\nClient A obtained B's public key from CA:", B_public_key)
    
    messages_from_A = ["Hello1", "Hello2", "Hello3"]
    encrypted_messages = []
    print("\n--- Client A sending messages to Client B ---")
    for msg in messages_from_A:
        ct = send_message(B_public_key, msg)
        encrypted_messages.append(ct)
        print("Client A sends (encrypted):", ct)
    
    print("\n--- Client B receiving messages from Client A ---")
    for ct in encrypted_messages:
        received_msg = receive_message(clientB['private_key'], ct)
        print("Client B receives (decrypted):", received_msg)
    
    messages_from_B = ["ACK1", "ACK2", "ACK3"]
    encrypted_responses = []
    print("\n--- Client B sending ACK messages to Client A ---")
    for msg in messages_from_B:
        ct = send_message(clientA['public_key'], msg)
        encrypted_responses.append(ct)
        print("Client B sends (encrypted):", ct)
    
    print("\n--- Client A receiving ACK messages from Client B ---")
    for ct in encrypted_responses:
        received_msg = receive_message(clientA['private_key'], ct)
        print("Client A receives (decrypted):", received_msg)

if __name__ == "__main__":
    main()
