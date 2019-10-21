#!/usr/bin/python

import os
import sys
import argparse
import os.path
from array import *

VERSION = 0
SUBVERSION = 1

HIMAX_CMD_OFFSET_OP         = 0
HIMAX_CMD_OFFSET_ADDR       = 1
HIMAX_CMD_OFFSET_REGADDR    = 2
HIMAX_CMD_OFFSET_REGVALUE   = 3

dict_DeviceDefaultAddress = {
    'HM01B0':       '24',
}

def check_file_existence(x):
    if not os.path.isfile(x):
        # Argparse uses the ArgumentTypeError to give a rejection message like:
        # error: argument input: x does not exist
        raise argparse.ArgumentTypeError("{0} does not exist".format(x))
    return x

def create_outputfile(args):

    path = os.path.dirname(os.path.abspath(args.ifile))
    basename = os.path.split(os.path.splitext(args.ifile)[0])[-1]
    outputfile = os.path.join(path, basename + '.h')

    return outputfile

def do_convert(args, ofilename):
    ofile = open(ofilename, 'w')
    ofile.write("\n")
    ofile.write("#include \"%s.h\"\n" % args.model)
    ofile.write("\n")
    ofile.write("const hm_script_t s%sInitScript[] =\n" % args.model)
    ofile.write("{\n")

    with open(args.ifile, 'r') as ifile:
        for line in ifile:
            items = line.split()
            ofile.write("// %s\n" % (' '.join(items)))
            print("// %s" % (' '.join(items)))
            if len(items) > 1:
                if items[HIMAX_CMD_OFFSET_OP] == 'W' and items[HIMAX_CMD_OFFSET_ADDR] == dict_DeviceDefaultAddress.get(args.model):
                    print("    {0x%s, 0x%s,}," % (items[HIMAX_CMD_OFFSET_REGADDR], items[HIMAX_CMD_OFFSET_REGVALUE]))
                    ofile.write("    {0x%s, 0x%s,},\n" % (items[HIMAX_CMD_OFFSET_REGADDR], items[HIMAX_CMD_OFFSET_REGVALUE]))

    ofile.write("};\n")
    ofile.close()

def main():
    parser = argparse.ArgumentParser(
        description = 'This program converts a given Himax Script into a c header file.')

    parser.add_argument('-i', '--input', 
                        dest        = 'ifile',
                        required    = True,
                        help        = 'input file',
                        metavar     = 'FILE',
                        type        = check_file_existence)

    parser.add_argument('-m', '--model',
                        dest        = 'model',
                        required    = True,
                        help        = 'Himax Sensor Model',
                        choices     = ['HM01B0'],
                        default     = 'HM01B0',
                        )


    parser.add_argument('-v', '--version',
                        help        = 'show the program version',
                        action      = 'version',
                        version     = '%(prog)s {ver}'.format(ver = 'v%d.%d' %\
                            (VERSION, SUBVERSION)))

    args = parser.parse_args()

    ofilename = create_outputfile(args)

    # print('%s' % ofile)

    do_convert(args, ofilename)

    print "done!"


if __name__ == "__main__":
   main()