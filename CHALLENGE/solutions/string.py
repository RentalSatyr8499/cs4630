def to_le_qwords(s: str):
    data = s.encode('utf-8')
    # pad to 8-byte boundary
    if len(data) % 8 != 0:
        data += b'\x00' * (8 - (len(data) % 8))

    qwords = []
    for i in range(0, len(data), 8):
        chunk = data[i:i+8]
        val = int.from_bytes(chunk, 'little')
        qwords.append(val)

    return qwords


s = "Congratulations, xaq7pj!\nYou have passed this challenge.\n"
for q in to_le_qwords(s):
    print(f"0x{q:016x}")
