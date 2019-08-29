# ELF64 Segment parser
import struct

def unpack(fmt, string):
    # This is a wrapper as the original returns a tuple >:(
    return struct.unpack(fmt, string)[0]

def load_elf(file):
    with open(file, 'rb') as f:
        return bytearray(f.read())

def find_SHT(elf):
    e_shoff = elf[40:48]
    return unpack("<q", e_shoff)

def parse_segments(elf):
    """
        Parses the segments and stores the information in a dictionary
        
        bytes  elf:          The ELF file

        return:      The segments
        return type: Dictionary
    """
    
    e_shoff = find_SHT(elf)
    raw_segments = elf[e_shoff:]
    segments = {}
    for i in range(0, len(raw_segments), 64): 
        segment     = raw_segments[i:i+64] # Each block is 64 bytes
        sh_name     = unpack('<i', segment[:4])
        sh_type     = unpack('<i', segment[4:8])
        sh_flags    = unpack('<q', segment[8:16])
        sh_addr     = unpack('<q', segment[16:24])
        sh_offset   = unpack('<q', segment[24:32])
        sh_size     = unpack('<q', segment[32:40])

        # To ensure the i is normal you need to divide by 64 (steps were in 64) 
        segments[int(i/64)] = [sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size]
        # not_needed_rn_bytes = segment[40:]

    return segments

def name_segments(elf):
    """
        Gives back the named versions of the segments
        
        bytes  elf:          The ELF file

        return:      The segments
        return type: Dictionary
    """

    segments = parse_segments(elf)
    e_shstrndx = unpack('<h', elf[62:64])
    shstrtab = segments[e_shstrndx]

    # This is offset : offset + size
    raw_shstrtab = elf[shstrtab[4]+1:shstrtab[4]+shstrtab[5]]

    # Each item, go to offset and loop through until null-byte is hit and give the sh_name that string
   
    for k, v in segments.items():
        s = ' '
        while s[-1] != '\x00':
            s += chr(raw_shstrtab[len(s)-1 + v[0]])
        v[0] = s[1:-1]    
    return segments

def print_segments(segments):
    for c,i in enumerate(segments.values()):
        print(f"""
Number: {c}
Section Header Table @ {hex(find_SHT(elf) + c*64)}
sh_name:    '{i[0]}'
sh_type:    {hex(i[1])}
sh_flags:   {hex(i[2])}
SH_ADDR:    {hex(i[3])}
SH_OFFSET:  {hex(i[4])}
SH_SIZE:    {hex(i[5])}""")

def extend_data_segment(elf, size):
    """
        Extends the .data section in an ELF binary
         
        bytes  elf:          The ELF file
        int    size:         The size to extend by

        return:      The elf bytes
        return type: Bytes
    """
    
    segments = name_segments(elf)
    data_index = next(filter(lambda x: segments[x][0] == 'data', segments.keys()))

    # Edit segments after with the adjusted values
    for i in range(data_index+1, len(segments)):
        segment = segments[i]
        segment[3] = segment[3] + size if segment[3] else segment[3] 
        segment[4] += size

    # Update the ELF Header e_shoff
    e_shoff = find_SHT(elf) + size
    elf[40:48] = struct.pack("<q", e_shoff)

    # Add new data
    new_data = bytearray([0 for i in range(size)])
    end_data_segment = segments[data_index][4]+segments[data_index][5]
    elf = elf[:end_data_segment] + new_data + elf[end_data_segment:]
    
    # Edit the segment with new size
    segments[data_index][5] += size

    # Write all the segments to the ELF in bytes
    raw_segments = elf[e_shoff:]
    for i in range(0, len(segments)):
        segment     = raw_segments[i*64 : (i+1)*64]
        segment[16:24] = struct.pack('<q', segments[i][3]) # sh_addr
        segment[24:32] = struct.pack('<q', segments[i][4]) # sh_offset
        segment[32:40] = struct.pack('<q', segments[i][5]) # sh_size
        raw_segments[i*64 : (i+1)*64] = segment
    elf[e_shoff:] = raw_segments
    
    return elf

def set_segment_permissions(elf, name, permissions):
    """
        Sets the segment permissions

        bytes  elf:          The ELF file
        string name:         The name of the section
        int    permissions:  The permissions (similar to unix permissions)

        return:      The elf bytes
        return type: Bytes
    """
    
    # Setting up
    segments = name_segments(elf)
    index = next(filter(lambda x: segments[x][0] == name, segments.keys()))
    e_shoff = find_SHT(elf)

    # Map the Section Header Table
    raw_segments = elf[e_shoff:]
    # Go to desired table no. and bytes and write permissions
    raw_segments[(index * 64) + 8: (index * 64) + 16] = struct.pack('<q', permissions)
    elf[e_shoff:] = raw_segments
    
    return elf
    
# Extending data example
# -----------------------
# 
elf = load_elf("./main")
new_elf = extend_data_segment(elf, 0x1000)
with open('./new_main', 'wb') as f:
    f.write(new_elf)

# Permissions data example
# -----------------------
elf = load_elf("./new_main")
elf = set_segment_permissions(elf, 'data', 0x7)
with open('./new_main', 'wb') as f:
    f.write(elf)


# Resources
#
# * http://osr507doc.sco.com/en/topics/ELF_secthead.html#ELF_sh_type
# *
# *
# *
