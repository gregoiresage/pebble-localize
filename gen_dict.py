#!/usr/bin/python
import os, sys, string, pdb
import re, fileinput
import ctypes
import struct
import json

#Helper functions
def hash_djb2(string):
    """hash a string using djb2 with seed 5381
    Args:
        string (str): string to hash
    Returns:
        (int): hashval of string masked to force positive by masking 32-bit
    """
    hashval = ctypes.c_uint(5381)
    for char in string:
        hashval.value = ((hashval.value << 5) + hashval.value) + ord(char)
    return hashval.value & 0x7FFFFFFF


def gen_loc_dict(code_dir):
    fileglob_list = []
    loc_dict = {}
    pbllog_regex = "_\(\"(?P<loc>[^\)]+)\"\)"

    for root, dirnames, filenames in os.walk(code_dir):
        for filename in [filename for filename in filenames \
                if ".c" in filename[-2:]]:
            fileglob_list.append(os.path.join(root, filename))
    for filename in fileglob_list:
        with open(filename, 'rb') as afile:
            text = afile.read()
            match_list = re.finditer(pbllog_regex, text)
            if match_list:
                for match in match_list:
                  text = match.group('loc').decode('string_escape')
                  loc_dict[hash_djb2(text)] = text

    return {str(key) : value for (key, value) in loc_dict.iteritems()}

def main():
    # arguments, print an example of correct usage.
    if len(sys.argv) - 1 != 2:
        print("********************")
        print("Usage suggestion:")
        print("python " + sys.argv[0] + " <code_dir> <loc_english.json>")
        print("********************")
        exit()

    code_dir = sys.argv[1]
    output_filename = sys.argv[2]

    json_dict = gen_loc_dict(code_dir) 
    json.dump(json_dict, open(output_filename, "wb"), indent=2, sort_keys=True)
    print("%s now has %d entries\n" % (output_filename, len(json_dict)))

if __name__ == '__main__':
    main()
