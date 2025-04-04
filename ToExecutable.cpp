#include "ToExecutable.h"
#include <Windows.h>
#include <string>
#include <fstream>

void ToExecutable(std::string AssemblySource, std::string OutputPath)  
{  
    std::string AsmPath = OutputPath + ".asm";
    std::string ObjPath = OutputPath + ".obj";
    std::string ExePath = OutputPath + ".qrkexe";

    // Write the assembly source to a .asm file
    std::ofstream OutputFile(AsmPath);
    OutputFile << AssemblySource;
    OutputFile.close();

	std::cout << AssemblySource << "\n";

    //std::string Path = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.29.30133\\bin\\HostX64\\x64\\";

    std::string PathEnv = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.29.30133\\bin\\HostX64\\x64;";
    SetEnvironmentVariable(L"PATH", std::wstring(PathEnv.begin(), PathEnv.end()).c_str());

    // Assemble to object file
    system(("ml64 /c /Fo " + ObjPath + " " + AsmPath).c_str());

    // Link to create executable (NO extra libs or subsystem flags)
    system(("link " + ObjPath + " /ENTRY:main " + " /OUT:" + ExePath).c_str());

    // Cleanup
    system(("del " + ObjPath).c_str());
    system(("del " + AsmPath).c_str());

    //std::cout << "Executable created: " << ExePath << "\n";
}
