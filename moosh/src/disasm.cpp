#include "msg_handler.hpp"

#include <parser.hpp>
#include <disassembler.hpp>
#include <bc_program.hpp>
#include <type_registry.hpp>
#include <intvar.hpp>
#include <realvar.hpp>
#include <strvar.hpp>
#include <compiler.hpp>

#include <iostream>
#include <fstream>
#include <memory>

extern "C" {

#include <unistd.h>

}

using namespace Moove;

std::auto_ptr<BytecodeProgram> compile(const Program& prog)
{
    TypeRegistry typeReg;
    
    typeReg.registerType("int", DefaultIntVar::classFactory());
    typeReg.registerType("real", DefaultRealVar::classFactory());
    typeReg.registerType("str", DefaultStrVar::classFactory());
    typeReg.registerType("list", DefaultListVar::classFactory());

    Compiler compiler(typeReg);

    return compiler.compile(prog);
}

std::auto_ptr<DebugBytecodeProgram> compileDebug(const Program& prog)
{
    TypeRegistry typeReg;
    
    typeReg.registerType("int", DefaultIntVar::classFactory());
    typeReg.registerType("real", DefaultRealVar::classFactory());
    typeReg.registerType("str", DefaultStrVar::classFactory());
    typeReg.registerType("list", DefaultListVar::classFactory());

    Compiler compiler(typeReg);

    return compiler.compileDebug(prog);
}

int main(int argc, char **argv)
{
    try {
	int  opt;
	bool debugOpt = false;

	while((opt = getopt(argc, argv, "dh")) != -1) {
	    switch (opt) {
		case 'd':
		    debugOpt = true;
		    break;

		case '?':
		    std::cerr << "Unrecognized option: -" << optopt << std::endl;
		    
		case 'h':
		    std::cout << "Usage: " << argv[0] << " [-d] [-h]\n";
		    return 1;
	    }
	}

        std::string source, line;
        std::ifstream file;
        std::istream* inStream = &std::cin;
        unsigned lineOffset = 0;

	if (optind < argc) {
            file.open(argv[optind], std::ios::in);
	    if(!file) {
                std::cerr << "Error opening file '" << argv[optind] << "'\n";
                return 1;
            }

            inStream = &file;
        }

        while(std::getline(*inStream, line)) {
            if(source.empty() && line.length() > 2 && line[0] == '#' && line[1] == '!') {
                // ignore executable line. Increase lineOffset to keep line numbers correct
                ++lineOffset;
                continue;
            }

            source += line + '\n';
        }

        MessageHandler msgs(lineOffset);
        Parser parser;
        if(parser.parse(source, msgs, false)) {
            std::auto_ptr<Program> program = parser.releaseProgram();

	    if (debugOpt) {
		std::auto_ptr<DebugBytecodeProgram> bc = compileDebug(*program);

		disassemble(std::cout, *bc);
	    } else {
		std::auto_ptr<BytecodeProgram> bc = compile(*program);

		disassemble(std::cout, *bc);
	    }
        } else {
            return 1;
        }
    } catch(const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}

