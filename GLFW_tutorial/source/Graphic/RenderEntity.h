#ifndef RENDER_ENTITY_H
#define RENDER_ENTITY_H

#include "Math/GLM.h"
#include<iostream>
class FramebufferId {
	struct ID {
		ID();
		~ID();
		unsigned int id_;
	};
	std::shared_ptr<ID> id_ = 0;

public:
	FramebufferId() :id_(std::make_shared<ID>()) {}
	inline bool remake()noexcept {
		if (id_.unique() == 0) {
			id_ = std::make_shared<ID>();
			return 1;
		}
		return 0;
	}
	inline unsigned int get()const noexcept { return id_->id_; }
};

class RenderbufferId {
	struct ID {
		ID();
		~ID();
		unsigned int id_;
	};
	std::shared_ptr<ID> id_ = 0;
public:
	RenderbufferId() :id_(std::make_shared<ID>()) {}
	inline bool remake()noexcept {
		if (id_.unique() == 0) {
			id_ = std::make_shared<ID>();
			return 1;
		}
		return 0;
	}
	inline unsigned int get()const noexcept { return id_->id_; }
};

/// <summary>
/// iFrame
/// Èםעונפויס
/// </summary>
class iFramebuffer {

public:

	virtual const glm::ivec2& size()const = 0;
	inline unsigned int id()const  {return id_.get();}

protected:
	FramebufferId id_;
};
#endif

