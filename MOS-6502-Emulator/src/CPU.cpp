/**
  ******************************************************************************
  * @file    CPU.cpp
  * @author  Josh Haden
  * @version V0.0.1
  * @date    05 NOV 2024
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "CPU.hpp"
#include "FancyConsole.hpp"

/* Global Variables ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const CPU6502::INSTRUCTION_T CPU6502::intruction_table[] =
{
    { 0x00, &CPU6502::BRK,   &CPU6502::implied,         1, 7 },
    { 0x01, &CPU6502::ORA,   &CPU6502::indexedIndirect, 2, 6 },
    { 0x05, &CPU6502::ORA,   &CPU6502::zeroPage,        2, 3 },
    { 0x06, &CPU6502::ASL,   &CPU6502::zeroPage,        2, 5 },
    { 0x08, &CPU6502::PHP,   &CPU6502::implied,         1, 3 },
    { 0x09, &CPU6502::ORA,   &CPU6502::immediate,       2, 2 },
    { 0x0A, &CPU6502::ASL_A, &CPU6502::implied,         1, 2 },
    { 0x0D, &CPU6502::ORA,   &CPU6502::absolute,        3, 4 },
    { 0x0E, &CPU6502::ASL,   &CPU6502::absolute,        3, 6 },
    
    { 0x10, &CPU6502::BPL,   &CPU6502::relative,        2, 2 },
    { 0x11, &CPU6502::ORA,   &CPU6502::indirectIndexed, 2, 2 },
    { 0x15, &CPU6502::ORA,   &CPU6502::zeroPageX,       2, 2 },
    { 0x16, &CPU6502::ASL,   &CPU6502::zeroPageX,       2, 2 },
    { 0x18, &CPU6502::CLC,   &CPU6502::implied,         1, 2 },
    { 0x19, &CPU6502::ORA,   &CPU6502::absoluteY,       3, 2 },
    { 0x1D, &CPU6502::ORA,   &CPU6502::absoluteX,       3, 2 },
    { 0x1E, &CPU6502::ASL,   &CPU6502::absoluteX,       3, 2 },

    { 0x20, &CPU6502::JSR,   &CPU6502::absolute,        3, 2 },
    { 0x21, &CPU6502::AND,   &CPU6502::indexedIndirect, 2, 2 },
    { 0x24, &CPU6502::BIT,   &CPU6502::zeroPage,        2, 2 },
    { 0x25, &CPU6502::AND,   &CPU6502::zeroPage,        2, 3 },
    { 0x26, &CPU6502::ROL,   &CPU6502::zeroPage,        2, 2 },
    { 0x28, &CPU6502::PLP,   &CPU6502::implied,         1, 2 },
    { 0x29, &CPU6502::AND,   &CPU6502::immediate,       2, 2 },
    { 0x2A, &CPU6502::ROL_A, &CPU6502::implied,         1, 2 },
    { 0x2C, &CPU6502::BIT,   &CPU6502::absolute,        3, 2 },
    { 0x2D, &CPU6502::AND,   &CPU6502::absolute,        3, 2 },
    { 0x2E, &CPU6502::ROL,   &CPU6502::absolute,        3, 2 },

    { 0x30, &CPU6502::BMI,   &CPU6502::relative,        2, 2 },
    { 0x21, &CPU6502::JSR,   &CPU6502::indexedIndirect, 2, 2 },
    { 0x35, &CPU6502::AND,   &CPU6502::zeroPageX,       2, 4 },
    { 0x36, &CPU6502::ROL,   &CPU6502::zeroPageX,       2, 2 },
    { 0x38, &CPU6502::SEC,   &CPU6502::implied,         1, 2 },
    { 0x39, &CPU6502::AND,   &CPU6502::absoluteY,       3, 2 },
    { 0x3D, &CPU6502::AND,   &CPU6502::absoluteX,       3, 4 },
    { 0x3E, &CPU6502::ROL,   &CPU6502::absoluteX,       3, 2 },

    { 0x40, &CPU6502::RTI,   &CPU6502::implied,         1, 2 },
    { 0x41, &CPU6502::EOR,   &CPU6502::indexedIndirect, 2, 2 },
    { 0x45, &CPU6502::EOR,   &CPU6502::zeroPageX,       2, 2 },
    { 0x46, &CPU6502::LSR,   &CPU6502::zeroPage,        2, 2 },
    { 0x48, &CPU6502::PHA,   &CPU6502::implied,         1, 2 },
    { 0x49, &CPU6502::EOR,   &CPU6502::immediate,       2, 2 },
    { 0x4A, &CPU6502::LSR,   &CPU6502::implied,         1, 2 },
    { 0x4C, &CPU6502::JMP,   &CPU6502::absolute,        3, 2 },
    { 0x4D, &CPU6502::EOR,   &CPU6502::absolute,        3, 2 },
    { 0x4E, &CPU6502::LSR,   &CPU6502::absolute,        3, 2 },

    { 0x50, &CPU6502::BVC,   &CPU6502::relative,        2, 2 },
    { 0x51, &CPU6502::EOR,   &CPU6502::indirectIndexed, 2, 2 },
    { 0x55, &CPU6502::EOR,   &CPU6502::zeroPageX,       2, 2 },
    { 0x56, &CPU6502::LSR,   &CPU6502::zeroPageX,       2, 2 },
    { 0x58, &CPU6502::CLI,   &CPU6502::implied,         1, 2 },
    { 0x59, &CPU6502::EOR,   &CPU6502::absoluteY,       3, 2 },
    { 0x5D, &CPU6502::EOR,   &CPU6502::absoluteX,       3, 2 },
    { 0x5E, &CPU6502::LSR,   &CPU6502::absoluteX,       3, 2 },

    { 0x60, &CPU6502::RTS,   &CPU6502::implied,         1, 2 },
    { 0x61, &CPU6502::ADC,   &CPU6502::indexedIndirect, 2, 6 },
    { 0x65, &CPU6502::ADC,   &CPU6502::zeroPage,        2, 3 },
    { 0x66, &CPU6502::ROR,   &CPU6502::zeroPage,        2, 2 },
    { 0x68, &CPU6502::PLA,   &CPU6502::implied,         1, 2 },
    { 0x69, &CPU6502::ADC,   &CPU6502::immediate,       2, 2 },
    { 0x6A, &CPU6502::ROR_A, &CPU6502::implied,         1, 2 },
    { 0x6C, &CPU6502::JMP,   &CPU6502::indirect,        2, 2 },
    { 0x6D, &CPU6502::ADC,   &CPU6502::absolute,        3, 4 },
    { 0x6E, &CPU6502::ROR,   &CPU6502::absolute,        3, 2 },

    { 0x70, &CPU6502::BVS,   &CPU6502::relative,        2, 2 },
    { 0x71, &CPU6502::ADC,   &CPU6502::indirectIndexed, 2, 5 },
    { 0x75, &CPU6502::ADC,   &CPU6502::zeroPageX,       2, 4 },
    { 0x76, &CPU6502::ROR,   &CPU6502::zeroPageX,       2, 2 },
    { 0x78, &CPU6502::PLA,   &CPU6502::implied,         1, 2 },
    { 0x79, &CPU6502::ADC,   &CPU6502::absoluteY,       3, 4 },
    { 0x7D, &CPU6502::ADC,   &CPU6502::absoluteX,       3, 4 },
    { 0x7E, &CPU6502::ROR,   &CPU6502::absoluteX,       3, 2 },

    { 0x81, &CPU6502::STA,   &CPU6502::indexedIndirect, 2, 2 },
    { 0x84, &CPU6502::STY,   &CPU6502::zeroPage,        2, 2 },
    { 0x85, &CPU6502::STA,   &CPU6502::zeroPage,        2, 3 },
    { 0x86, &CPU6502::STX,   &CPU6502::zeroPage,        2, 2 },
    { 0x88, &CPU6502::DEY,   &CPU6502::implied,         1, 2 },
    { 0x8A, &CPU6502::TXA,   &CPU6502::implied,         1, 2 },
    { 0x8C, &CPU6502::STY,   &CPU6502::absolute,        3, 2 },
    { 0x8D, &CPU6502::STA,   &CPU6502::absolute,        3, 4 },
    { 0x8E, &CPU6502::STX,   &CPU6502::absolute,        3, 2 },

    { 0x90, &CPU6502::BCC,   &CPU6502::relative,        2, 2 },
    { 0x91, &CPU6502::STA,   &CPU6502::indirectIndexed, 2, 2 },
    { 0x94, &CPU6502::STY,   &CPU6502::zeroPageX,       2, 2 },
    { 0x95, &CPU6502::STA,   &CPU6502::zeroPageX,       2, 2 },
    { 0x96, &CPU6502::STX,   &CPU6502::zeroPageY,       2, 2 },
    { 0x98, &CPU6502::TYA,   &CPU6502::implied,         1, 2 },
    { 0x99, &CPU6502::STA,   &CPU6502::absoluteY,       3, 2 },
    { 0x9A, &CPU6502::TXS,   &CPU6502::implied,         1, 2 },
    { 0x9D, &CPU6502::STA,   &CPU6502::absoluteX,       3, 2 },

    { 0xA0, &CPU6502::LDY,   &CPU6502::immediate,       2, 2 },
    { 0xA1, &CPU6502::LDA,   &CPU6502::indexedIndirect, 2, 2 },
    { 0xA2, &CPU6502::LDX,   &CPU6502::immediate,       2, 2 },
    { 0xA4, &CPU6502::LDY,   &CPU6502::zeroPage,        2, 2 },
    { 0xA5, &CPU6502::LDA,   &CPU6502::zeroPage,        2, 3 },
    { 0xA6, &CPU6502::LDX,   &CPU6502::zeroPage,        2, 2 },
    { 0xA8, &CPU6502::TAY,   &CPU6502::implied,         1, 2 },
    { 0xA9, &CPU6502::LDA,   &CPU6502::immediate,       2, 2 },
    { 0xAA, &CPU6502::TAX,   &CPU6502::implied,         1, 2 },
    { 0xAC, &CPU6502::LDY,   &CPU6502::absolute,        3, 2 },
    { 0xAD, &CPU6502::LDA,   &CPU6502::absolute,        3, 2 },
    { 0xAE, &CPU6502::LDX,   &CPU6502::absolute,        3, 2 },

    { 0xB0, &CPU6502::BCS,   &CPU6502::relative,        2, 2 },
    { 0xB1, &CPU6502::LDA,   &CPU6502::indirectIndexed, 2, 2 },
    { 0xB4, &CPU6502::LDY,   &CPU6502::zeroPageX,       2, 2 },
    { 0xB5, &CPU6502::LDA,   &CPU6502::zeroPageX,       2, 2 },
    { 0xB6, &CPU6502::LDX,   &CPU6502::zeroPageY,       2, 2 },
    { 0xB8, &CPU6502::CLV,   &CPU6502::implied,         1, 2 },
    { 0xB9, &CPU6502::LDA,   &CPU6502::absoluteY,       3, 2 },
    { 0xBA, &CPU6502::TSX,   &CPU6502::implied,         1, 2 },
    { 0xBC, &CPU6502::LDY,   &CPU6502::absoluteX,       3, 2 },
    { 0xBD, &CPU6502::LDA,   &CPU6502::absoluteX,       3, 2 },
    { 0xBE, &CPU6502::LDX,   &CPU6502::absoluteY,       3, 2 },

    { 0xC0, &CPU6502::CPY,   &CPU6502::immediate,       2, 2 },
    { 0xC1, &CPU6502::CMP,   &CPU6502::indexedIndirect, 2, 2 },
    { 0xC4, &CPU6502::CPY,   &CPU6502::zeroPage,        2, 2 },
    { 0xC5, &CPU6502::CMP,   &CPU6502::zeroPage,        2, 2 },
    { 0xC6, &CPU6502::DEC,   &CPU6502::zeroPage,        2, 2 },
    { 0xC8, &CPU6502::INY,   &CPU6502::implied,         1, 2 },
    { 0xC9, &CPU6502::CMP,   &CPU6502::immediate,       2, 2 },
    { 0xCA, &CPU6502::DEX,   &CPU6502::implied,         1, 2 },
    { 0xCC, &CPU6502::CPY,   &CPU6502::absolute,        3, 2 },
    { 0xCD, &CPU6502::CMP,   &CPU6502::absolute,        3, 2 },
    { 0xCE, &CPU6502::DEC,   &CPU6502::absolute,        3, 2 },

    { 0xD0, &CPU6502::BNE,   &CPU6502::relative,        2, 2 },
    { 0xD1, &CPU6502::CMP,   &CPU6502::indirectIndexed, 2, 2 },
    { 0xD5, &CPU6502::CMP,   &CPU6502::zeroPageX,       2, 2 },
    { 0xD6, &CPU6502::DEC,   &CPU6502::zeroPageX,       2, 2 },
    { 0xD8, &CPU6502::CLD,   &CPU6502::implied,         1, 2 },
    { 0xD9, &CPU6502::CMP,   &CPU6502::absoluteY,       3, 2 },
    { 0xDD, &CPU6502::CMP,   &CPU6502::absoluteX,       3, 2 },
    { 0xDE, &CPU6502::DEC,   &CPU6502::absoluteX,       3, 2 },

    { 0xE0, &CPU6502::CPX,   &CPU6502::immediate,       2, 2 },
    { 0xE1, &CPU6502::SBC,   &CPU6502::indexedIndirect, 2, 2 },
    { 0xE4, &CPU6502::CPX,   &CPU6502::zeroPage,        2, 2 },
    { 0xE5, &CPU6502::SBC,   &CPU6502::zeroPage,        2, 2 },
    { 0xE6, &CPU6502::INC,   &CPU6502::zeroPage,        2, 2 },
    { 0xE8, &CPU6502::INX,   &CPU6502::implied,         1, 2 },
    { 0xE9, &CPU6502::SBC,   &CPU6502::immediate,       2, 2 },
    { 0xEA, &CPU6502::NOP,   &CPU6502::implied,         1, 2 },
    { 0xEC, &CPU6502::CPX,   &CPU6502::absolute,        3, 2 },
    { 0xED, &CPU6502::SBC,   &CPU6502::absolute,        3, 2 },
    { 0xEE, &CPU6502::INC,   &CPU6502::absolute,        3, 2 },

    { 0xF0, &CPU6502::BEQ,   &CPU6502::relative,        2, 2 },
    { 0xF1, &CPU6502::SBC,   &CPU6502::indirectIndexed, 2, 2 },
    { 0xF5, &CPU6502::SBC,   &CPU6502::zeroPageX,       2, 2 },
    { 0xF6, &CPU6502::INC,   &CPU6502::zeroPageX,       2, 2 },
    { 0xF8, &CPU6502::SED,   &CPU6502::implied,         1, 2 },
    { 0xF9, &CPU6502::SBC,   &CPU6502::absoluteY,       3, 2 },
    { 0xFD, &CPU6502::SBC,   &CPU6502::absoluteX,       3, 2 },
    { 0xFE, &CPU6502::INC,   &CPU6502::absoluteX,       3, 2 },

    { 0xFF, nullptr,         nullptr,                   0, 0 }
};

/* Private function prototypes -----------------------------------------------*/



