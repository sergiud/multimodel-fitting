#!/usr/bin/env python2

# Copyright (C) 2016  Martin Stumpf
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

import argparse

parser = argparse.ArgumentParser(description='Converts any file to a C resource file.')
parser.add_argument('name', help='the name of the resource')
parser.add_argument('infile', type=argparse.FileType('rb'), help='the input file')
parser.add_argument('outfile', type=argparse.FileType('w'), help='the output file name')

args = parser.parse_args()


name = args.name
infile = args.infile
outfile = args.outfile

try:
    data = bytearray()
    data.extend(infile.read())

    outfile.write('#include <cstddef>\n')
    outfile.write('const unsigned char ' + name + '[] = {\n    ')

    i = 0
    for c in data :
        if i%8==0 and i>0:
            outfile.write(',\n    ')
        elif i>0:
            outfile.write(',')
        outfile.write(str(c))
        i = i + 1

    outfile.write('\n};\n')
    outfile.write('const size_t ' + name + '_len = ' + str(len(data)) + ';\n')
finally:
    outfile.close()
    infile.close()
