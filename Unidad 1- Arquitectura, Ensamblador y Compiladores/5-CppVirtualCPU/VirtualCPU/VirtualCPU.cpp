#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>
#include "Matrix.h"
#include "OptionParser.h"
using namespace std;

/****************************
*                           *
*      Virtual Machine      *
*                           *
****************************/

// List of available instructions
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
    // Load the executable into a vector of uint16_t by reading in 16-bit chunks
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

    ///@brief Perform one CPU cycle by processing the current instruction.
    void PerformCycle()
    {
        // Get instruction and parameter from RAM
        instructionRegister = ram[addressRegister];
        parameterRegister = ram[addressRegister + 1];

        // Transform the content of instructionRegister to a MachineInstruction
        MachineInstructions currentInstruction = static_cast<MachineInstructions>(instructionRegister);

        // Execute instruction
        // Homework: Fill the behavior of each instruction
        switch (currentInstruction)
        {
            case MachineInstructions::LoadA:
            {

            }
            break;
            case MachineInstructions::LoadB:
            {

            }
            break;
            case MachineInstructions::SetA:
            {

            }
            break;
            case MachineInstructions::SetB:
            {

            }
            break;
            case MachineInstructions::Store:
            {

            }
            break;
            case MachineInstructions::Add:
            {

            }
            break;
            case MachineInstructions::Substract:
            {

            }
            break;
            case MachineInstructions::Increment:
            {

            }
            break;
            case MachineInstructions::Decrement:
            {

            }
            break;
            case MachineInstructions::Jump:
            {

            }
            break;
            case MachineInstructions::JumpPos:
            {

            }
            break;
            case MachineInstructions::JumpNeg:
            {

            }
            break;
            case MachineInstructions::JumpZero:
            {

            }
            break;
            case MachineInstructions::JumpNotZero:
            {

            }
            break;
            case MachineInstructions::JumpOdd:
            {

            }
            break;
            case MachineInstructions::Call:
            {

            }
            break;
            case MachineInstructions::Return:
            {

            }
            break;
            case MachineInstructions::Print:
            {

            }
            break;
            case MachineInstructions::Draw:
            {

            }
            break;
            default:
                break;
        }
    }


};

/****************************
*                           *
*       Option Parser       *
*                           *
****************************/

struct Arg : public option::Arg
{
    static void PrintError(const char* msg1, const option::Option& opt, const char* msg2)
    {
#if defined(_WIN32)
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
#endif
    }

    static option::ArgStatus Required(const option::Option& option, bool msg)
    {
        if (option.arg != 0)
            return option::ARG_OK;

        if (msg) PrintError("Opcion '", option, "' requiere un argumento.\n");
        return option::ARG_ILLEGAL;
    }
};

enum  OptionIndex {
    UNKNOWN, INPUT_FILE, VIEW_EXECUTION, VIEW_INTERVAL , HELP
};

const option::Descriptor usage[] =
{
    {UNKNOWN, 0, "", "",Arg::None, "INSTRUCTIONS: VirtualCPU [options]\n"},
    {INPUT_FILE,  0,"i", "input_file", Arg::Required, "  -i <path>, \t--input_file=<path>  \tPath to executable." },
    {VIEW_EXECUTION,  0,"v","view_execution", Arg::None,"  -v , \t--view_execution  \tShow status of each execution cycle." },
    {VIEW_INTERVAL,  0, "c", "view_interval", Arg::Required, "  -c <num>, \t--view_interval=<num>  \tCPU cycles between every execution view." },
    {HELP, 0,"", "help", Arg::None,    "  \t--help  \tShow instructions." },
    {0,0,0,0,0,0}
};

/****************************
*                           *
*    Terminal Functions     *
*                           *
****************************/

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

/****************************
*                           *
*       Main Program        *
*                           *
****************************/

int main(int argc, char* argv[])
{
    // Default parameters
    string inputFile;
    bool viewExecution = false;
    int viewInterval = 1;

    // Execute argument parser
    argc -= (argc > 0); argv += (argc > 0);
    option::Stats  stats(usage, argc, argv);
    option::Option* options = new option::Option[stats.options_max];
    option::Option* buffer = new option::Option[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);

    // Process argument parser options
    if (parse.error())
        return 1;

    if (options[HELP] || argc == 0)
    {
        option::printUsage(cout, usage);
        return 0;
    }

    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
        cout << "Invalid option: " << opt->name << "\n";

    for (int i = 0; i < parse.optionsCount(); ++i)
    {
        option::Option& opt = buffer[i];
        switch (opt.index())
        {
        case INPUT_FILE:
            inputFile = opt.arg;
            break;
        case VIEW_EXECUTION:
            viewExecution = true;
            break;
        case VIEW_INTERVAL:
            viewInterval = stoi(opt.arg);
            break;
        }
    }

    delete[] options;
    delete[] buffer;

    // Start VirtualCPU execution
    VirtualCPU cpu;
    cpu.LoadExecutableFromFile(inputFile);

    int i = 0;
    while (!cpu.IsHalted())
    {
        cpu.PerformCycle();
        if (viewExecution && (i % viewInterval == 0))
        {
            cpu.DrawRegistersStatus();
            cpu.DrawBuffersOutput();
            pause();
            clear_screen();
        }

        i++;
    }

    cpu.DrawRegistersStatus();
    cpu.DrawBuffersOutput();
}