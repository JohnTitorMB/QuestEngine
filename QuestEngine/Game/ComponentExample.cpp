#include "ComponentExample.h"

ComponentExample::ComponentExample() : Component()
{
	
}

ComponentExample::ComponentExample(const ComponentExample& componentExample) : Component(componentExample)
{
}

void ComponentExample::Start()
{
	Component::Start();
}


void ComponentExample::Update()
{
	Component::Update();
}




ComponentExample::~ComponentExample()
{
}

Component* ComponentExample::Clone()
{
	ComponentExample* componentExample = new ComponentExample(*this);
	clonnedObject = componentExample;
	clonnedObject->baseObject = this;
	return componentExample;
}

void ComponentExample::AssignPointerAndReference()
{
	Component::AssignPointerAndReference();
}
