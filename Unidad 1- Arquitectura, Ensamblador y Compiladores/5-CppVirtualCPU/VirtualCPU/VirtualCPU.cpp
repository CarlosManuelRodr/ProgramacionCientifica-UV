#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include "Matrix.h"
using namespace std;

enum class MachineInstructions
{
    LoadA,
    LoadB,
    SetA,
    SetB,
    Store, 
    Add, 
    Substract, 
    Increment, 
    Decrement, 
    Jump, 
    JumpPos, 
    JumpNeg, 
    JumpZero, 
    JumpNotZero, 
    JumpOdd, 
    Call, 
    Return, 
    Print, 
    Draw
};

string instruction_as_string(uint16_t instruction)
{
    MachineInstructions currentInstruction = static_cast<MachineInstructions>(instruction);

    switch (currentInstruction)
    {
        case MachineInstructions::LoadA:
            return "LoadA";
        break;
        case MachineInstructions::LoadB:
            return "LoadB";
        break;
        case MachineInstructions::SetA:
            return "SetA";
        break;
        case MachineInstructions::SetB:
            return "SetB";
        break;
        case MachineInstructions::Store:
            return "Store";
        break;
        case MachineInstructions::Add:
            return "Add";
        break;
        case MachineInstructions::Substract:
            return "Substract";
        break;
        case MachineInstructions::Increment:
            return "Increment";
        break;
        case MachineInstructions::Decrement:
            return "Decrement";
        break;
        case MachineInstructions::Jump:
            return "Jump";
        break;
        case MachineInstructions::JumpPos:
            return "JumpPos";
        break;
        case MachineInstructions::JumpNeg:
            return "JumpNeg";
        break;
        case MachineInstructions::JumpZero:
            return "JumpZero";
        break;
        case MachineInstructions::JumpNotZero:
            return "JumpNotZero";
        break;
        case MachineInstructions::JumpOdd:
            return "JumpOdd";
        break;
        case MachineInstructions::Call:
            return "Call";
        break;
        case MachineInstructions::Return:
            return "Return";
        break;
        case MachineInstructions::Print:
            return "Print";
        break;
        case MachineInstructions::Draw:
            return "Draw";
        break;
        default:
            return "Invalid instruction";
        break;
    }
}

uint16_t read_uint16(istream& file)
{
    uint16_t val;
    uint8_t bytes[2];

    file.read((char*)bytes, 2);        // read 2 bytes from the file
    val = bytes[0] | (bytes[1] << 8);  // construct the 16-bit value from those bytes

    return val;
}

vector<uint16_t> load_executable(string path)
{
    ifstream machineCodeFile(path, ios::in | ios::binary);

    if (machineCodeFile.is_open())
    {
        vector<uint16_t> machineCode;
        while (!machineCodeFile.eof())
        {
            machineCode.push_back(read_uint16(machineCodeFile));
        }
        machineCode.pop_back();

        return machineCode;
    }
    else
    {
        cout << "Error while loading program" << endl;
        return vector<uint16_t>();
    }
}

class VirtualCPU
{
private:
    uint16_t registerA, registerB;
    uint16_t addressRegister, instructionRegister, parameterRegister, stackPointerRegister;

    bool aluPositiveFlag, aluNegativeFlag, aluZeroFlag, aluNotZeroFlag, aluParityFlag;
    bool halted;

    uint16_t ram[1024];
    uint16_t stack[8];

    vector<uint16_t> textBuffer;
    Matrix<bool> imageBuffer = Matrix<bool>(17, 17);

public:
    VirtualCPU()
    {
        registerA = 0;
        registerB = 0;
        addressRegister = 0;
        instructionRegister = 0;
        parameterRegister = 0;
        stackPointerRegister = 0;

        aluPositiveFlag = false;
        aluNegativeFlag = false;
        aluZeroFlag = false;
        aluNotZeroFlag = false;
        aluParityFlag = false;

        halted = false;

        for (unsigned i = 0; i < 1024; i++)
            ram[i] = 0;

        for (unsigned i = 0; i < 8; i++)
            stack[i] = 0;

        for (unsigned i = 0; i < 17; i++)
            for (unsigned j = 0; j < 17; j++)
                imageBuffer[i][j] = false;
    }

    void DrawRegistersStatus()
    {
        cout << "####### ALU Registers #######" << endl;
        cout << "AluPositiveFlag: " << aluPositiveFlag << endl;
        cout << "AluNegativeFlag: " << aluNegativeFlag << endl;
        cout << "AluZeroFlag: " << aluZeroFlag << endl;
        cout << "AluNotZeroFlag: " << aluNotZeroFlag << endl;
        cout << "AluParityFlag: " << aluParityFlag << endl;

        cout << endl;

        cout << "####### Main Registers #######" << endl;
        cout << "RegisterA: " << registerA << endl;
        cout << "RegisterB: " << registerB << endl;
        cout << "AddressRegister: " << addressRegister << endl;
        cout << "InstructionRegister: " << instruction_as_string(instructionRegister) << endl;
        cout << "ParameterRegister: " << parameterRegister << endl;
        cout << "StackPointerRegister: " << stackPointerRegister << endl;

        cout << endl;
    }

    bool IsHalted()
    {
        return halted;
    }

