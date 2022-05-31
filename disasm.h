#ifndef DISASM_H_
#define DISASM_H_

#include <string>
#include <vector>

class Inst
{
    public:
        Inst(std::string code, int dep);
        std::string getCode();
        int getDependency();
    private:
        std::string code;
        int depend;
};

std::string parseRegister(std::string bits);
int32_t parseImmediate(std::string bits);
int32_t parseOffset(std::string bits);

Inst parseRtpInst(std::string inst, std::string bits);
Inst parseItpInst(std::string inst, std::string bits);
Inst parseDtpInst(std::string inst, std::string bits);
Inst parseBtpInst(std::string inst, std::string bits, int32_t base);
Inst parseCBtpInst(std::string inst, std::string bits, int32_t base);
Inst parseIWtpInst(std::string inst, std::string bits);
#endif