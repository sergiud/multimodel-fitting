#!/usr/bin/env python

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
parser.add_argument('name', nargs=1, help='the name of the resource')
parser.add_argument('infile', nargs=1, type=argparse.FileType('rb'), help='the input file')
parser.add_argument('outfile', nargs=1, type=argparse.FileType('wb'), help='the output file name')

args = parser.parse_args()

data = args.infile[0].read()
args.infile[0].close()

name = args.name[0]

outfile = args.outfile[0]
outfile.write('#include <cstddef>\n')
outfile.write('unsigned char ' + name + '[] = {\n    ')

i = 0
for c in data :
    if i%8==0 and i>0:
        outfile.write(',\n    ')
    elif i>0:
        outfile.write(',')
    outfile.write(str(ord(c)))
    i = i + 1

outfile.write('\n};\n')
outfile.write('size_t ' + name + '_len = ' + str(len(data)) + ';\n')

