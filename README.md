# MicroVolt Archiver

A MicroVolt Surge game encrypted archives extractor.

MVA is capable of both extracting (and decrypting) and repack the infamous protected archive 'cgd.dip'.

This file contains various binary structures used by the game for items properties, nVidia PhysX properties, server list, channel list, etc..

## Dependencies

Check VC++ Directories for library setup.

* ZipLib - https://bitbucket.org/wbenny/ziplib/wiki/Home

## Usage

For the help screen:
```
./MVA.exe -h
```

For the unpacking of the archive:
```
./MVA.exe -u cgd.dip
```

For the packing of the archive:
```
./MVA.exe -p cdg.dip_out
```

## Screenshots

### Help Screen

![HelpScr](https://github.com/d3v1l401/Microvolt-Archiver/blob/master/RepoMisc/helpscr.png)

### Unpacking Screen

![UnpackScr](https://github.com/d3v1l401/Microvolt-Archiver/blob/master/RepoMisc/unpackscr.png)

## License

GNU GPLv3

## Credits

d3vil401 <d3vil401(at)protonmail.com>
