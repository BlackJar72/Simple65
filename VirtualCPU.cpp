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


    // FIXME: If I'm not going to use "paging" (particularly not for IO)
    //        all memory should be in one array and this should not be needed!
    vbyte* VirtualCPU::GetMemoryAddress(unsigned short address) {
        // TODO: Handle "Paging" (i.e., access to mapped io and video memory)
        // ...but, should that be here or through a separate function?
        if(address < 0x0100) {
            return zero + address;
        } else if(address < 0x200) {
            return stack + address - 0x0100;
        } else return mainmem + address - 0x0200;
    }


    // Load From Memory


    void VirtualCPU::ExecLda() {
        // TODO: Handle "Paging" (i.e., access to mapped io and video memory)
        // FIXME: If I'm not going to use "paging" (particularly not for IO)
        //        this can be greatly simplified!
        switch (curOpmode) {
        case opdirc:
            ac = *GetMemoryAddress(ip++);
            break;
        case opzero:
            ac = zero[GetMemoryAddress(ip++)->uval];
            break;
        case oprela:
            ac = *GetMemoryAddress(ip - GetMemoryAddress(ip)->sval);
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (GetMemoryAddress(ip++)->uval) << 8;
            address |= GetMemoryAddress(ip++)->uval;
            ac = *GetMemoryAddress(address);
            break;
        }
    }


    void VirtualCPU::ExecLdx() {
        // TODO: Handle "Paging" (i.e., access to mapped io and video memory)
        // FIXME: If I'm not going to use "paging" (particularly not for IO)
        //        this can be greatly simplified!
        switch (curOpmode) {
        case opdirc:
            rx = *GetMemoryAddress(ip++);
            break;
        case opzero:
            rx = zero[GetMemoryAddress(ip++)->uval];
            break;
        case oprela:
            rx = *GetMemoryAddress(ip - GetMemoryAddress(ip)->sval);
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (GetMemoryAddress(ip++)->uval) << 8;
            address |= GetMemoryAddress(ip++)->uval;
            rx = *GetMemoryAddress(address);
            break;
        }
    }


    void VirtualCPU::ExecLdy() {
        // TODO: Handle "Paging" (i.e., access to mapped io and video memory)
        // FIXME: If I'm not going to use "paging" (particularly not for IO)
        //        this can be greatly simplified!
        switch (curOpmode) {
        case opdirc:
            ry = *GetMemoryAddress(ip++);
            break;
        case opzero:
            ry = zero[GetMemoryAddress(ip++)->uval];
            break;
        case oprela:
            ry = *GetMemoryAddress(ip - GetMemoryAddress(ip)->sval);
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (GetMemoryAddress(ip++)->uval) << 8;
            address |= GetMemoryAddress(ip++)->uval;
            ry = *GetMemoryAddress(address);
            break;
        }
    }


    // Store To Memory


    void VirtualCPU::ExecSta() {
        // FIXME: If I'm not going to use "paging" (particularly not for IO)
        //        this can be greatly simplified!
        switch (curOpmode) {
        case opdirc:
            *GetMemoryAddress(ip++) = ac;
            break;
        case opzero:
            zero[GetMemoryAddress(ip++)->uval] = ac;
            break;
        case oprela:
            *GetMemoryAddress(ip - GetMemoryAddress(ip)->sval) = ac;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (GetMemoryAddress(ip++)->uval) << 8;
            address |= GetMemoryAddress(ip++)->uval;
            *GetMemoryAddress(address) = ac;
            break;
        }
    }


    void VirtualCPU::ExecStx() {
        // FIXME: If I'm not going to use "paging" (particularly not for IO)
        //        this can be greatly simplified!
        switch (curOpmode) {
        case opdirc:
            *GetMemoryAddress(ip++) = rx;
            break;
        case opzero:
            zero[GetMemoryAddress(ip++)->uval] = rx;
            break;
        case oprela:
            *GetMemoryAddress(ip - GetMemoryAddress(ip)->sval) = rx;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (GetMemoryAddress(ip++)->uval) << 8;
            address |= GetMemoryAddress(ip++)->uval;
            *GetMemoryAddress(address) = rx;
            break;
        }
    }


    void VirtualCPU::ExecSty() {
        // FIXME: If I'm not going to use "paging" (particularly not for IO)
        //        this can be greatly simplified!
        switch (curOpmode) {
        case opdirc:
            *GetMemoryAddress(ip++) = ry;
            break;
        case opzero:
            zero[GetMemoryAddress(ip++)->uval] = ry;
            break;
        case oprela:
            *GetMemoryAddress(ip - GetMemoryAddress(ip)->sval) = ry;
            ip++;
            break;
        case opmain:
        default:
            unsigned short address = (GetMemoryAddress(ip++)->uval) << 8;
            address |= GetMemoryAddress(ip++)->uval;
            *GetMemoryAddress(address) = ry;
            break;
        }
    }

    // Register Transfers


    void VirtualCPU::ExecTax() { rx = ac; }
    void VirtualCPU::ExecTay() { ac = rx; }
    void VirtualCPU::ExecTxa() { ry = ac; }
    void VirtualCPU::ExecTya() { ac = ry; }


    // Stack Operations







}