# Simple 65

This is a hobby and learning project, inspired by the "Simpletron" project used in some 
programming classes.  I started out wanting wanting to create a more complex and realistic 
toy virtual machine.  I also was interested in learning 6502 programming.  So, this mixes 
the two idea together.  Its not a serious project, has no timeline, and no guarantee of 
being completed; its done when (and if) its done.

Examples of systems that used the 6502 include:

* The Atari 400 and 800 home computers
* The Apple II
* The Commodore 64
* The BBC Micro
* The Atari 2600 console
* The original Nintendo Entertainment System (NES)

This is basically virtual machine based largely on the 6502, though not an attempt to create 
a faithful recreation of that CPU, much less an emulator for any particular computer.  The 
instruction set is mostly the same as the 6502, but with a few addition and minor simplifications. 
The main limitations of the 6502 will be retained, as part of its use is to experiment with 
the kind of programming and problem solving involved in creating software for those early home 
computers.  

However, a few instructions have been added based on the "Simpletron" (mostly, 
alternative branching instructions based directly on the accumulator).  Also, 
the mode in which instructions the take an operand have been simplified and 
to a consistent standard of...

* Direct, taking a single byte operand as the value
* Full (64 kb) memory range using two byte as operand and reference it as an address
* First 256 "Zero Page" range based on a single operand and reference it as an address
* Relative to the instruction pointer based on a signed byte (mostly for jumps / branching)

All non-IO instructions that can take an operator can be used in any of these modes, even when 
doing so would not make sense.

The biggest change is likely the IO (including video memory). In real 6502 systems the this is 
done through memory mapped IO, including mapping video memory to the same address space as 
ordinary memory.  For most, this limited the amount of memory to less than the 64 kb that the 
6502 could address.  The Commodore 64, which had a full 64 kb if RAM as a core selling point 
got around this through a kind of paging system, where byte 0 determined the whether the 
memory address would access real memory or memory mapped IO (including video memory).  Instead, 
I'm adding separate instructions for accessing video memory and for general IO, thus eliminating 
the need for paging while allowing a full 64 kb of main memory.

Video is unlike the computers that use the 6502, and in some ways more like early PCs.  This 
is a simplification, allowing video modes to be set and graphics to be written to the video 
memory buffer.  I may even include the ability to trigger updates, at least in some modes; 
how realistic I keep this is not decided (two smaller 16 color buffers alternated, or one 
256 color buffer that magically updates without extra memory).  Different real 6502 systems 
could vary wildly, often using separate proprietary hardware anyway (e.g., Atari's ANTIC and 
CTIA/GTIA chips, Nintendo's PPU, etc.).  The graphics are planned to be 320x180 pixels, with 
gray scale, 256 color immediate mode, and 16 color page-flipping mode (unless I go the less 
realistic route and allow magic flipping with imaginary pages).  Of course, a text mode will 
also be needed.  Technically, this is beyond the capabilities of any of systems actually using 
the 6502 back in the early 80s. 

Its is expected that implementing graphics and IO will be more challenging that the core system, 
which so far hasn't proven that difficult.  More challenging still, if I still with this long 
enough to mater, would be creating an assembler so you don't have to input raw machine language, 
possibly a compiler if I get really ambitious, and a way to have the virtual machine access 
some kind of (virtual) file system to save and load from within code.  These things are very 
far away, if I ever get to them.

QT is being used to allow panel representing the computer's screen as an image (or text window 
in text mode) along with other panels and controls for start and stopping the virtual machine,
loading files ("ROMs" or text files to generate them), and similar control features.

Hopefully this will be a fun learning project that will then be usable for other learning projects.