/******************************************************************************/
/******************************************************************************/
/* Public Functions                                                           */
/******************************************************************************/
/******************************************************************************/


// Helper functions
void CPU6502::setZN(BYTE_T value)
{
    SR.Z = (value == 0);
    SR.N = (value & 0x80) != 0;
}

BYTE_T CPU6502::readByte(ADDRESS_T addr)
{
    return memory[addr];
}

ADDRESS_T CPU6502::readWord(ADDRESS_T addr)
{
    return readByte(addr) | (readByte(addr + 1) << 8);
}

void CPU6502::writeByte(ADDRESS_T addr, BYTE_T value)
{
    memory[addr] = value;
}

void CPU6502::push(BYTE_T value)
{
    writeByte(0x100 | SP, value);
    SP--;
}

void CPU6502::pushWord(WORD_T value)
{
    push((value >> 8) & 0xFF);
    push(value & 0xFF);
}

BYTE_T CPU6502::pull()
{
    SP++;
    return readByte(0x100 | SP);
}

WORD_T CPU6502::pullWord()
{
    ADDRESS_T low = pull();
    ADDRESS_T high = pull();
    return (high << 8) | low;
}

void CPU6502::initPC(ADDRESS_T startAddr)
{
    PC = startAddr;
}

// Addressing modes
ADDRESS_T CPU6502::implied() { return 0; }

