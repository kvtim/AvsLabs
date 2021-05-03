
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
    public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        if (instr->_src1 && (instr->_imm || instr->_src2))
        {
            if(instr->_imm)
                SetData(instr, ip, ALUOperations(instr->_src1Val, instr->_imm.value(), instr->_aluFunc), true);
            else
                SetData(instr, ip, ALUOperations(instr->_src1Val, instr->_src2Val, instr->_aluFunc), true);
        }
        else
            SetData(instr, ip, 0, false);
        
        SetIp(instr, ip);
    }

private:
    void SetData(InstructionPtr& instr, Word ip, Word resultAfterAlUOperations, bool checkRegisters)
    {
        if(checkRegisters)
        {
            switch(instr->_type)
            {
                case IType::Ld:
                    instr->_addr = resultAfterAlUOperations;
                    break;
                case IType::St:
                    instr->_addr = resultAfterAlUOperations;
                    instr->_data = instr->_src2Val;
                    break;
                case IType::Alu:
                case IType::Unsupported:
                case IType::Br:
                    instr->_data = resultAfterAlUOperations;
                    break;
                default:
                    break;
            }
        }

        switch(instr->_type)
        {
            case IType::St:
                instr->_data = instr->_src2Val;
                break;
            case IType::Csrr:
                instr->_data = instr->_csrVal;
                break;
            case IType::Csrw:
                instr->_data = instr->_src1Val;
                break;
            case IType::J:
                instr->_data = ip + 4;
                break;
            case IType::Jr:
                instr->_data = ip + 4;
                break;
            case IType::Auipc:
                instr->_data = ip + instr->_imm.value();
                break;
            default:
                break;
        }

    }

    void SetIp(InstructionPtr& instr, Word ip)
    {
        if (((instr->_src1 && instr->_src2) || instr->_brFunc == BrFunc::AT || instr->_brFunc == BrFunc::NT) 
        && TransitionCondition(instr->_src1Val, instr->_src2Val, instr->_brFunc))
        {
            switch(instr->_type)
            {
                case IType::Alu:
                    instr->_nextIp = ip + 4;
                    break;
                case IType::Br:
                    instr->_nextIp = ip + instr->_imm.value();
                    break;
                case IType::J:
                    instr->_nextIp = ip + instr->_imm.value();
                    break;
                case IType::Jr:
                    instr->_nextIp = instr->_imm.value() + instr->_src1Val;
                    break;
                case IType::Auipc:
                    instr->_nextIp = ip + instr->_imm.value();
                    break;
                case IType::Csrr:
                    instr->_nextIp = instr->_csrVal;
                    break;
                case IType::Csrw:
                    instr->_nextIp = instr->_src1Val;
                    break;
                case IType::St:
                    instr->_nextIp = instr->_src2Val;
                    break;
                default:
                    break;
            }
        }
        else
        {
            instr->_nextIp = ip + 4;
        }
    }

    Word ALUOperations(Word A, Word B, AluFunc aluFunc)
    {
        Word temp;

        switch(aluFunc)
        {
            case AluFunc::Add:
                return (A + B);
            case AluFunc::Sub:
                return (A - B);
            case AluFunc::And:
                return (A & B);
            case AluFunc::Or:
                return (A | B);
            case AluFunc::Xor:
                return (A ^ B);
            case AluFunc::Slt:
                return (static_cast<SignedWord>(A) < static_cast<SignedWord>(B));
            case AluFunc::Sltu:
                return (A < B);
            case AluFunc::Sll:
                temp = B % 32;
                return (A << temp);
            case AluFunc::Srl:
                temp = B % 32;
                return (A >> temp);
            case AluFunc::Sra:
                temp = B % 32;
                return (static_cast<SignedWord>(A) >> temp);
            default:
                return 0;
        }
    }

    bool TransitionCondition(Word A, Word B, BrFunc brFunc)
    {
        switch(brFunc)
        {
            case BrFunc::Eq:
                return (A == B);
            case BrFunc::Neq:
                return (A != B);
            case BrFunc::Lt:
                return (static_cast<SignedWord>(A) < static_cast<SignedWord>(B));
            case BrFunc::Ltu:
                return (A < B);
            case BrFunc::Ge:
                return (static_cast<SignedWord>(A) >= static_cast<SignedWord>(B));
            case BrFunc::Geu:
                return (A >= B);
            case BrFunc::AT:
                return true;
            case BrFunc::NT:
                return false;
            default:
                return false;
        }
    }
};

#endif // RISCV_SIM_EXECUTOR_H
