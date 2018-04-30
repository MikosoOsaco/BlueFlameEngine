#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

namespace ENGINE {

	class Component 
	{
	public:
		virtual void Update() {};
		virtual void Render() {};
	};
}

#endif
