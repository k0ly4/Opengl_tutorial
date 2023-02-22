#include "RenderEntity.h"
#include "ContextRender.h"
FramebufferId::ID::ID() {	sRender::Framebuffer::gen(id_);}
FramebufferId::ID::~ID() {	sRender::Framebuffer::destroy(id_); }

RenderbufferId::ID::ID() {	sRender::RenderBuffer::gen(id_); }
RenderbufferId::ID::~ID() { sRender::RenderBuffer::destroy(id_); }