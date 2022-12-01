#ifndef COMMAND_H
#define COMMAND_H
#include "Game/Entities/Player.h"

class Scene;

//Command------------------------------------------------------------------
class Command
{
public:

	virtual void execute(Event& event) = 0;
	//virtual void execute(Event::Type type,) = 0;

private:
};
/// Все возможные изощренные действия
namespace com {
	//Player
	class ComPlayer :public Command {

	public: ComPlayer(Player* player_) :player(player_) {}

	protected: Player* player;

	};

	class UpdateMeshChunks :public Command {
	public:
		UpdateMeshChunks(Scene* s) :scene(s) {}
		void execute(Event& event);
	private: Scene* scene;
	};

	namespace pl {

		class Jump :public ComPlayer {
		public:
			Jump(Player* p) :ComPlayer(p) {}
			void execute(Event& event);
		};

		class Forward :public ComPlayer {
		public:
			Forward(Player* p) :ComPlayer(p) {}
			void execute(Event& event);
		};

		class Left :public ComPlayer {
		public:
			Left(Player* p) :ComPlayer(p) {}
			void execute(Event& event);
		};

		class Right :public ComPlayer {
		public:
			Right(Player* p) :ComPlayer(p) {}
			void execute(Event& event);
		};

		class Back :public ComPlayer {
		public:
			Back(Player* p) :ComPlayer(p) {}
			void execute(Event& event);
		};

		class HandCreate :public ComPlayer {
		public:
			HandCreate(Player* p, Scene* s) :ComPlayer(p), scene(s) {}
			void execute(Event& event);
		private:
			Scene* scene;
		};

		class HandDestroy:public ComPlayer {
		public:
			HandDestroy(Player* p, Scene* s) :ComPlayer(p),scene(s) {}
			void execute(Event& event);
		private:
			Scene* scene;
		};

	}

	class cmNull :public Command {
	public:
		void execute(Event& event) {}
	};
	static cmNull Null;
}
#endif