    void DrawBuffersOutput()
    {
        cout << "####### Buffers output #######" << endl;
        cout << "Text buffer: ";
        for (auto c : textBuffer)
            cout << c << " ";
        cout << endl;

        cout << "Image buffer: " << endl;
        cout << "-------------------" << endl;
        for (unsigned i = 0; i < 17; i++)
        {
            cout << "|";
            for (unsigned j = 0; j < 17; j++)
            {
                if (imageBuffer[i][j])
                    cout << "X";
                else
                    cout << " ";
            }
            cout << "|" << endl;
        }
        cout << "-------------------" << endl;
    }

    void LoadExecutableFromFile(string path)
    {
        vector<uint16_t> machineCode = load_executable(path);
        for (unsigned i = 0; i < machineCode.size(); i++)
        {
            ram[i] = machineCode[i];
        }
    }

    void UpdateALU()
    {
        uint16_t n = registerA;
        if (n > 0)
        {
            aluPositiveFlag = true;
            aluNegativeFlag = false;
        }

        if (n == 0)
            aluZeroFlag = true;
        else
            aluZeroFlag = false;

        if (n != 0)
            aluNotZeroFlag = true;
        else
            aluNotZeroFlag = false;

        if (n % 2 == 0)
            aluParityFlag = true;
        else
            aluParityFlag = false;
    }

    void UpdateALU(int16_t n)
    {
        if (n > 0)
        {
            aluPositiveFlag = true;
            aluNegativeFlag = false;
        }

        if (n < 0)
        {
            aluPositiveFlag = false;
            aluNegativeFlag = true;
        }

        if (n == 0)
            aluZeroFlag = true;
        else
            aluZeroFlag = false;

        if (n != 0)
            aluNotZeroFlag = true;
        else
            aluNotZeroFlag = false;

        if (n % 2 == 0)
            aluParityFlag = true;
        else
            aluParityFlag = false;
    }

    void PerformCycle()
    {
        instructionRegister = ram[addressRegister];
        parameterRegister = ram[addressRegister + 1];

        MachineInstructions currentInstruction = static_cast<MachineInstructions>(instructionRegister);

        switch (currentInstruction)
        {
            case MachineInstructions::LoadA:
            {
                registerA = ram[parameterRegister];
                this->UpdateALU();
                addressRegister += 2;
            }
            break;
            case MachineInstructions::LoadB:
            {
                registerB = ram[parameterRegister];
                addressRegister += 2;
            }
            break;
            case MachineInstructions::SetA:
            {
                registerA = parameterRegister;
                addressRegister += 2;
            }
            break;
            case MachineInstructions::SetB:
            {
                registerB = parameterRegister;
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Store:
            {
                ram[parameterRegister] = registerA;
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Add:
            {
                int16_t sum = registerA + registerB;
                registerA = sum;
                this->UpdateALU(sum);
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Substract:
            {
                int16_t sub = registerA - registerB;
                registerA = sub;
                this->UpdateALU(sub);
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Increment:
            {
                int16_t sum = registerA + 1;
                registerA = sum;
                this->UpdateALU(sum);
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Decrement:
            {
                int16_t sub = registerA - 1;
                registerA = sub;
                this->UpdateALU(sub);
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Jump:
            {
                addressRegister = parameterRegister;
            }
            break;
            case MachineInstructions::JumpPos:
            {
                if (aluPositiveFlag)
                    addressRegister = parameterRegister;
                else
                    addressRegister += 2;
            }
            break;
            case MachineInstructions::JumpNeg:
            {
                if (aluNegativeFlag)
                    addressRegister = parameterRegister;
                else
                    addressRegister += 2;
            }
            break;
            case MachineInstructions::JumpZero:
            {
                if (aluZeroFlag)
                    addressRegister = parameterRegister;
                else
                    addressRegister += 2;
            }
            break;
            case MachineInstructions::JumpNotZero:
            {
                if (aluNotZeroFlag)
                    addressRegister = parameterRegister;
                else
                    addressRegister += 2;
            }
            break;
            case MachineInstructions::JumpOdd:
            {
                if (aluParityFlag)
                    addressRegister = parameterRegister;
                else
                    addressRegister += 2;
            }
            break;
            case MachineInstructions::Call:
            {
                stack[stackPointerRegister] = addressRegister;
                stackPointerRegister++;
                addressRegister = parameterRegister;
            }
            break;
            case MachineInstructions::Return:
            {
                if (stackPointerRegister == 0)
                    halted = true;
                else
                {
                    stackPointerRegister--;
                    addressRegister = stack[stackPointerRegister];
                    stack[stackPointerRegister] = 0;
                    addressRegister += 2;
                }
            }
            break;
            case MachineInstructions::Print:
            {
                textBuffer.push_back(ram[parameterRegister]);
                addressRegister += 2;
            }
            break;
            case MachineInstructions::Draw:
            {
                uint16_t row = registerA;
                uint16_t col = registerB;
                bool color = static_cast<bool>(ram[parameterRegister]);

                imageBuffer[row][col] = color;
                addressRegister += 2;
            }
            break;
            default:
                break;
        }
    }


};

void pause()
{
    std::cout << "Press ENTER to continue...";
    std::cin.clear();
    std::cin.sync();
    std::cin.get();
}

void clear_screen()
{
#ifdef _WIN32
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

int main()
{
    VirtualCPU cpu;
    cpu.LoadExecutableFromFile("rule90.vme");

    for (int i = 0; i < 20000; i++)
    {
        cpu.PerformCycle();
        if (i % 1000 == 0)
        {
            cpu.DrawRegistersStatus();
            cpu.DrawBuffersOutput();
            pause();
            clear_screen();
        }
    }

    cpu.DrawRegistersStatus();
    cpu.DrawBuffersOutput();
}