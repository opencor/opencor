/**
 * @file node.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_NODE_HPP__
#define CMZN_NODE_HPP__

#include "opencmiss/zinc/node.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/timesequence.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Fieldmodule;
class Nodeset;
class NodesetGroup;
class Nodetemplate;

class Node
{
private:

	cmzn_node_id id;

public:

	Node() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Node(cmzn_node_id node_id) : id(node_id)
	{ }

	Node(const Node& node) :
		id(cmzn_node_access(node.id))
	{ }

	/**
	 * Node change bit flags returned by Nodesetchanges query functions.
	 */
	enum ChangeFlag
	{
		CHANGE_FLAG_NONE = CMZN_NODE_CHANGE_FLAG_NONE,
		CHANGE_FLAG_ADD = CMZN_NODE_CHANGE_FLAG_ADD,
		CHANGE_FLAG_REMOVE = CMZN_NODE_CHANGE_FLAG_REMOVE,
		CHANGE_FLAG_IDENTIFIER = CMZN_NODE_CHANGE_FLAG_IDENTIFIER,
		CHANGE_FLAG_DEFINITION = CMZN_NODE_CHANGE_FLAG_DEFINITION,
		CHANGE_FLAG_FIELD = CMZN_NODE_CHANGE_FLAG_FIELD
	};

	/**
	 * Type for passing logical OR of #ChangeFlag
	 */
	typedef int ChangeFlags;

	enum ValueLabel
	{
		VALUE_LABEL_INVALID = CMZN_NODE_VALUE_LABEL_INVALID,
		VALUE_LABEL_VALUE = CMZN_NODE_VALUE_LABEL_VALUE,                /*!< literal field value */
		VALUE_LABEL_D_DS1 = CMZN_NODE_VALUE_LABEL_D_DS1,                /*!< derivative w.r.t. arc length S1 */
		VALUE_LABEL_D_DS2 = CMZN_NODE_VALUE_LABEL_D_DS2,                /*!< derivative w.r.t. arc length S2 */
		VALUE_LABEL_D2_DS1DS2 = CMZN_NODE_VALUE_LABEL_D2_DS1DS2,        /*!< cross derivative w.r.t. arc lengths S1,S2 */
		VALUE_LABEL_D_DS3 = CMZN_NODE_VALUE_LABEL_D_DS3,                /*!< derivative w.r.t. arc length S3 */
		VALUE_LABEL_D2_DS1DS3 = CMZN_NODE_VALUE_LABEL_D2_DS1DS3,        /*!< cross derivative w.r.t. arc lengths S1,S3 */
		VALUE_LABEL_D2_DS2DS3 = CMZN_NODE_VALUE_LABEL_D2_DS2DS3,        /*!< cross derivative w.r.t. arc lengths S2,S3 */
		VALUE_LABEL_D3_DS1DS2DS3 = CMZN_NODE_VALUE_LABEL_D3_DS1DS2DS3,  /*!< triple cross derivative w.r.t. arc lengths S1,S2,S3 */
	};

	Node& operator=(const Node& node)
	{
		cmzn_node_id temp_id = cmzn_node_access(node.id);
		if (0 != id)
		{
			cmzn_node_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Node()
	{
		if (0 != id)
		{
			cmzn_node_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_node_id getId() const
	{
		return id;
	}

	int getIdentifier()
	{
		return cmzn_node_get_identifier(id);
	}

	int setIdentifier(int identifier)
	{
		return cmzn_node_set_identifier(id, identifier);
	}

	inline Nodeset getNodeset() const;

	int merge(const Nodetemplate& nodeTemplate);

};

inline bool operator==(const Node& a, const Node& b)
{
	return a.getId() == b.getId();
}

class Nodetemplate
{
private:

	cmzn_nodetemplate_id id;

public:

	Nodetemplate() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Nodetemplate(cmzn_nodetemplate_id node_template_id) :
		id(node_template_id)
	{ }

	Nodetemplate(const Nodetemplate& nodeTemplate) :
		id(cmzn_nodetemplate_access(nodeTemplate.id))
	{ }

	Nodetemplate& operator=(const Nodetemplate& nodeTemplate)
	{
		cmzn_nodetemplate_id temp_id = cmzn_nodetemplate_access(nodeTemplate.id);
		if (0 != id)
		{
			cmzn_nodetemplate_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Nodetemplate()
	{
		if (0 != id)
		{
			cmzn_nodetemplate_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_nodetemplate_id getId() const
	{
		return id;
	}

	int defineField(const Field& field)
	{
		return cmzn_nodetemplate_define_field(id, field.getId());
	}

	int defineFieldFromNode(const Field& field, const Node& node)
	{
		return cmzn_nodetemplate_define_field_from_node(id, field.getId(), node.getId());
	}

	Timesequence getTimesequence(const Field& field)
	{
		return Timesequence(cmzn_nodetemplate_get_timesequence(id, field.getId()));
	}

	int setTimesequence(const Field& field, const Timesequence& timesequence)
	{
		return cmzn_nodetemplate_set_timesequence(id, field.getId(), timesequence.getId());
	}

	int getValueNumberOfVersions(const Field& field, int componentNumber,
		Node::ValueLabel valueLabel)
	{
		return cmzn_nodetemplate_get_value_number_of_versions(id, field.getId(),
			componentNumber, static_cast<cmzn_node_value_label>(valueLabel));
	}

	int setValueNumberOfVersions(const Field& field, int componentNumber,
		Node::ValueLabel valueLabel, int numberOfVersions)
	{
		return cmzn_nodetemplate_set_value_number_of_versions(id, field.getId(),
			componentNumber, static_cast<cmzn_node_value_label>(valueLabel), numberOfVersions);
	}

	int removeField(const Field& field)
	{
		return cmzn_nodetemplate_remove_field(id, field.getId());
	}
	int undefineField(const Field& field)
	{
		return cmzn_nodetemplate_undefine_field(id, field.getId());
	}
};

class Nodeiterator
{
private:

	cmzn_nodeiterator_id id;

public:

	Nodeiterator() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Nodeiterator(cmzn_nodeiterator_id node_iterator_id) :
		id(node_iterator_id)
	{ }

	Nodeiterator(const Nodeiterator& nodeIterator) :
		id(cmzn_nodeiterator_access(nodeIterator.id))
	{ }

	Nodeiterator& operator=(const Nodeiterator& nodeIterator)
	{
		cmzn_nodeiterator_id temp_id = cmzn_nodeiterator_access(nodeIterator.id);
		if (0 != id)
		{
			cmzn_nodeiterator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Nodeiterator()
	{
		if (0 != id)
		{
			cmzn_nodeiterator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Node next()
	{
		return Node(cmzn_nodeiterator_next(id));
	}
};

class Nodeset
{
protected:

	cmzn_nodeset_id id;

public:

	Nodeset() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Nodeset(cmzn_nodeset_id nodeset_id) : id(nodeset_id)
	{  }

	Nodeset(const Nodeset& nodeset) :
		id(cmzn_nodeset_access(nodeset.id))
	{  }

	Nodeset& operator=(const Nodeset& nodeset)
	{
		cmzn_nodeset_id temp_id = cmzn_nodeset_access(nodeset.id);
		if (0 != id)
		{
			cmzn_nodeset_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Nodeset()
	{
		if (0 != id)
		{
			cmzn_nodeset_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_nodeset_id getId() const
	{
		return id;
	}

	inline NodesetGroup castGroup();

	bool containsNode(const Node& node)
	{
		return cmzn_nodeset_contains_node(id, node.getId());
	}

	Nodetemplate createNodetemplate()
	{
		return Nodetemplate(cmzn_nodeset_create_nodetemplate(id));
	}

	Node createNode(int identifier, const Nodetemplate& nodeTemplate)
	{
		return Node(cmzn_nodeset_create_node(id, identifier, nodeTemplate.getId()));
	}

	Nodeiterator createNodeiterator()
	{
		return Nodeiterator(cmzn_nodeset_create_nodeiterator(id));
	}

	int destroyAllNodes()
	{
		return cmzn_nodeset_destroy_all_nodes(id);
	}

	int destroyNode(const Node& node)
	{
		return cmzn_nodeset_destroy_node(id, node.getId());
	}

	int destroyNodesConditional(const Field& conditionalField)
	{
		return cmzn_nodeset_destroy_nodes_conditional(id, conditionalField.getId());
	}

	Node findNodeByIdentifier(int identifier)
	{
		return Node(cmzn_nodeset_find_node_by_identifier(id, identifier));
	}

	inline Fieldmodule getFieldmodule() const;

	Nodeset getMasterNodeset()
	{
		return Nodeset(cmzn_nodeset_get_master_nodeset(id));
	}

	char *getName()
	{
		return cmzn_nodeset_get_name(id);
	}

	int getSize()
	{
		return cmzn_nodeset_get_size(id);
	}

};

inline bool operator==(const Nodeset& a, const Nodeset& b)
{
	return cmzn_nodeset_match(a.getId(), b.getId());
}

inline Nodeset Node::getNodeset() const
{
	return Nodeset(cmzn_node_get_nodeset(id));
}

class NodesetGroup : public Nodeset
{

public:

	// takes ownership of C handle, responsibility for destroying it
	explicit NodesetGroup(cmzn_nodeset_group_id nodeset_id) : Nodeset(reinterpret_cast<cmzn_nodeset_id>(nodeset_id))
	{ }

	NodesetGroup()
	{ }

	cmzn_nodeset_group_id getId() const
	{
		return (cmzn_nodeset_group_id)(id);
	}

	int addNode(const Node& node)
	{
		return cmzn_nodeset_group_add_node(
			reinterpret_cast<cmzn_nodeset_group_id>(id), node.getId());
	}

	int addNodesConditional(const Field& conditionalField)
	{
		return cmzn_nodeset_group_add_nodes_conditional(
			reinterpret_cast<cmzn_nodeset_group_id>(id), conditionalField.getId());
	}

	int removeAllNodes()
	{
		return cmzn_nodeset_group_remove_all_nodes(
			reinterpret_cast<cmzn_nodeset_group_id>(id));
	}

	int removeNode(const Node& node)
	{
		return cmzn_nodeset_group_remove_node(reinterpret_cast<cmzn_nodeset_group_id>(id),
			node.getId());
	}

	int removeNodesConditional(const Field& conditionalField)
	{
		return cmzn_nodeset_group_remove_nodes_conditional(
			reinterpret_cast<cmzn_nodeset_group_id>(id), conditionalField.getId());
	}

};

inline NodesetGroup Nodeset::castGroup()
{
	return NodesetGroup(cmzn_nodeset_cast_group(id));
}

class Nodesetchanges
{
private:

	cmzn_nodesetchanges_id id;

public:

	Nodesetchanges() : id(0)
	{ }

	// takes ownership of C handle, responsibility for destroying it
	explicit Nodesetchanges(cmzn_nodesetchanges_id nodesetchanges_id) :
		id(nodesetchanges_id)
	{ }

	Nodesetchanges(const Nodesetchanges& nodesetchanges) :
		id(cmzn_nodesetchanges_access(nodesetchanges.id))
	{ }

	Nodesetchanges& operator=(const Nodesetchanges& nodesetchanges)
	{
		cmzn_nodesetchanges_id temp_id = cmzn_nodesetchanges_access(nodesetchanges.id);
		if (0 != id)
			cmzn_nodesetchanges_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Nodesetchanges()
	{
		if (0 != id)
			cmzn_nodesetchanges_destroy(&id);
	}

	bool isValid() const
	{
		return (0 != id);
	}

	Node::ChangeFlags getNodeChangeFlags(const Node& node)
	{
		return cmzn_nodesetchanges_get_node_change_flags(id, node.getId());
	}

	int getNumberOfChanges()
	{
		return cmzn_nodesetchanges_get_number_of_changes(id);
	}

	Node::ChangeFlags getSummaryNodeChangeFlags()
	{
		return cmzn_nodesetchanges_get_summary_node_change_flags(id);
	}
};

inline int Node::merge(const Nodetemplate& nodeTemplate)
{
	return cmzn_node_merge(id, nodeTemplate.getId());
}

}  // namespace Zinc
}

#endif