ADDRESS_T CPU6502::immediate()
{
    return PC + 1;
}

ADDRESS_T CPU6502::absolute()
{
    return readByte(PC + 1) | (readByte(PC + 2) << 8);
}

ADDRESS_T CPU6502::zeroPage()
{
    return readByte(PC + 1);
}

ADDRESS_T CPU6502::zeroPageX()
{
    return (readByte(PC + 1) + XR) & 0xFF;
}

ADDRESS_T CPU6502::zeroPageY()
{
    return (readByte(PC + 1) + YR) & 0xFF;
}

ADDRESS_T CPU6502::absoluteX()
{
    return (readByte(PC + 1) + XR) & 0xFF;
}

ADDRESS_T CPU6502::absoluteY()
{
    return (readByte(PC + 1) + YR) & 0xFF;
}

ADDRESS_T CPU6502::indirect()
{
    ADDRESS_T ptr = readWord(PC);
    //PC += 2;
    // Simulate 6502 bug: if ptr ends in FF, high byte is read from xx00 instead of xx+1:00
    if ((ptr & 0xFF) == 0xFF)
    {
        return readByte(ptr) | (readByte(ptr & 0xFF00) << 8);
    }
    return readWord(ptr);
}

ADDRESS_T CPU6502::indexedIndirect()
{  
    BYTE_T zp = (readByte(PC++) + XR) & 0xFF; // (zeropage,X)
    return readByte(zp) | (readByte((zp + 1) & 0xFF) << 8);
}

