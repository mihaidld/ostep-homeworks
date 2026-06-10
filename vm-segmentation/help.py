length_space = 7
hex_string = "6c"
binary_value = bin(int(hex_string, 16))[2:].zfill(length_space)
print("binary_value", binary_value)
print("segment", binary_value[0])
print("offset", int(binary_value[1:], 2))
