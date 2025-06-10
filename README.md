# Varios8-wakeup
Wake up patches from VSynth engine for use with Varios8

![roland varios hardware](images/variosHW.png)

Roland VariOS hardware supports a `VPI-01 for VariOS.BIN` mode, marketed as a virtual Jupiter8 with MacOS9 editor, this is actually the full Roland VSynth VA (Virtual Analogue) engine, a monotimbral single part, single channel. 

![roland varios8 software editor](images/variosSW.png)

Any `.p00` file from a VSynth is compatibile with a few quirks here. Notably:

1. Any configured PCM SAMPLED or VARIPHRASE waveform is silenced (unless you use my `wakeup` tool here).

2. Few System Exclusive parameters map 1-to-1 onto the Jupiter8 MacOS9 editor but some do. The most useful mappings are listed below.

3. Multiple 'project' folders require the `V-Synth Project.ini` format described below.

# Wakeup!

This tool `wakeup` MODIFIES IN PLACE the binary data of the filename given as its argument.

It simply sets byte 273 and byte 306 (two sound sources set to VA mode).

Usage is typically with a batch process for all files in a 'PRJ' folder, e.g.

$ cd /path/to/wakeup
$ for i in path/to/library.prj/Patches/*.P00 do; echo "$i"; ./wakeup "$i"; done;

# System Exclusive and control messages

When the VariOS hardware is running `VPI-01 for VariOS.BIN` it will parse a few System Exclusive and Midi Channel Control messages that are useful for adjusting the behaviour of patches originally configured on a Roland VSynth in VA (Virtual Analogue) mode.


|description   |parameter |hexadecimal (cs = roland checksum)      |cs (hex)|
|--------------|----------|----------------------------------------|--------|
|mfx on/off    | nn = 0,1 | F0 41 10 00 6D 12 10 00 00 1D nn cs F7 | 53, 54 |
|chorus on/off | nn = 0,1 | F0 41 10 00 6D 12 10 00 00 1E nn cs F7 | 52, 51 |
|reverb on/off | nn = 0,1 | F0 41 10 00 6D 12 10 00 00 1F nn cs F7 | 51, 50 |
|sync int/ext  | nn = 0,1 | F0 41 10 00 6D 12 01 00 00 07 nn cs F7 | 78, 77 |
|arpeg on/off  | nn = 0,1 | F0 41 10 00 6D 12 10 00 00 20 nn cs F7 | 50, 4F |

|--------|-----------|
|midi cc#|description|
|--------|-----------|
|CC1     |Modulation |
|CC2     |K1         |
|CC5     |Glide time |
|CC7     |Volume     |
|CC10    |Pan        |
|CC11    |Expression |
|CC16    |DBEAM L    |
|  :     |   :       |
|CC19    |   .       |
|CC34    |K2         |
|CC48    |DBEAM R    |
|  :     |   :       |
|CC51    |   .       |
|CC64    |Pedal 1    |
|CC65    |Pedal 2    |
|CC80    |TIMETRIP   |
|  :     |   :       |
|CC83    |   .       |
|CC91    |CHORUS FX  |
|CC93    |REVERB FX  |
|--------|-----------|

# Project folders

When the VariOS hardware loads the `VPI-01 for VariOS.BIN` file, it searches for patch files in the  placed in the root of the VariOS flash memory will target the `VA.prj` folder but you may RE-TARGET this search to another folder with a `V-Synth Project.ini` file in the format given below.

```
[current project]
drive="PC Card"
path="Example.prj\"
; path must end with a \ backslash
```
