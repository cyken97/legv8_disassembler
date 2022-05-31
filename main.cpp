#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <bitset>

#include "disasm.h"

typedef unsigned char byte;
typedef unsigned int dword;

using namespace std;

vector<string> readFile(string fileName)
{
    ifstream inFile(fileName, ios::binary);
    vector<byte> buffer(istreambuf_iterator<char>(inFile), {});
    
    // if (buffer.size() % 4 != 0)
        //Common::printe("input binary file is not 32bit aligned");

    vector<string> arr;
    auto it = buffer.begin();
    while (it != buffer.end())
    {
        string inst = "";
        for (int i = 0; i < 4; i++)
        {
            inst += bitset<8>(*it).to_string();
            it++;
        }
        arr.push_back(inst);
    }
    return arr;
}

// void writeFile(std::vector<std::string> instructions, std::string fileName)
// {

// }

vector<string> branchCond;

Inst decode(string inst_bits, int index)
{
    Inst di("", -1);

    do 
    {
        if (inst_bits.substr(0, 11).compare("10001011000") == 0) // ADD
        {
            di = parseRtpInst("ADD", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 10).compare("1001000100") == 0) // ADDI
        {
            di = parseItpInst("ADDI", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("10001010000") == 0) // AND
        {
            di = parseRtpInst("AND", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 10).compare("1001001000") == 0) // ANDI
        {
            di = parseItpInst("ANDI", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 6).compare("000101") == 0) // B
        {
            di = parseBtpInst("B", inst_bits, index);
            break;
        }
        else if (inst_bits.substr(0, 8).compare("01010100") == 0) // B.cond
        {
            // parse condition flag in Rt
            int32_t cond = parseImmediate(inst_bits.substr(27, 5));
            int32_t offset = parseOffset(inst_bits.substr(8, 19));
            int32_t brAddr = index + offset;
            string decodedInst = "B." + branchCond[cond] + " label" + to_string(brAddr);

            di = Inst(decodedInst, brAddr);
            break;
        }
        else if (inst_bits.substr(0, 6).compare("100101") == 0) // BL
        {
            // BL <label>, to call function
            di = parseBtpInst("BL", inst_bits, index);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11010110000") == 0) // BR
        {
            // BR <reg>, branch to reg
            string brReg = parseRegister(inst_bits.substr(22, 5));
            string decodedInst = "BR " + brReg;
            di = Inst(decodedInst, -1);
            break;
        }
        else if (inst_bits.substr(0, 8).compare("10110101") == 0) // CBNZ
        {
            // CBNZ <reg>, <label>
            di = parseCBtpInst("CBNZ", inst_bits, index);
            break;
        }
        else if (inst_bits.substr(0, 8).compare("10110100") == 0) // CBZ
        {
            // CBNZ <reg>, <label>
            di = parseCBtpInst("CBZ", inst_bits, index);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11001010000") == 0) // EOR
        {
            di = parseRtpInst("EOR", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 10).compare("1101001000") == 0) // EORI
        {
            di = parseItpInst("EORI", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11111000010") == 0) // LDUR
        {
            di = parseDtpInst("LDUR", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11010011011") == 0) // LSL
        {
            // R-format, Rm unused
            string Rd = parseRegister(inst_bits.substr(27, 5));
            string Rn = parseRegister(inst_bits.substr(22, 5));
            int32_t shamt = parseImmediate(inst_bits.substr(16, 6));
            string decodedString = "LSL " + Rd + ", " + Rn + ", #" + to_string(shamt); 
            di = Inst(decodedString, -1);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11010011010") == 0) // LSR
        {
            // R-format, Rm unused
            string Rd = parseRegister(inst_bits.substr(27, 5));
            string Rn = parseRegister(inst_bits.substr(22, 5));
            int32_t shamt = parseImmediate(inst_bits.substr(16, 6));
            string decodedString = "LSR " + Rd + ", " + Rn + ", #" + to_string(shamt); 
            di = Inst(decodedString, -1);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("10101010000") == 0) // ORR
        {
            di = parseRtpInst("ORR", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 10).compare("1011001000") == 0) // ORRI
        {
            di = parseItpInst("ORRI", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11111000000") == 0) // STUR
        {
            di = parseDtpInst("STUR", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11001011000") == 0) // SUB
        {
            di = parseRtpInst("SUB", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 10).compare("1101000100") == 0) // SUBI
        {
            di = parseItpInst("SUBI", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 10).compare("1111000100") == 0) // SUBIS
        {
            di = parseItpInst("SUBIS", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11101011000") == 0) // SUBS
        {
            di = parseRtpInst("SUBS", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("10011011000") == 0) // MUL
        {
            di = parseRtpInst("MUL", inst_bits);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11111111101") == 0) // PRNT
        {
            string Rd = parseRegister(inst_bits.substr(27, 5));
            string decodedInst = "PRNT " + Rd;
            di = Inst(decodedInst, -1);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11111111100") == 0) // PRNL
        {
            di = Inst("PRNL", -1);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11111111110") == 0) // DUMP
        {
            di = Inst("DUMP", -1);
            break;
        }
        else if (inst_bits.substr(0, 11).compare("11111111111") == 0) // HALT
        {
            di = Inst("HALT", -1);
            break;
        }
        else
        {
            // error logging
            cout << "No inst found!" << endl;
            cout << inst_bits << endl;
        }
    } while (false);

    return di;
}


int main(int argc, char *argv[])
{
    // Initialize branchCond
    branchCond.resize(14);
    branchCond[0] = "EQ";
    branchCond[1] = "NE";
    branchCond[2] = "HS";
    branchCond[3] = "LO";
    branchCond[4] = "MI";
    branchCond[5] = "PL";
    branchCond[6] = "VS";
    branchCond[7] = "VC";
    branchCond[8] = "HI";
    branchCond[9] = "LS";
    branchCond[10] = "GE";
    branchCond[11] = "LT";
    branchCond[12] = "GT";
    branchCond[13] = "LE";

    vector<string> data = readFile(argv[1]);

    vector<Inst> decodeList;
    for (int i = 0; i < data.size(); i++)
    {
        decodeList.push_back(decode(data[i], i));
    }

    // generate jump labels
    vector<bool> labelList;
    labelList.resize(decodeList.size() + 1);

    for (int i = 0; i < decodeList.size(); i++)
    {
        int refIndex = decodeList[i].getDependency();
        if (refIndex != -1)
        {
            labelList[refIndex] = true;
        }
    }

    // print to std
    for (int i = 0; i < decodeList.size(); i++)
    {
        if (labelList[i] == true)
            cout << "label" << to_string(i) << ":" << endl;
        
        cout << decodeList[i].getCode() << endl;
    }

    // print to file
    ofstream outFile("disasm.legv8asm");
    for (int i = 0; i < decodeList.size(); i++)
    {
        if (labelList[i])
            outFile << "label" << to_string(i) << ":" << endl;
        
        outFile << decodeList[i].getCode() << endl;
    }

    if (labelList[labelList.size() - 1])
        outFile << "label" << to_string(decodeList.size()) << ":" << endl;
    outFile.close();

    return 0;
}