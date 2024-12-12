/**
  ******************************************************************************
  * @file    CPU.hpp
  * @author  Josh Haden
  * @version V0.0.1
  * @date    05 NOV 2024
  * @brief   Header for CPU.cpp
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CPU_HPP
#define __CPU_HPP

/* Includes ------------------------------------------------------------------*/
#include <array>
#include <functional>
#include <unordered_map>

#include "Types.hpp"
#include "Memory.hpp"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define CPU_AUTO_STEP   false
#define CPU_USER_STEP   true

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* Class definition --------------------------------------------------------- */
class CPU6502
{
private:
    WORD_T PC = 0;     // Program Counter
    BYTE_T AC = 0;     // Accumulator
    BYTE_T XR = 0;     // X register
    BYTE_T YR = 0;     // Y register
    BYTE_T SP = 0xFF;  // Stack Pointer

    // Status register
    struct
    {
        BYTE_T C : 1;  // Carry
        BYTE_T Z : 1;  // Zero
        BYTE_T I : 1;  // Interrupt disable
        BYTE_T D : 1;  // Decimal mode
        BYTE_T B : 1;  // Break command
        BYTE_T U : 1;  // Unused (always 1)
        BYTE_T V : 1;  // Overflow
        BYTE_T N : 1;  // Negative
    } SR;

    // Structure to hold instruction information
    struct INSTRUCTION_T
    {
        OPCODE_T opcode;
        void (CPU6502::*inst_fn)(ADDRESS_T*);
        ADDRESS_T(CPU6502::*addr_fn)();
        BYTE_T bytes;
        BYTE_T cycles;
    };
    
    Memory memory;      // Memory (64KB)
    uint8_t cycles = 0;

    // Addressing mode function pointers
    //using AddrMode = std::function<ADDRESS_T(CPU6502&)>;
    //std::unordered_map<uint8_t, AddrMode> addressing_modes;

    // Instruction function pointers
    //using Instruction = std::function<void(CPU6502&, ADDRESS_T)>;
    //std::unordered_map<uint8_t, Instruction> instructions;

    // Helper functions
    void setZN(BYTE_T value);

    BYTE_T readByte(ADDRESS_T addr);
    ADDRESS_T readWord(ADDRESS_T addr);

    void writeByte(ADDRESS_T addr, BYTE_T value);

    void push(BYTE_T value);
    void pushWord(WORD_T value);
    BYTE_T pull();
    ADDRESS_T pullWord();

    void initPC(ADDRESS_T startAddr);

    // Addressing modes
    ADDRESS_T implied();
    ADDRESS_T immediate();
    ADDRESS_T absolute();
    ADDRESS_T zeroPage();
    ADDRESS_T zeroPageX();
    ADDRESS_T zeroPageY();
    ADDRESS_T absoluteX();
    ADDRESS_T absoluteY();
    ADDRESS_T indirect();
    ADDRESS_T indexedIndirect();
    ADDRESS_T indirectIndexed();
    ADDRESS_T relative();

    // Instructions
    // Load/Store Operations
    void LDA(ADDRESS_T *addr);
    void LDX(ADDRESS_T *addr);
    void LDY(ADDRESS_T *addr);
    void STA(ADDRESS_T *addr);
    void STX(ADDRESS_T *addr);
    void STY(ADDRESS_T *addr);

    // Register Transfers
    void TAX(ADDRESS_T*);
    void TAY(ADDRESS_T*);
    void TXA(ADDRESS_T*);
    void TYA(ADDRESS_T*);

    // Stack Operations
    void TSX(ADDRESS_T*);
    void TXS(ADDRESS_T*);
    void PHA(ADDRESS_T*);
    void PHP(ADDRESS_T*);
    void PLA(ADDRESS_T*);
    void PLP(ADDRESS_T*);

    // Logical Operations
    void AND(ADDRESS_T *addr);
    void EOR(ADDRESS_T *addr);
    void ORA(ADDRESS_T *addr);
    void BIT(ADDRESS_T *addr);

    // Arithmetic Operations
    void ADC(ADDRESS_T *addr);
    void SBC(ADDRESS_T *addr);

    // Increments & Decrements
    void INC(ADDRESS_T *addr);
    void INX(ADDRESS_T*);
    void INY(ADDRESS_T*);
    void DEC(ADDRESS_T *addr);
    void DEX(ADDRESS_T*);
    void DEY(ADDRESS_T*);

    // Shifts
    void ASL(ADDRESS_T *addr);
    void ASL_A(ADDRESS_T*);
    void LSR(ADDRESS_T *addr);
    void LSR_A(ADDRESS_T*);

    void ROL(ADDRESS_T *addr);
    void ROL_A(ADDRESS_T*);
    void ROR(ADDRESS_T *addr);
    void ROR_A(ADDRESS_T*);

    // Jumps & Calls
    void JMP(ADDRESS_T *addr);
    void JSR(ADDRESS_T *addr);
    void RTS(ADDRESS_T*);
    void RTI(ADDRESS_T*);

    // Branches
    void branch(ADDRESS_T *addr, bool condition);

    void BCC(ADDRESS_T *addr);
    void BCS(ADDRESS_T *addr);
    void BEQ(ADDRESS_T *addr);
    void BNE(ADDRESS_T *addr);
    void BVC(ADDRESS_T *addr);
    void BVS(ADDRESS_T *addr);
    void BPL(ADDRESS_T *addr);
    void BMI(ADDRESS_T *addr);

    // Status Flag Changes
    void CLC(ADDRESS_T*);
    void CLD(ADDRESS_T*);
    void CLI(ADDRESS_T*);
    void CLV(ADDRESS_T*);
    void SEC(ADDRESS_T*);
    void SED(ADDRESS_T*);
    void SEI(ADDRESS_T*);

    // Compare Operations
    void compare(BYTE_T reg, BYTE_T value);

    void CMP(ADDRESS_T *addr);
    void CPX(ADDRESS_T *addr);
    void CPY(ADDRESS_T *addr);

    // System Functions
    void BRK(ADDRESS_T*);

    void NOP(ADDRESS_T*);

    static const INSTRUCTION_T intruction_table[];

public:
    CPU6502();

    void reset();

    // Status register helpers
    uint8_t getStatus() const;

    void setStatus(BYTE_T status);

    void flashProgram(const BYTE_T* program, size_t size, ADDRESS_T addr);

    void step();
    void execute();
    void run(int numCycles);

    // Interrupt handlers
    void nmi();
    void irq();

    void viewROM(ADDRESS_T laddr, ADDRESS_T haddr);
    void viewRegisters();
    void viewFlags();

    // Debug functions
    BYTE_T getA();
    BYTE_T getX();
    BYTE_T getY();
    BYTE_T getSP();
    ADDRESS_T getPC();
    BYTE_T readMem(ADDRESS_T addr);
};



#endif /* __CPU_HPP */

/************************END OF FILE************************/
