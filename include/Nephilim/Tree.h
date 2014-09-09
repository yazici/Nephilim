#ifndef PARABOLA_TREE_H
#define PARABOLA_TREE_H

#include <Nephilim/Platform.h>
#include "Strings.h"

NEPHILIM_NS_BEGIN

	/**
		\ingroup Foundation
		\class Tree
		\brief A tree structure utility
	*/
	class NEPHILIM_API Tree{
	public:
		
		/// Get the name of the node
		String name();

	private:
		String node_name;
	};

	/**
		\ingroup Foundation
		\class TreeContainerNode
		\brief Custom node to hold an object of type T
	*/
	template<class T>
	class NEPHILIM_API TreeContainerNode : Tree{
	public:
		T value;
	};

NEPHILIM_NS_END
#endif