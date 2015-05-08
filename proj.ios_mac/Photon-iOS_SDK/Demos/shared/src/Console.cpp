#include "Console.h"

Console::Console(void)
	: mImp(*reinterpret_cast<ConsoleImplementation*>(NULL))
{
}

Console::~Console(void)
{
}

void Console::update(void)
{
}

void Console::write(const ExitGames::Common::JString& str)
{
	std::wcout << str;
}

void Console::writeLine(const ExitGames::Common::JString& str)
{
	std::wcout << str << std::endl;
}

Console& Console::get(void)
{
	static Console console;
	return console;
}