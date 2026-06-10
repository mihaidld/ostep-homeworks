length_space = 14
hex_string = "0x00002e0f"
binary_value = bin(int(hex_string, 16))[2:].zfill(length_space)
print("binary_value", binary_value)
print("segment", int(binary_value[0:4], 2))
print("offset", int(binary_value[4:], 2))
