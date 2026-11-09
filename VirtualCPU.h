#ifndef VIRTUALCPU_H
#define VIRTUALCPU_H

namespace ssixtyfive {

    // All our machine language instructions here
    // Note: Most of these instructions are borrow from the
    //       6502 instruction set, but the actual op codes
    //       are not but are created for this specifically.
    // The codes were organized with a different first digit,
    // but have since been compacted to keep all ones in the
    // first six bits, leaving bits 6 and 7 to hold data about
    // how the following bytes should be read (see opmodes
    // below).
    enum opcodes : unsigned char {
        // Load From Memory
        oplda = 0x00, // Load Accumulator
        opldx = 0x01, // Load Reg X
        opldy = 0x02, // Load Reg Y
        // Store To Memory
        opsta = 0x03, // Store Accumulator
        opstx = 0x04, // Store Reg X
        opsty = 0x05, // Store Reg y
        // Register Transfers
        optax = 0x06, // Accumulator to Reg X
        optay = 0x07, // Reg X to Accumulator
        optxa = 0x08, // Accumulator to Reg Y
        optya = 0x09, // Reg Y to Accumulator
        // Stack Operations
        optsx = 0x0A, // Stack Pointer to Reg X
        optxs = 0x0B, // Reg X to Stack Pointer
        oppha = 0x0C, // Push Accumulator to Stack
        opphs = 0x0D, // Push CPU Status to Stack
        oppla = 0x0E, // Pop Accumulator from Stack
        oppls = 0x0F, // Pop CPU Status from Stack
        // Logical Operations
        opand = 0x10, // Bitwise And
        opxor = 0x11, // Bitwise Exclusive Or
        opior = 0x12, // Bitwise Includise Or
        opbit = 0x13, // Non-Destructive Bit Mask Test (for Zero)
        // Arithmatic Operations
        opadd = 0x14, // Add Without Carry
        opadc = 0x15, // Add With Carry
        opsub = 0x16, // Add Without Carry
        opsbc = 0x17, // Add With Carry
        opcmp = 0x18, // Compare the Accumular with Value at Address
        opcpx = 0x19, // Compare Reg X with Value at Address
        opcpy = 0x1A, // Compare Reg Y with Value at Address
        // Shift Operations (all shift by one)
        opasl = 0x1B, // Arithmetic Shift Left (first set to 0, carry flag set to bit 7)
        oplsr = 0x1C, // Logical Shift Right (bit 7 set to zero, carry flat set to bit 0)
        oprol = 0x1D, // Rotate Left (carry flag to bit zero, bit seven to carry flag)
        opror = 0x1E, // Rotate Right (carry flag to bit seven, bit zero to carry flag)
        // Jumps, Calls, and Branches
        opjmp = 0x1F, // Jump to Location
        opjsr = 0x20, // Jump to Subroutine (push return address minus on to stack, then jump)
        oprts = 0x20, // Return from Subroutine (pop address from stack)
        opbcc = 0x21, // Branch if Carry Clear
        opbcs = 0x22, // Branch if Carry Set
        opbeq = 0x23, // Branch if Zero Flag Set
        opbne = 0x24, // Branch if Zero Flag Clear
        opbmi = 0x25, // Branch if Negative Flag Set (???)
        opbpl = 0x26, // Branch if Negative Flag Clear (???)
        opbvc = 0x27, // Branch if Overflow Flag Clear
        opbvs = 0x28, // Branch if Overflow Flag Set
        opjne = 0x29, // Jump if Accumulator Negative
        opjze = 0x2A, // Jump if Accumulator Zero
        // Special: Status Flags
        opclc = 0x2B, // Clear Carry Flag
        opcld = 0x2C, // Clear Decimal Mode Flag
        opcli = 0x2D, // Clear Interrupt Disable Flag
        opclv = 0x2E, // Clear Overflow Flag
        opsec = 0x2F, // Set Carry Flag
        opsed = 0x30, // Set Decimal Mode Flag
        opsei = 0x31, // Set Interrupt Disable Flag
        // Special: Other
        opbrk = 0x32, // Force and Interrupt
        oprti = 0x33, // Return from Interrupt
        opnop = 0x34, // Do Nothing (instruction pointrt increments)
        ophlt = 0x35, // Halt (stop all execution, effectively shut down virtual machine)
        opspg = 0x36, // Set Page Register (mostly to switch from normal to video or io-mapped memory)
        // IO (as an alternative to special memory pages)
        opwio = 0x37, // Write to IO
        oprio = 0x38, // Read from IO
        opwvi = 0x39, // Write to Video Memory
        oprvi = 0x3A  // Read from Video Memory
    };


