#ifndef DISASSEMBLERALGORITHM_H
#define DISASSEMBLERALGORITHM_H

#include <stack>
#include <set>
#include "../../redasm.h"
#include "../../redasm/analyzer/analyzer.h"
#include "statemachine.h"

namespace REDasm {

class DisassemblerAlgorithm: public StateMachine
{
    public:
        enum: u32 { OK, SKIP, FAIL };
        enum: state_t { DecodeState, JumpState, CallState, AddressTableState, MemoryState, ImmediateState };

    private:
        typedef std::set<address_t> DecodedAddresses;

    protected:
        DisassemblerAlgorithm(DisassemblerAPI* disassembler, AssemblerPlugin* assembler);

    public:
        u32 disassembleInstruction(address_t address, const InstructionPtr& instruction);
        void enqueue(address_t address);
        bool analyze();

    protected:
        virtual void onDecoded(const InstructionPtr& instruction);
        virtual void onDecodeFailed(const InstructionPtr& instruction);

    protected:
        virtual void decodeState(const State *state);
        virtual void jumpState(const State* state);
        virtual void callState(const State* state);
        virtual void addressTableState(const State* state);
        virtual void memoryState(const State* state);
        virtual void immediateState(const State* state);

    private:
        bool canBeDisassembled(address_t address);
        void createInvalidInstruction(const InstructionPtr& instruction);
        u32 disassemble(address_t address, const InstructionPtr& instruction);

    protected:
        DisassemblerAPI* m_disassembler;
        AssemblerPlugin* m_assembler;
        ListingDocument* m_document;
        FormatPlugin* m_format;

    private:
        DecodedAddresses m_disassembled;
        std::unique_ptr<Analyzer> m_analyzer;
        const Segment* m_currentsegment;
        bool m_analyzed;
};

} // namespace REDasm

#endif // DISASSEMBLERALGORITHM_H
