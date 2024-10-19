#include <memory>
#include <box2d/box2d.h>

class PhysicsWorld {
public:
private:
	static std::unique_ptr<b2World> m_world;
};