ADDRESS_T CPU6502::indirectIndexed()
{
    BYTE_T zp = readByte(PC++); // (zeropage),Y
    ADDRESS_T base = readByte(zp) | (readByte((zp + 1) & 0xFF) << 8);
    ADDRESS_T addr = base + YR;
    if ((base & 0xFF00) != (addr & 0xFF00))
    {
        cycles++;
    }
    return addr;
}

ADDRESS_T CPU6502::relative()
{
    BYTE_T offset = readByte(PC + 1);
    ADDRESS_T addr = PC + static_cast<int8_t>(offset);
    if ((PC & 0xFF00) != (offset & 0xFF00))
    {
        cycles++;
    }
    return addr;
}

// Instructions
// Load/Store Operations
void CPU6502::LDA(ADDRESS_T *addr) { AC = readByte(*addr); setZN(AC); }
//void CPU6502::LDA_IMM(ADDRESS_T addr) { A = readByte(PC++); setZN(A); }
void CPU6502::LDX(ADDRESS_T *addr) { XR = readByte(*addr); setZN(XR); }
void CPU6502::LDY(ADDRESS_T *addr) { YR = readByte(*addr); setZN(YR); }
void CPU6502::STA(ADDRESS_T *addr) { writeByte(*addr, AC); }
void CPU6502::STX(ADDRESS_T *addr) { writeByte(*addr, XR); }
void CPU6502::STY(ADDRESS_T *addr) { writeByte(*addr, YR); }

