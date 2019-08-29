# ELF Segment parser
import struct

def unpack(fmt, string):
    # This is a wrapper as the original returns a tuple >:(
    return struct.unpack(fmt, string)[0]

def load_elf(file):
    with open(file, 'rb') as f:
        return f.read()

def find_SHT(elf):
    e_shoff = elf[40:48]
    return unpack("<q", e_shoff)

def parse_segments(elf, e_shoff):
    raw_segments = elf[e_shoff:]
    segments_dict = {}
    for i in range(0, len(raw_segments), 64): 
        segment     = raw_segments[i:i+64] # Each block is 64 bytes
        sh_name     = unpack('<i', segment[:4])
        sh_type     = unpack('<i', segment[4:8])
        sh_flags    = unpack('<q', segment[8:16])
        sh_addr     = unpack('<q', segment[16:24])
        sh_offset   = unpack('<q', segment[24:32])
        sh_size     = unpack('<q', segment[32:40])

        # To ensure the i is normal you need to divide by 64 (steps were in 64) 
        segments_dict[int(i/64)] = [sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size]
        # not_needed_rn_bytes = segment[40:]

    return segments_dict

def name_segments(elf, segments_dict):
    e_shstrndx = unpack('<h', elf[62:64])
    shstrtab = segments_dict[e_shstrndx]

    # This is offset : offset + size
    raw_shstrtab = elf[shstrtab[4]+1:shstrtab[4]+shstrtab[5]]

    # Each item, go to offset and loop through until null-byte is hit and give the sh_name that string
   
    for k, v in segments_dict.items():
        s = ' '
        while s[-1] != '\x00':
            s += chr(raw_shstrtab[len(s)-1 + v[0]])
        v[0] = s[1:-1]    
    return segments_dict

def print_segments(segments_dict):
    for i in segments_dict.values():
        print(f"""
sh_name:    '{i[0]}'
sh_ADDR:    {hex(i[3])}
sh_SIZE:    {hex(i[5])}
sh_OFFSET:  {hex(i[4])}
sh_flags:   {hex(i[2])}
sh_type:    {hex(i[1])}""")


elf = load_elf("./main")
e_shoff = find_SHT(elf)
segments_dict = parse_segments(elf, e_shoff)

segments_dict = name_segments(elf, segments_dict)

print_segments(segments_dict)

# Resources
#
# * http://osr507doc.sco.com/en/topics/ELF_secthead.html#ELF_sh_type
# *
# *
# *
