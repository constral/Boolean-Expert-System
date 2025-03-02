#ifndef CORTEX_H
#define CORTEX_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>



class Set;
class Relation;
class Variable;
class Chain;
class WorkingMem;



class Set {

	std::string set_name;						    //
	std::vector<Relation*> relations_vector;		// where the relations are held
	bool searched_through;					        // aux variable for performing BFS, should be demarked after BFS is finished

public:

	Set(std::string name);

	std::string getSetName();

	bool wasSearched();

	int getRelationsCount();

	Relation* getRelationAt(int position);

	Set* getEndOfRelationAt(int position);

	void addRelation(Set* end_point, int type, bool order);

	void removeRelation(Set* end_point_to_remove, int type_to_remove, bool order_to_remove);

	void searchMarkAs(bool mark);

};

class Relation {

	Set* end_A;
	Set* end_B;
	int relation_type;
	bool LtR_order;

public:

	Relation(Set* A, Set* B, int type, bool order);

	Set* getEndpointA();
	Set* getEndpointB();
	Set* getEndpointOppositeOf(Set* what_end);

	int getRelationType();

	bool hasLtROrder();
};

class Variable {

	Set* linked_set;
	int variable_value;     // is not boolean because we can have the uncertainty state of 17 in it (welp, ideally we could signal it otherwise, but im patching it to avoid rewriting everything)

public:

	Variable(Set* set, int value);

	Set* getLinkedSet();

	int getVariableValue();

	void linkToSet(Set* set_to_link);

	void changeValueTo(int new_value);

};

class Chain {

	// sets in the order that they have to be used
	// VTD has to not be in the chain

	std::vector<Set*> chain_sets_vector;
	std::vector<Relation*> chain_relations_vector;
	int chain_size;

public:

	Chain();

	int getChainSize();

	int getSetsCount();

	Set* getSetAt(int position);

	int getRelationsCount();

	Relation* getRelationAt(int position);

	void addChainSet(Set* set_to_add);

	void addChainRelation(Relation* relation_to_add);

	void addChainComponent(Set* set_to_add, Relation* relation_to_add);
};



class WorkingMem {

	std::vector<Set*> wms_sets_vector;
	std::vector<Variable*> wms_variables_vector;

public:

	WorkingMem();


	int getSetsCount();

	Set* getSetAt(int position);

	int getVariablesCount();

	Variable* getVariableAt(int position);


	// add pointers
	void WMS_Add_Set(Set* set_to_add);

	void WMS_Add_Variable(Variable* variable_to_add);


	// check existence of items
	bool WMS_Check_Exists_Set(std::string set_to_check);

	bool WMS_Check_Exists_Variable(std::string variable_to_check);


	// return pointers
	Set* WMS_Find_Set(Set* set_to_find);

	Variable* WMS_Find_Variable(Variable* variable_to_find);


	// delete items
	void WMS_Clear_Variables_Vector();

	void WMS_Clear_Marked_Sets();
};



Variable* booleanSolve(Variable premise_variable, Relation premise_relation);

Variable* deduce(Chain* chain_to_use, int current_chain_pos);

Chain* pathfind(Set* set_to_determine, Set* known_set);

void interpretQuery(std::vector<std::string> data_text);



extern WorkingMem WorkingMemory;

#endif
