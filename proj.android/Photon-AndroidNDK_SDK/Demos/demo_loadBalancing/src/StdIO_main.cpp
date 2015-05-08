#include "Console.h"
#include "NetworkLogic.h"

int getcharIfKbhit(void);

#ifdef _EG_WINDOWS_PLATFORM
#include <conio.h>

int getcharIfKbhit(void)
{
	int res = _kbhit();
	if(res)
		res = _getch();
	return res;
}
#elif defined _EG_PS4_PLATFORM
#include "Controller.h"

int getcharIfKbhit(void)
{
	static Controller controller;
	return controller.readInput();
}
#else
#include <termios.h>
#include <fcntl.h>

int getcharIfKbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	return ch;
}
#endif

#if defined _EG_WINDOWS_PLATFORM
#	define SLEEP(ms) Sleep(ms)
#else
#	include <unistd.h>
#	define SLEEP(ms) usleep(ms*1000)
#endif


class StdUI: public NetworkLogicListener
{
public:
	void main();

private:
	virtual void stateUpdate(State newState)
	{}
	void usage();
private:
};

void StdUI::usage()
{
#ifdef _EG_PS4_PLATFORM
	Console::get().writeLine(L"Photon LoadBalancing Demo");
	Console::get().writeLine(L"usage:");
	Console::get().writeLine(L"always:");
	Console::get().writeLine(L" Triangle - print this help");
	Console::get().writeLine(L" Circle - exit");
	Console::get().writeLine(L"--------------------");
	Console::get().writeLine(L"outside a game room:");
	Console::get().writeLine(L" Cross - create game");
	Console::get().writeLine(L" Square - join random game or last joined game");
	Console::get().writeLine(L"--------------------");
	Console::get().writeLine(L"inside a game room:");
	Console::get().writeLine(L" Cross - leave game");
	Console::get().writeLine(L" Square - leave game (will come back - disconnects and quits)");
	Console::get().writeLine(L"--------------------");
#else
	Console::get().writeLine(L"Photon LoadBalancing Demo");
	Console::get().writeLine(L"usage:");
	Console::get().writeLine(L"always:");
	Console::get().writeLine(L" h - print this help");
	Console::get().writeLine(L" 0 - exit");
	Console::get().writeLine(L"--------------------");
	Console::get().writeLine(L"outside a game room:");
	Console::get().writeLine(L" 1 - create game");
	Console::get().writeLine(L" 2 - join random game or last joined game");
	Console::get().writeLine(L"--------------------");
	Console::get().writeLine(L"inside a game room:");
	Console::get().writeLine(L" 1 - leave game");
	Console::get().writeLine(L" 2 - leave game (will come back - disconnects and quits)");
	Console::get().writeLine(L"--------------------");
#endif
}

void StdUI::main()
{
	NetworkLogic networkLogic(&Console::get(), ExitGames::LoadBalancing::AuthenticationValues(ExitGames::LoadBalancing::CustomAuthenticationType::CUSTOM, L"username=yes&token=yes"));
	usage();
	networkLogic.connect();
	bool exit = false;
	while(networkLogic.getLastInput() != INPUT_EXIT && !exit)
	{
		int ch = getcharIfKbhit();
		switch(ch)
		{
			case 'h':
				usage();
				break;
			case '0':
				networkLogic.setLastInput(INPUT_EXIT);
				exit = true;
				break;
			default:
				networkLogic.setLastInput(INPUT_NON);
				break;
		}
		switch(ch)
		{
		case '1':
			networkLogic.setLastInput(INPUT_1);
			break;
		case '2':
			networkLogic.setLastInput(INPUT_2);
			break;
		}
		networkLogic.run();
		Console::get().update();
		SLEEP(100);
	}
}

int main(int argc, const char* argv[])
{
	StdUI ui;
	ui.main();
	return 0;
}