// Register Transfers
void CPU6502::TAX(ADDRESS_T*) { XR = AC; setZN(XR); }
void CPU6502::TAY(ADDRESS_T*) { YR = AC; setZN(YR); }
void CPU6502::TXA(ADDRESS_T*) { AC = XR; setZN(AC); }
void CPU6502::TYA(ADDRESS_T*) { AC = YR; setZN(AC); }

// Stack Operations
void CPU6502::TSX(ADDRESS_T*) { XR = SP; setZN(XR); }
void CPU6502::TXS(ADDRESS_T*) { SP = XR; }
void CPU6502::PHA(ADDRESS_T*) { push(AC); }
void CPU6502::PHP(ADDRESS_T*) { push(getStatus() | 0x10); }
void CPU6502::PLA(ADDRESS_T*) { AC = pull(); setZN(AC); }
void CPU6502::PLP(ADDRESS_T*) { setStatus(pull() & 0xEF); }

// Logical Operations
void CPU6502::AND(ADDRESS_T *addr) { AC &= readByte(*addr); setZN(AC); }
void CPU6502::EOR(ADDRESS_T *addr) { AC ^= readByte(*addr); setZN(AC); }
void CPU6502::ORA(ADDRESS_T *addr) { AC |= readByte(*addr); setZN(AC); }
void CPU6502::BIT(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr);
    SR.Z = (AC & value) == 0;
    SR.N = (value & 0x80) != 0;
    SR.V = (value & 0x40) != 0;
}

