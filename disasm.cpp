#include "disasm.h"

#include <bitset>
#include <iostream>
#include <stdlib.h>
using namespace std;

/*
    Parse string of bits into 
*/
string parseRegister(string bits)
{
    int32_t regNumber = parseImmediate(bits);

    string regBuffer = "";
    if (regNumber >= 0 && regNumber <= 27)
        regBuffer = "X" + to_string(regNumber);
    else if (regNumber == 28)
        regBuffer = "SP";
    else if (regNumber == 29)
        regBuffer = "FP";
    else if (regNumber == 30)
        regBuffer = "LR";
    else if (regNumber == 31)
        regBuffer = "XZR";
    else
    {
        // Common::printe("parseReg given invalid range register");
    }
    return regBuffer;
}

int32_t parseImmediate(string bits)
{
    bitset<32> numBitset(bits);
    int32_t num = (int32_t) numBitset.to_ulong();
    return num;
}

int32_t parseOffset(string bits)
{
    int32_t num;

    if (bits[0] == '1') // handle negative offset
    {
        // handle negative integers by pumping bits
        int pumpLength = 32 - bits.size();
        bits.insert(0, pumpLength, '1');

        bitset<32> bs(bits);
        bs.flip();
        num = (int32_t) bs.to_ulong() + 1;
        num = -num;
    }
    else
    {
        num = parseImmediate(bits);
    }
    
    return num;
}

Inst parseRtpInst(string inst, string bits)
{
    // parse operands
    string Rm = parseRegister(bits.substr(11, 5));
    string Rn = parseRegister(bits.substr(22, 5));
    string Rd = parseRegister(bits.substr(27, 5));
    string decodedInst = inst + " " + Rd + ", " + Rn + ", " + Rm;
    
    return Inst(decodedInst, -1);
}

Inst parseItpInst(string inst, string bits)
{
    int32_t imm = parseImmediate(bits.substr(10, 12));
    string Rn = parseRegister(bits.substr(22, 5));
    string Rd = parseRegister(bits.substr(27, 5));
    string decodedInst = inst + " " + Rd + ", " + Rn + ", #" + to_string(imm);

    return Inst(decodedInst, -1);
}

Inst parseDtpInst(string inst, string bits)
{
    int32_t offset = parseOffset(bits.substr(11, 9));
    string Rn = parseRegister(bits.substr(22, 5));
    string Rt = parseRegister(bits.substr(27, 5));
    string decodedInst = inst + " " + Rt + ", [" + Rn + ", #" + to_string(offset) + "]";

    return Inst(decodedInst, -1);
}

Inst parseBtpInst(string inst, string bits, int32_t base)
{
    int32_t brOffset = parseOffset(bits.substr(6, 26));
    int32_t brAddr = base + brOffset;
    string decodedInst = inst + " label" + to_string(brAddr);

    if (brAddr == 131)
        cout << inst << endl;

    return Inst(decodedInst, brAddr);
}

Inst parseCBtpInst(string inst, string bits, int32_t base)
{
    int32_t brOffset = parseOffset(bits.substr(8, 19));
    int32_t brAddr = base + brOffset;
    string Rt = parseRegister(bits.substr(27, 5));
    string decodedInst = inst + " " + Rt + ", " + "label" + to_string(brAddr);

    return Inst(decodedInst, brAddr);
}

// Inst parseIWtpInst(string inst, string bits);

Inst::Inst(string code, int dep)
{
    this->code = code;
    this->depend = dep;
}

string Inst::getCode()
{
    return code;
}

int Inst::getDependency()
{
    return depend;
}