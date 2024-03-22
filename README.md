# Rnd

The utility for the creation and verification of files filled with a pseudo-random sequence of bytes.

The program can be used to test the surface of flash drives, or to generate absolutely incompressible data.

## Usage

```
rnd ("create"|"verify") file_name [ file_size (=0 - infinite) [K|M|G] [ pseudo-random_seed_number (=1) ] ]
```

 * **create** - create a new file.
 * **verify** - verify a previously created file.
 * **file name** - a name of target file.
 * **file size** - a desired size of file in bytes, or an infinite size (i.e. "0") if not specified, can be used with "K", "M" or "G" suffixes.
 * **pseudo-random seed number** - a seed for number sequence, by default "1".

## Copyright

Copyright (C) 2024 Nikolay Raspopov <<raspopov@cherubicsoft.com>> \
https://github.com/raspopov/rnd

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
