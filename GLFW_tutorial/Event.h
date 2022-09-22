
#ifndef EVENT_H
#define EVENT_H
#include "EventEntites.h"
#include <list>
#include "GLFW.h"
#include "GLM.h"
class Event {

public:
	enum Type : int {

		KeyReleased,
		KeyPressed,
		KeyRepeated,
		TextEntered,
		MouseButtonReleased,
		MouseButtonPressed,
		CursorLeft,
		CursorEntered,
		MouseScrolled,
		MouseMoved,
		WindowResized,
		WindowMoved,
		LostFocus,
		GainedFocus,
		SizeType,
	};
public:
	Type type;
public:

	

	struct SizeEvent {
		glm::ivec2 size;
		SizeEvent(const glm::ivec2& size_):size(size_) {}
	};
	struct MoveEvent {
		glm::ivec2 pos;
		MoveEvent(const glm::ivec2& pos_) :pos(pos_) {}
	};
	struct KeyEvent {
		int code;
		int mods;
		KeyEvent(int code_, int mods_) :code(code_), mods(mods_) {}
	};

	struct TextEvent {
		unsigned int unicode;
		TextEvent(unsigned int codepoint_):unicode(codepoint_) {}
	};

	struct MouseMoveEvent {
		glm::vec2 pos;
		MouseMoveEvent(const glm::vec2& pos_):pos(pos_){}
	};

	struct MouseButtonEvent {
		glm::vec2 pos;
		int button;
		MouseButtonEvent(int button_, const glm::vec2& pos_) :pos(pos_),button(button_) {}
	};

	struct MouseScrollEvent {
		int yOffset;
		MouseScrollEvent(int yOffset_) :yOffset(yOffset_) {}
	};

	union
	{
		SizeEvent      size;      //!< Size event parameters (Event::Resized)
		MoveEvent      move;      //!< Pos event parameters (Event::WindowMoved)
		KeyEvent       key;       //!< Key event parameters (Event::KeyPressed, Event::KeyReleased)
		TextEvent      text;      //!< Text event parameters (Event::TextEntered)
		MouseMoveEvent mouseMove; //!< Mouse move event parameters (Event::MouseMoved)
		MouseButtonEvent mouseButton; //!< Mouse button event parameters (Event::MouseButtonPressed, Event::MouseButtonReleased)
		MouseScrollEvent mouseScroll; //!< Mouse wheel event parameters (Event::MouseWheelScrolled)
	};

	Event(const SizeEvent& event) :						type(WindowResized),	size(event) {}
	Event(const MoveEvent& event) :						type(WindowMoved),		move(event) {}
	Event(Type type_, const KeyEvent& event) :			type(type_),			key(event) {}
	Event(const TextEvent& event) :						type(TextEntered),		text(event) {}
	Event(const MouseMoveEvent& event) :				type(MouseMoved),		mouseMove(event) {}
	Event(Type type_, const MouseButtonEvent& event):	type(type_),			mouseButton(event) {}
	Event(const MouseScrollEvent& event) :				type(MouseScrolled),	mouseScroll(event) {}

	Event(Type type_) : type(type_){}
	Event()  {}
};

struct Key
{
	enum Type :int
	{
		Release = GLFW_RELEASE, 
		Press= GLFW_PRESS, 
		Repeat= GLFW_REPEAT,
	};

	int action;
	int mods;
};

class EventStack {

public:

	void push(const Event&event){
		events.push_back(event);
	}

	Event& get() {
		return events.front();
	}

	bool isEmpty() {
		return events.size() == 0;
	}

	void next() {
		events.pop_front();
	}
	Event& last() {
		return events.back();
	}
private:

	std::list<Event> events;
};
#endif