#!/usr/bin/python3

import os
import sys
import argparse
import os.path
from array import *

from PIL import Image
import numpy as np
import re

dict_Resolutions = {
    'QVGA':     (324, 244),
}

# height = 244
# width = 324

VERSION     = 0
SUBVERSION  = 1

class RawData:
    ui8Array                = None

    def __init__(self):
        self.ui8Array       = array('B')


def check_file_existence(x):
    if not os.path.isfile(x):
        # Argparse uses the ArgumentTypeError to give a rejection message like:
        # error: argument input: x does not exist
        raise argparse.ArgumentTypeError("{0} does not exist".format(x))
    return x

def create_bmp(args, framelist):

    (width, height) = dict_Resolutions.get(args.resolution, ("Resolution not supported", 0, 0))

    for idx, frame in enumerate(framelist):
        bitmap = np.zeros((height, width), dtype=np.uint8)

        h_idx = height - 1
        w_idx = 0

        # fill up bitmap array
        for pixel in frame.ui8Array:
            bitmap[h_idx, w_idx] = pixel
            if w_idx == width - 1:
                w_idx = 0
                h_idx -= 1
            else:
                w_idx += 1



        # h_idx = height - 1
        # w_idx = width - 1

        # # fill up bitmap array
        # for pixel in frame.ui8Array:
        #     bitmap[h_idx, w_idx] = pixel
        #     if w_idx == 0:
        #         w_idx = width - 1
        #         h_idx -= 1
        #     else:
        #         w_idx -= 1



        # h_idx = 0
        # w_idx = 0

        # # fill up bitmap array
        # for pixel in frame.ui8Array:
        #     bitmap[h_idx, w_idx] = pixel
        #     if w_idx == width - 1:
        #         w_idx = 0
        #         h_idx += 1
        #     else:
        #         w_idx += 1



        path = os.path.dirname(os.path.abspath(args.inputfile))
        basename = os.path.split(os.path.splitext(args.inputfile)[0])[-1]
        outputfile = os.path.join(path, basename + '_' + str(idx) + '.bmp')

        # print (bitmap)
        img = Image.fromarray(bitmap, 'L')
        img.save(outputfile)
        img.show()

        print ("%s created" % (basename + '_' + str(idx) + '.bmp'))

def do_convert(args):

    (width, height) = dict_Resolutions.get(args.resolution, ("Resolution not supported", 0, 0))

    with open(args.inputfile) as f:
        h_idx = 0
        w_idx = 0
        rawdata = None
        framestart = False
        framestop = False
        framelist = list()

        # collect all pixel data into an int array
        for line in f:
            if line == "+++ frame +++\n":
                framestart = True
                rawdata = RawData()
                continue
            elif line == '--- frame ---\n':
                framestop = True

            if framestart == True and framestop == False:
                linelist = re.findall(r"[\w']+", line)

                if len(linelist) != 17:
                    # drop this frame
                    framestart = False
                    continue

                for item in linelist[1 : ]:
                    rawdata.ui8Array.append(int(item, base=16))

            elif framestart == True and framestop == True:

                (address, length) = rawdata.ui8Array.buffer_info()

                if (length * rawdata.ui8Array.itemsize) != (height * width):
                    print ("Incorrect total data length %d" % length * rawdata.ui8Array.itemsize)
                else:
                    framelist.append(rawdata)
                    framestart = False
                    framestop = False

    create_bmp(args, framelist)

def main():
    parser = argparse.ArgumentParser(
        description = 'This program converts raw data from HM01B0 to a bmp file.')

    parser.add_argument('-i', '--input', 
                        dest        = 'inputfile',
                        required    = True,
                        help        = 'input file',
                        metavar     = 'FILE',
                        type        = check_file_existence
                        )

    parser.add_argument('-r', '--resolution', 
                        dest        = 'resolution',
                        required    = False,
                        help        = 'Resolution',
                        choices     = ['QVGA'],
                        default     = 'QVGA',
                        )

    parser.add_argument('-v', '--version',
                        help        = 'Program version',
                        action      = 'version',
                        version     = '%(prog)s {ver}'.format(ver = 'v%d.%d' %\
                            (VERSION, SUBVERSION))
                        )

    args = parser.parse_args()

    do_convert(args)

    print ("done!")


if __name__ == "__main__":
   main()