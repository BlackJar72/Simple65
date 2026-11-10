#include "VirtualCPU.h"

namespace ssixtyfive {

    void VirtualCPU::processOpcode(vbyte opcode) {
        curOpmode = (opmodes)(opcode.uval & opPrefix);
        curOperation = (opcodes)(opcode.uval & opBody);
    }


    // Does this even need to be separate from processOpcode?
    void VirtualCPU::ExecuteNextOpcode(vbyte code) {
        processOpcode(code);
        ip++;
        // TODO
    }


    // Load From Memory


    void VirtualCPU::ExecLda() {
        switch (curOpmode) {
        case opdirc:
            ac = ram[ip++];
            break;
        case opzero:
            ac = ram[ram[ip++].uval];
            break;
        case oprela:
            ac = ram[ip + ram[ip].sval];
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ac = ram[address];
            break;
        }
    }


    void VirtualCPU::ExecLdx() {
        switch (curOpmode) {
        case opdirc:
            rx = ram[ip++];
            break;
        case opzero:
            rx = ram[ram[ip++].uval];
            break;
        case oprela:
            rx = ram[ip + ram[ip].sval];
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            rx = ram[address];
            break;
        }
    }


    void VirtualCPU::ExecLdy() {
        switch (curOpmode) {
        case opdirc:
            ry = ram[ip++];
            break;
        case opzero:
            ry = ram[ram[ip++].uval];
            break;
        case oprela:
            ry = ram[ip + ram[ip].sval];
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ry = ram[address];
            break;
        }
    }


    // Store To Memory


    void VirtualCPU::ExecSta() {
        switch (curOpmode) {
        case opdirc:
            ram[ip++] = ac;
            break;
        case opzero:
            ram[ram[ip++].uval] = ac;
            break;
        case oprela:
            ram[ip + ram[ip].sval] = ac;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ram[address] = ac;
            break;
        }
    }


    void VirtualCPU::ExecStx() {
        switch (curOpmode) {
        case opdirc:
            ram[ip++] = rx;
            break;
        case opzero:
            ram[ram[ip++].uval] = rx;
            break;
        case oprela:
            ram[ip + ram[ip].sval] = rx;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ram[address] = rx;
            break;
        }
    }


    void VirtualCPU::ExecSty() {
        switch (curOpmode) {
        case opdirc:
            ram[ip++] = ry;
            break;
        case opzero:
            ram[ram[ip++].uval] = ry;
            break;
        case oprela:
            ram[ip + ram[ip].sval] = ry;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ram[address] = ry;
            break;
        }
    }

    // Register Transfers


    void VirtualCPU::ExecTax() { rx = ac; }
    void VirtualCPU::ExecTay() { ac = rx; }
    void VirtualCPU::ExecTxa() { ry = ac; }
    void VirtualCPU::ExecTya() { ac = ry; }


    // Stack Operations


    void VirtualCPU::ExecTsx() { rx = sp; }                              // Stack Pointer to Reg X
    void VirtualCPU::EexcTxs() { sp = rx; }                              // Reg X to Stack Pointer
    void VirtualCPU::ExecPha() { ram[StackOffset + sp.uval++] = ac; }    // Push Accumulator to Stack
    void VirtualCPU::ExecPhs() { ram[StackOffset + sp.uval++] = flags; } // Push CPU Status to Stack
    void VirtualCPU::ExecPla() { ac = ram[StackOffset + sp.uval--]; }    // Pop Accumulator from Stack
    void VirtualCPU::ExecPls() { flags = ram[StackOffset +sp.uval--]; }  // Pop CPU Status from Stack


    // Logical Operations


    void VirtualCPU::ExecAnd() { // Bitwise And
        switch (curOpmode) {
        case opdirc:
            ac.uval &= ram[ip++].uval;
            break;
        case opzero:
            ac.uval &= ram[ram[ip++].uval].uval;
            break;
        case oprela:
            ac.uval &= ram[ip + ram[ip].sval].uval;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ac.uval &= ram[address].uval;
            break;
        }
    }


    void VirtualCPU::ExecXor() { // Bitwise Exclusive Or
        switch (curOpmode) {
        case opdirc:
            ac.uval ^= ram[ip++].uval;
            break;
        case opzero:
            ac.uval ^= ram[ram[ip++].uval].uval;
            break;
        case oprela:
            ac.uval ^= ram[ip + ram[ip].sval].uval;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ac.uval ^= ram[address].uval;
            break;
        }
    }


    void VirtualCPU::ExecIor() { // Bitwise Includise Or
        switch (curOpmode) {
        case opdirc:
            ac.uval |= ram[ip++].uval;
            break;
        case opzero:
            ac.uval |= ram[ram[ip++].uval].uval;
            break;
        case oprela:
            ac.uval |= ram[ip + ram[ip].sval].uval;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            ac.uval |= ram[address].uval;
            break;
        }
    }


    void VirtualCPU::ExecBit() { // Non-Destructive Bit Mask Test (for Zero)
        unsigned char tmp;
        switch (curOpmode) {
        case opdirc:
            tmp = ac.uval & ram[ip++].uval;
            break;
        case opzero:
            tmp = ac.uval & ram[ram[ip++].uval].uval;
            break;
        case oprela:
            tmp = ac.uval * ram[ip + ram[ip].sval].uval;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (ram[ip++].uval) << 8;
            address |= ram[ip++].uval;
            tmp = ac.uval & ram[address].uval;
            break;
        }
        // Set overflow to bit 6
        flags.uval |= ((tmp & 0x01 << 6) >> 1);
        // Set negative to bit 7
        flags.uval |= ((tmp & 0x01 << 7) >> 1);
        // set zero flag if result is zero, unset it if not
        flags.uval = (flags.uval & ~(1u << 1)) | ((tmp == 0) << 1);
    }


    // Arithmatic Operations











}