#include "ToExecutable.h"
#include <Windows.h>
#include <string>
#include <fstream>

void ToExecutable(std::string AssemblySource, std::string OutputPath)  
{  
    std::string AsmPath = OutputPath + ".asm";
    std::string ObjPath = OutputPath + ".obj";
    std::string ExePath = OutputPath + ".exe";
    
    std::ofstream OutputFile(AsmPath);
    OutputFile << AssemblySource;
    OutputFile.close();

	std::cout << AssemblySource << "\n";

    // Assemble to object file
    if (system(("ml64 /c /Fo " + ObjPath + " " + AsmPath).c_str()) != 0)
    {
		std::cerr << "Error: Assembly failed.\n";
		std::cin.get();
        exit(1);
    }

    const std::string LIBS =
        " /LIBPATH:\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\ucrt\\x64\""
        " /LIBPATH:\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64\""
        " /LIBPATH:\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.42.34433\\lib\\x64\""
		" /LIBPATH:\"C:\\Users\\anisr\\source\\repos\\QuirkTC-Windowsx86-64\\x64\\Debug\""
        " Quirk-Standard-Library-Windows.lib ucrt.lib ucrtd.lib kernel32.lib vcruntime.lib";

    /*std::string STDIO_LIBS =
        "\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.42.34433\\lib\\x64\\legacy_stdio_definitions.lib\" "
        "\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.42.34433\\lib\\x64\\legacy_stdio_wide_specifiers.lib\"";*/

    // Link to create executable (NO extra libs or subsystem flags)
    if (system(("link " + ObjPath + " /subsystem:console " + " /ENTRY:main /OUT:" + ExePath + " " + LIBS).c_str()) != 0)
    {
		std::cerr << "Error: Linking failed.\n";
        std::cin.get();
    }

	printf("Executable created: %s\n", ExePath.c_str());

    // Cleanup
    system(("del " + ObjPath).c_str());
    system(("del " + AsmPath).c_str());
}

/*std::string PathEnv = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.29.30133\\bin\\HostX64\\x64;";
SetEnvironmentVariable(L"PATH", std::wstring(PathEnv.begin(), PathEnv.end()).c_str());*/