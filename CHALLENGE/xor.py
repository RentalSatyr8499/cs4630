import os

original = b'\x40\x22\xe3'
forbidden = {0x40, b'Con'}

while True:
    rand = os.urandom(len(original))
    partner = bytes([r ^ o for r, o in zip(rand, original)])

    # Check forbidden bytes
    if any(b in forbidden for b in rand):
        continue
    if any(b in forbidden for b in partner):
        continue

    break

# Convert each byte to hex like 0xAB
rand_hex = ", ".join(f"0x{b:02X}" for b in rand)
partner_hex = ", ".join(f"0x{b:02X}" for b in partner)

# Sanity check
reconstructed = bytes([r ^ p for r, p in zip(rand, partner)])

print("original:      ", original)
print("random:        ", rand_hex)
print("partner:       ", partner_hex)
print("reconstructed: ", reconstructed)