    // Represents the first to bytes of an opcode, which act as a prefix
    // to determine how the following byte will be read, whether directly
    // as the value or as an address, and if as an address what kind.
    // These only apply to opcodes that take an operand; opcodes that don't
    // take and operand will ignore this, but should be set as 0x00 (opdirc)
    // for consistency.
    enum opmodes : unsigned char {
        opdirc = 0x00, // Following Byte is Value
        opzero = 0x40, // Following Byte is Memory Address (accessing "zero page)
        oprela = 0x80, // Next Byte Relative to Instruction Pointer (only intended for jumping / branching)
        opmain = 0xC0  // Following Two Bytes are 16 Bit Memory Address
    };


    enum iomodes : unsigned char {
        iozero = 0x00, // Accumulator to Single Byte "Zero Page" (0x0000 to 0x0100)
        ioacum = 0x40, // Accumulator to Full Two Byte Address (0x0000 to 0xFFFF)
        ioregx = 0x80, // Reg X to Full Two Byte Address
        ioregy = 0xC0  // Reg Y to Full Two Byte Address
    };


    union vbyte {
        signed char sval;
        unsigned char uval;
    };


    class VirtualCPU {
    public:
        void ExecuteNextOpcode(vbyte code);
        vbyte* GetMemoryAddress(unsigned short address);
        // Load From Memory
        void ExecLda(); // Load Accumulator
        void ExecLdx(); // Load Reg X
        void ExecLdy(); // Load Reg Y
        // Store To Memory
        void ExecSta(); // Store Accumulator
        void ExecStx(); // Store Reg X
        void ExecSty(); // Store Reg y
        // Register Transfers
        void ExecTax(); // Accumulator to Reg X
        void ExecTay(); // Reg X to Accumulator
        void ExecTxa(); // Accumulator to Reg Y
        void ExecTya(); // Reg Y to Accumulator
    private:
        unsigned short ip;    // Instruction Pointer (aka, program counter)
        vbyte  sp;    // Stack Pointer (points to location between 0x100 and 0x1FF)
        vbyte  ac;    // Accumulator (main register for doing things)
        vbyte  rx;    // Index Register X (a counter, also can be swapped with stack pointer sp)
        vbyte  ry;    // Index Register Y (an extra counter and place to stash a byte)
        vbyte  flags; // Status Flags
        vbyte  page;  // 64k Meta-Page of Memory (should be one real one, but mapped io and video memory get pages)

        // Masks for the opcode and its prefix (opmode)
        const unsigned char opPrefix = 0xC0;
        const unsigned char opBody   = 0x3F;

        // Masks for the page register
        const unsigned char mainPage  = 0x00;

        // Start of main memory (and other memory mapped data)
        const unsigned short highOffset  = 0x0200;

        opmodes curOpmode;
        opcodes curOperation;

        // FIXME? If I'm not going to use "paging" the arrays below can be greatly simplified! Should only
        //        need one full 65536 length array for all memory.  Any paging breaks that, though, as we
        //        always need access to the "zero page" and stack reguardless of "page," thus the existing
        //        system would need to be maintained.

        // The following two low-address memory areas are always available, not effected by page register
        vbyte zero[256];  // Memory Address 0x0000 to 0x00FF, as a kind of proto-cache (not exactly)
        vbyte stack[256]; // Memory Address 0x0100 to 0x01FF, as a hardware / main stack

        // The following high-address memory areas effected by page register, swapping availability
        vbyte mainmem[65536 - 512];  // The video memory, 0x0200 to 0xFFFF

        void processOpcode(vbyte opcode);
    };


};

#endif // VIRTUALCPU_H