// Arithmetic Operations
void CPU6502::ADC(ADDRESS_T *addr)
{
    //BYTE_T value = readByte(*addr);
    //WORD_T result;

    //if (SR.D)
    //{  // Decimal mode
    //    ADDRESS_T lo = (AC & 0x0F) + (value & 0x0F) + SR.C;
    //    ADDRESS_T hi = (AC & 0xF0) + (value & 0xF0);
    //    if (lo > 0x09)
    //    {
    //        hi += 0x10;
    //        lo += 0x06;
    //    }
    //    if (hi > 0x90) hi += 0x60;

    //    SR.C = (hi & 0xFF00) > 0;
    //    result = (lo & 0x0F) | (hi & 0xF0);
    //}
    //else
    //{  // Binary mode
    //    result = AC + value + SR.C;
    //    SR.C = result > 0xFF;
    //}

    //SR.V = (~(AC ^ value) & (AC ^ result) & 0x80) != 0;
    //AC = result & 0xFF;
    //setZN(AC);
    BYTE_T value = readByte(*addr);
    WORD_T result = AC + value + SR.C;

    SR.C = result > 0xFF;
    SR.V = (~(AC ^ value) & (AC ^ result) & 0x80) != 0;
    AC = result & 0xFF;
    setZN(AC);
}

void CPU6502::SBC(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr);
    ADDRESS_T result;

    if (SR.D)
    {  // Decimal mode
        ADDRESS_T lo = (AC & 0x0F) - (value & 0x0F) - !SR.C;
        ADDRESS_T hi = (AC & 0xF0) - (value & 0xF0);
        if (lo & 0x10)
        {
            hi -= 0x10;
            lo -= 0x06;
        }
        if (hi & 0x0100) hi -= 0x60;

        result = (lo & 0x0F) | (hi & 0xF0);
        SR.C = (hi & 0xFF00) == 0;
    }
    else
    {  // Binary mode
        result = AC - value - !SR.C;
        SR.C = result < 0x100;
    }

    SR.V = ((AC ^ value) & (AC ^ result) & 0x80) != 0;
    AC = result & 0xFF;
    setZN(AC);
}

// Increments & Decrements
void CPU6502::INC(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr) + 1;
    writeByte(*addr, value);
    setZN(value);
}

void CPU6502::INX(ADDRESS_T*) { XR++; setZN(XR); }
void CPU6502::INY(ADDRESS_T*) { YR++; setZN(YR); }
void CPU6502::DEC(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr) - 1;
    writeByte(*addr, value);
    setZN(value);
}
void CPU6502::DEX(ADDRESS_T*) { XR--; setZN(XR); }
void CPU6502::DEY(ADDRESS_T*) { YR--; setZN(YR); }

// Shifts
void CPU6502::ASL(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr);
    SR.C = (value & 0x80) != 0;
    value <<= 1;
    writeByte(*addr, value);
    setZN(value);
}

void CPU6502::ASL_A(ADDRESS_T*)
{
    SR.C = (AC & 0x80) != 0;
    AC <<= 1;
    setZN(AC);
}

void CPU6502::LSR(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr);
    SR.C = value & 1;
    value >>= 1;
    writeByte(*addr, value);
    setZN(value);
}

void CPU6502::LSR_A(ADDRESS_T*)
{
    SR.C = AC & 1;
    AC >>= 1;
    setZN(AC);
}

void CPU6502::ROL(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr);
    BYTE_T newC = value & 0x80;
    value = (value << 1) | SR.C;
    SR.C = newC;
    writeByte(*addr, value);
    setZN(value);
}

void CPU6502::ROL_A(ADDRESS_T*)
{
    BYTE_T newC = AC & 0x80;
    AC = (AC << 1) | SR.C;
    SR.C = newC;
    setZN(AC);
}

void CPU6502::ROR(ADDRESS_T *addr)
{
    BYTE_T value = readByte(*addr);
    BYTE_T newC = value & 1;
    value = (value >> 1) | (SR.C << 7);
    SR.C = newC;
    writeByte(*addr, value);
    setZN(value);
}

void CPU6502::ROR_A(ADDRESS_T*)
{
    BYTE_T newC = AC & 1;
    AC = (AC >> 1) | (SR.C << 7);
    SR.C = newC;
    setZN(AC);
}

