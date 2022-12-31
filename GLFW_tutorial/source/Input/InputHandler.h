#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Command.h"
#include"Event.h"

class Scene;

///InputHandler---------------------------------------------------
class InputHandler
{
public:

	enum CommandInterface:size_t
	{
		kW,kS,kA,kD,kU,kSpace, mLeft, mRight,mScroll, mMiddle,size_com
	};

	InputHandler() {
		for (size_t i = 0; i < size_com; i++)command[i] = &com::Null;
	}
	void solveScroll(Event& event);
	void solveMouse(Event& event);
	void solveKey(Event& event);

	Command* command[size_com];

private:

};

///sCommandHandler---------------------------------------------------
class sCommandHandler {
public:

	static void init(InputHandler& input, GlobalScene* scene, Player* player);

private:

	sCommandHandler() {}
	~sCommandHandler() {}

};

#endif