// Jumps & Calls
void CPU6502::JMP(ADDRESS_T *addr) { PC = *addr; }
void CPU6502::JSR(ADDRESS_T *addr)
{
    pushWord(PC + 2);
    PC = *addr;
}

void CPU6502::RTS(ADDRESS_T*) { PC = pullWord() - 1; }
void CPU6502::RTI(ADDRESS_T*)
{
    setStatus(pull());
    PC = pullWord();
}

// Branches
void CPU6502::branch(ADDRESS_T *addr, bool condition)
{
    if (condition)
    {
        //int8_t offset = static_cast<int8_t>(*addr); // Treat addr as signed byte for offset

        // Calculate the target PC as the current PC + 2 (size of branch instruction) + the offset
        PC = *addr;

        // Branch taken, so increment cycles by 1 (additional cycle is required)
        cycles++;
    }
}

void CPU6502::BCC(ADDRESS_T *addr) { branch(addr, !SR.C); }
void CPU6502::BCS(ADDRESS_T *addr) { branch(addr, SR.C); }
void CPU6502::BEQ(ADDRESS_T *addr) { branch(addr, SR.Z); }
void CPU6502::BNE(ADDRESS_T *addr) { branch(addr, !SR.Z); }
void CPU6502::BVC(ADDRESS_T *addr) { branch(addr, !SR.V); }
void CPU6502::BVS(ADDRESS_T *addr) { branch(addr, SR.V); }
void CPU6502::BPL(ADDRESS_T *addr) { branch(addr, !SR.N); }
void CPU6502::BMI(ADDRESS_T *addr) { branch(addr, SR.N); }

// Status Flag Changes  
void CPU6502::CLC(ADDRESS_T*) { SR.C = 0; }
void CPU6502::CLD(ADDRESS_T*) { SR.D = 0; }
void CPU6502::CLI(ADDRESS_T*) { SR.I = 0; }
void CPU6502::CLV(ADDRESS_T*) { SR.V = 0; }
void CPU6502::SEC(ADDRESS_T*) { SR.C = 1; }
void CPU6502::SED(ADDRESS_T*) { SR.D = 1; }
void CPU6502::SEI(ADDRESS_T*) { SR.I = 1; }

// Compare Operations
void CPU6502::compare(BYTE_T reg, BYTE_T value)
{
    WORD_T result = static_cast<int16_t>(reg) - static_cast<int16_t>(value);

    SR.N = (result & 0x80);
    SR.Z = (!(result & 0xFF));
    SR.C = result < 0x100;
}

void CPU6502::CMP(ADDRESS_T *addr) { compare(AC, readByte(*addr)); }
void CPU6502::CPX(ADDRESS_T *addr) { compare(XR, readByte(*addr)); }
void CPU6502::CPY(ADDRESS_T *addr) { compare(YR, readByte(*addr)); }

// System Functions
void CPU6502::BRK(ADDRESS_T*)
{
    PC++;
    pushWord(PC);
    PHP(0);
    SR.I = 1;
    PC = readWord(0xFFFE);
}

void CPU6502::NOP(ADDRESS_T*) {}

CPU6502::CPU6502()
{
    reset();
}

void CPU6502::reset()
{
    AC = XR = YR = 0;
    SP = 0xFF;
    SR = { 0 };
    SR.U = 1;
    PC = readWord(0xFFFC);  // Reset vector
}

// Status register helpers
BYTE_T CPU6502::getStatus() const
{
    return (SR.C) | (SR.Z << 1) | (SR.I << 2) | (SR.D << 3) |
        (SR.B << 4) | (1 << 5) | (SR.V << 6) | (SR.N << 7);
}

void CPU6502::setStatus(BYTE_T status)
{
    SR.C = status & 0x01;
    SR.Z = (status >> 1) & 0x01;
    SR.I = (status >> 2) & 0x01;
    SR.D = (status >> 3) & 0x01;
    SR.B = (status >> 4) & 0x01;
    SR.U = 1;
    SR.V = (status >> 6) & 0x01;
    SR.N = (status >> 7) & 0x01;
}

// Memory interface
void CPU6502::flashProgram(const BYTE_T* program, size_t size, ADDRESS_T addr)
{
    for (ADDRESS_T i = 0; i < size; i++)
    {
        writeByte(addr + i, program[i]);
    }
    PC = addr;
}

void CPU6502::step()
{
    OPCODE_T opcode = readByte(PC);

    // Find matching instruction
    const INSTRUCTION_T* instr = nullptr;

    for (const auto& entry : intruction_table)
    {
        if (entry.opcode == opcode)
        {
            instr = &entry;
            break;
        }

        if (entry.opcode == 0xFF) break; // End of table marker
    }

    if (instr && instr->inst_fn)
    {
        ADDRESS_T addr = 0;

        if (instr->addr_fn)
        {
            addr = (this->*(instr->addr_fn))();
        }

        (this->*(instr->inst_fn))(&addr);

        PC += instr->bytes;
    }
    else
    {
        // Handle invalid opcode
        PC++;
    }
}

void CPU6502::execute()
{

}

// Execute multiple steps
void CPU6502::run(int numCycles)
{
    int totalCycles = 0;
    while (totalCycles < numCycles)
    {
        step();
        totalCycles += cycles;
    }
}

// Interrupt handlers
void CPU6502::nmi()
{
    pushWord(PC);
    PHP(0);
    SR.I = 1;
    PC = readWord(0xFFFA);
}

void CPU6502::irq()
{
    if (!SR.I)
    {
        pushWord(PC);
        PHP(0);
        SR.I = 1;
        PC = readWord(0xFFFE);
    }
}

void CPU6502::viewROM(ADDRESS_T laddr, ADDRESS_T haddr)
{
    const char* color = "";

    int offset = laddr - 16;
    int rows = ((haddr - laddr + 0x10) / (16));

    ADDRESS_T c = laddr;

    for (int i = -1; i <= rows; i++)
    {
        for (int j = 0; j <= 16; j++)
        {
            if (i == -1 || i == rows)
            {
                printf("%s", KWHT);

                if (j == 0)
                {
                    printf("     ");
                }

                if (j < 16)
                {
                    printf("%02X ", j);
                }
            }
            else if (i >= 0 && (j == 0 || j == 16))
            {
                printf("%s", KWHT);
                printf("%04X ", offset);
            }

            if (memory[c] == 0)
            {
                color = KGRY;
            }
            else if (memory[c] > 0 && memory[c] <= 63)
            {
                color = KBLU;
            }
            else if (memory[c] >= 64 && memory[c] <= 127)
            {
                color = KGRN;
            }
            else if (memory[c] >= 128 && memory[c] <= 191)
            {
                color = KYEL;
            }
            else if (memory[c] >= 192 && memory[c] <= 255)
            {
                color = KRED;
            }

            if (i >= 0 && i < rows && j < 16)
            {
                if (PC == c)
                {
                    printf("\x1B[7m");
                }

                printf("%s%02X\x1B[0m ", color, memory[c]);
                c++;
            }

        }

        offset += 16;
        printf("\n%s", KWHT);
    }

    printf("\r\n");
}

void CPU6502::viewRegisters()
{
    printf("PC   IRQ  SR AC XR YR SP\r\n");
    printf("%04X %04X %02X %02X %02X %02X %02X\r\n", PC, 0, SR, AC, XR, YR, SP);
    //printf("A: %02X   X: %02X   Y: %02X   SP: %02X   PC: %04X\r\n", AC, XR, YR, SP, PC);
}

void CPU6502::viewFlags()
{
    printf("NV-BDIZC\n");
    printf("%u%u %u%u%u%u%u\n", SR.N, SR.V, SR.B, SR.D, SR.I, SR.Z, SR.C);
}

// Debug functions
BYTE_T    CPU6502::getA()                  { return AC; }
BYTE_T    CPU6502::getX()                  { return XR; }
BYTE_T    CPU6502::getY()                  { return YR; }
BYTE_T    CPU6502::getSP()                 { return SP; }
ADDRESS_T CPU6502::getPC()                 { return PC; }
BYTE_T    CPU6502::readMem(ADDRESS_T addr) { return memory[addr]; }



/******************************************************************************/
/******************************************************************************/
/* Private Functions                                                          */
/******************************************************************************/
/******************************************************************************/



/************************END OF FILE************************/
