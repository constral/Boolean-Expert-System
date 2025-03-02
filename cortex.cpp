#include "files.h"
#include "cortex.h"
#include "speech.h"

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
class WorkingMemory;



Set::Set(std::string name) {
	set_name = name;
	searched_through = 0;
}

std::string Set::getSetName() {
	return set_name;
}

bool Set::wasSearched() {
	return searched_through;
}

int Set::getRelationsCount() {
	return relations_vector.size();
}

Relation* Set::getRelationAt(int position) {
	return relations_vector.at(position);
}

Set* Set::getEndOfRelationAt(int position) {
    if(relations_vector.at(position)->getEndpointA()->getSetName() == set_name)
        {
            return relations_vector.at(position)->getEndpointA();
        }
        else if(relations_vector.at(position)->getEndpointB()->getSetName() == set_name)
        {
            return relations_vector.at(position)->getEndpointB();
        }
}

void Set::addRelation(Set* end_point, int type, bool order) {
	bool reverse_order;
	if(order == 1)
	{
		reverse_order = 0;
	}
	else
	{
		reverse_order = 1;
	}

	// initialize new relation
	Relation* rel_1 = new Relation(this, end_point, type, order);
	Relation* rel_2 = new Relation(end_point, this, type, reverse_order);

	// add relation to relations_vector of both sets
	this->relations_vector.push_back(rel_1);
	end_point->relations_vector.push_back(rel_2);
}

void Set::removeRelation(Set* end_point_to_remove, int type_to_remove, bool order_to_remove) {
	bool reverse_order;
	if(order_to_remove == 1)
	{
		reverse_order = 0;
	}
	else
	{
		reverse_order = 1;
	}

	// loop through this' relations_vector
	for (int rel_1_cnt = 0; rel_1_cnt < relations_vector.size(); rel_1_cnt++)
	{
		// if the relation has the same type and endpoint, remove the item from the position
		// (LtR order is not accounted for)
		if(relations_vector[rel_1_cnt]->getEndpointB() == end_point_to_remove && relations_vector[rel_1_cnt]->getRelationType() == type_to_remove)
		{
			relations_vector.erase(relations_vector.begin() + rel_1_cnt);
		}
	}

	// loop through end_point_to_remove's relations_vector
	for (int rel_2_cnt = 0; rel_2_cnt < end_point_to_remove->relations_vector.size(); rel_2_cnt++)
	{
		// if the relation has the same type and endpoint, remove the item from the position
		// (LtR order is not accounted for)
		if(end_point_to_remove->relations_vector[rel_2_cnt]->getEndpointB() == this && end_point_to_remove->relations_vector[rel_2_cnt]->getRelationType() == type_to_remove)
		{
			end_point_to_remove->relations_vector.erase(end_point_to_remove->relations_vector.begin() + rel_2_cnt);
		}
	}
}

void Set::searchMarkAs(bool mark) {
	searched_through = mark;
}



///////////////////////////////



Relation::Relation(Set* A, Set* B, int type, bool order) {
	end_A = A;
	end_B = B;
	relation_type = type;
	LtR_order = order;
}

Set* Relation::getEndpointA() {
	return end_A;
}

Set* Relation::getEndpointB() {
	return end_B;
}

Set* Relation::getEndpointOppositeOf(Set* what_end) {
        if(what_end == end_A)
        {
            return end_B;
        }
        else if(what_end == end_B)
        {
            return end_A;
        }
}

int Relation::getRelationType() {
	return relation_type;
}

bool Relation::hasLtROrder() {
	return LtR_order;
}



///////////////////////////////



Variable::Variable(Set* set, int value) {
	linked_set = set;
	variable_value = value;
}

Set* Variable::getLinkedSet() {
	return linked_set;
}

int Variable::getVariableValue() {
	return variable_value;
}

void Variable::linkToSet(Set* set_to_link) {
	linked_set = set_to_link;
}

void Variable::changeValueTo(int new_value) {
	variable_value = new_value;
}



///////////////////////////////



Chain::Chain() {
	// create the chain_set_vector automatically in definition
	// create the chain_relation_vector automatically in definition

	chain_size = 0;
}

int Chain::getChainSize() {
	return chain_size;
}

int Chain::getSetsCount() {
	return chain_sets_vector.size();
}

Set* Chain::getSetAt(int position) {
	return chain_sets_vector[position];
}

int Chain::getRelationsCount() {
	return chain_relations_vector.size();
}

Relation* Chain::getRelationAt(int position) {
	return chain_relations_vector[position];
}


void Chain::addChainSet(Set* set_to_add) {
	// obviously the VTD is not added to the chain, and is the -1th element,
	// the last link in the chain is the KV
		// ...or the last queried entry, in which case it'd need to be extended while pathfinding in the static memory

	chain_sets_vector.push_back(set_to_add);
}

void Chain::addChainRelation(Relation* relation_to_add) {
	chain_relations_vector.push_back(relation_to_add);
}

void Chain::addChainComponent(Set* set_to_add, Relation* relation_to_add) {
	// obviously the VTD is not added to the chain, and is the -1th element,
	// the last link in the chain is the KV
		// ...or the last queried entry, in which case it'd need to be extended while pathfinding in the static memory

	chain_sets_vector.push_back(set_to_add);

	chain_relations_vector.push_back(relation_to_add);

        // chain size:
        chain_size++;
}



///////////////////////////////



WorkingMem::WorkingMem() {
	// create the wms_sets_vector automatically in definition
	// create the wms_variables_vector automatically in definition
}

int WorkingMem::getSetsCount() {
	return wms_sets_vector.size();
}

Set* WorkingMem::getSetAt(int position) {
	return wms_sets_vector[position];
}

int WorkingMem::getVariablesCount() {
	return wms_variables_vector.size();
}

Variable* WorkingMem::getVariableAt(int position) {
	return wms_variables_vector[position];
}

// add pointers
void WorkingMem::WMS_Add_Set(Set* set_to_add) {
	// push_back given set_to_add to wms_sets_vector
	wms_sets_vector.push_back(set_to_add);
}

void WorkingMem::WMS_Add_Variable(Variable* variable_to_add) {
	// push_back given variable_to_add to wms_variables_vector
	wms_variables_vector.push_back(variable_to_add);
}

// check existence of items
/// because cpp is messing with me, you will have to use these before the "return pointers" functions in order to avoid errors
bool WorkingMem::WMS_Check_Exists_Set(std::string set_to_check) {
    // loop through wms_sets_vector (coada -> cap)
	for(int c = wms_sets_vector.size()-1; c > -1; c--)
	{
		// if a set with the same name as the set_to_find is found in the working memory:
		if (wms_sets_vector[c]->getSetName() == set_to_check)
		{
			return true;
		}
	}

	return false;
}

bool WorkingMem::WMS_Check_Exists_Variable(std::string variable_to_check) {
    // loop through wms_variables_vector (coada -> cap)
	for(int c = wms_variables_vector.size()-1; c > -1; c--)
	{
		// if a set with the same name as the variable_to_find is found in the working memory:
		if (wms_variables_vector[c]->getLinkedSet()->getSetName() == variable_to_check)
		{
			return true;
		}
	}

	return false;
}

// return pointers
Set* WorkingMem::WMS_Find_Set(Set* set_to_find) {
	// loop through wms_sets_vector (coada -> cap)
	for(int c = wms_sets_vector.size()-1; c > -1; c--)
	{
		// if a set with the same name as the set_to_find is found in the working memory:
		if (wms_sets_vector[c]->getSetName() == set_to_find->getSetName())
		{
			return wms_sets_vector[c];
		}
	}
}

Variable* WorkingMem::WMS_Find_Variable(Variable* variable_to_find) {
	// loop through wms_variables_vector (coada -> cap)
	for(int c = wms_variables_vector.size()-1; c > -1; c--)
	{
		// if a set with the same name as the variable_to_find is found in the working memory:
		if (wms_variables_vector[c]->getLinkedSet()->getSetName() == variable_to_find->getLinkedSet()->getSetName())
		{
			return wms_variables_vector[c];
		}
	}
}

// delete items
void WorkingMem::WMS_Clear_Variables_Vector() {
    // resets the variables vector
    wms_variables_vector.clear();
}

void WorkingMem::WMS_Clear_Marked_Sets() {
    /// this function works better than wiping through the sets of each chain, assuming that:
            /// there are many more occurences of variables inside the entirety of the chains,
            /// than there are variables stored in wms_sets_vector

    // go through all sets in the memory
    for(int uj = 0; uj < wms_sets_vector.size(); uj++)
        {
            // mark set as not searched through
            wms_sets_vector.at(uj)->searchMarkAs(0);
        }
}





//
//  Solving functions
//

// applies a pattern to an argument of one variable and one relation
Variable* booleanSolve(Variable premise_variable, Relation premise_relation) {
	Variable* result_variable;
	int res_value;                     // resulting variable's value IS AN INT BECAUSE WE NEED TO KNOW IF ITS 17 AKA UNCERTAIN
	Set* res_linked_set;


	std::string var_name;
	int var_value;                 // this cant be 17 so no need to be an int

	std::string rel_name_1;
	std::string rel_name_2;
	int rel_type;
	bool rel_order;

	bool pat_order;					// determines FtS


	// first, extract the data from the premises

	// takes from variable x or y:
		// alias in the relation: x or y
		// value: 1 or 0

	var_name = premise_variable.getLinkedSet()->getSetName();
	var_value = premise_variable.getVariableValue();


	// takes from relation x_y:
		// type: A, V, L, N, etc.
		// direction: x --> y (1-to-2 or 2-to-1 format)

	rel_name_1 = premise_relation.getEndpointA()->getSetName();
	rel_name_2 = premise_relation.getEndpointB()->getSetName();
	rel_type = premise_relation.getRelationType();
	rel_order = premise_relation.hasLtROrder();

	/// add an error handler for a mistyped query



	// now, determine the pattern order (First-to-Second or vice versa)
	// as well as the result's linked_set

	// LtR relation order
	if(rel_order == 1)
	{
		// variable is in relation's first position
		if(var_name == rel_name_1)
		{
			// First-to-Second
			pat_order = 1;
			// links result's set to Second
			res_linked_set = premise_relation.getEndpointB();
		}

		// variable is in relation's second position
		else if(var_name == rel_name_2)
		{
			// Second-to-First
			pat_order = 0;
			// links result's set to First
			res_linked_set = premise_relation.getEndpointA();
		}
	}

	// RtL relation order
	else if(rel_order == 0)
	{
		// variable is in relation's first position
		if(var_name == rel_name_1)
		{
			// Second-to-First
			pat_order = 0;
			// links result's set to Second
			res_linked_set = premise_relation.getEndpointB();
		}

		// variable is in relation's second position
		else if(var_name == rel_name_2)
		{
			// First-to-Second
			pat_order = 1;
			// links result's set to First
			res_linked_set = premise_relation.getEndpointA();
		}
	}



	// exception: if the given variable is uncertain, what do you expect to deduce?
	if(var_value == 17)
    {
        // so, we just return the variable we were gonna return, except its value is locked at 17
        result_variable = new Variable(res_linked_set, 17);

    }


	// otherwise, apply the pattern
    else
    {
        // directly, straight up invoke the proper pattern, which has:
            // relation’s: type
            // pattern's: FtS order
            // variable’s: value
		res_value = Pattern[rel_type][pat_order][var_value];

        // build the variable
        result_variable = new Variable(res_linked_set, res_value);
    }


	// add the variable to the WorkingMemory
	WorkingMemory.WMS_Add_Variable(result_variable);

	// returns it
	return result_variable;
}



// recursively call boolean_solve() to use the chain from pathfind()
    // VTD has to not be in the chain
Variable* deduce(Chain* chain_to_use, int current_chain_pos) {

    Variable* next_var;     /// optionally, you can add the variable to the WorkingMemory, at some point
    Relation* next_rel;

    Variable* variable_to_return;

    bool found_variable_in_memory = 0;

    // first check if the variable is known in working memory
    for(int wms_v_c = 0; wms_v_c < WorkingMemory.getVariablesCount(); wms_v_c++)
    {
        // if a variable with the same name as the set is found in the working memory:
        if(WorkingMemory.getVariableAt(wms_v_c)->getLinkedSet()->getSetName() == chain_to_use->getSetAt(current_chain_pos)->getSetName())
        {
            found_variable_in_memory = 1;
            next_var = WorkingMemory.getVariableAt(wms_v_c);
            next_rel = chain_to_use->getRelationAt(current_chain_pos);

            std::cout << "in deduce() - " << current_chain_pos << "/" << chain_to_use->getChainSize()-1 << ":" << std::endl;
            std::cout << "found variable: " << WorkingMemory.getVariableAt(wms_v_c)->getLinkedSet()->getSetName() << " as " << WorkingMemory.getVariableAt(wms_v_c)->getVariableValue() << std::endl;
        }
    }

    // start determining the variable, since it hadn't been found in the working memory...
    // to prevent crashes (won't access more items than the chain has):
    if(current_chain_pos < chain_to_use->getChainSize()-1 && found_variable_in_memory == 0)
    {
        // deduce the next variable
        next_var = deduce(chain_to_use, current_chain_pos+1);
        next_rel = chain_to_use->getRelationAt(current_chain_pos);

        std::cout << "in deduce() - " << current_chain_pos << "/" << chain_to_use->getChainSize()-1 << ":" << std::endl;
        std::cout << "next variable to compute: " << next_rel->getEndpointOppositeOf(next_var->getLinkedSet())->getSetName() << std::endl;
    }

    // solve whatever result there is
        /// NAMES OF RESULTED SETS ARE RETURNED BY booleanSolve(), INCLUDING VTD
    variable_to_return = booleanSolve(*next_var, *next_rel);

    // add the newly deduced variable to working memory, so you dont have to deduce it again
    WorkingMemory.WMS_Add_Variable(variable_to_return);

    std::cout << "in deduce() - " << current_chain_pos << "/" << chain_to_use->getChainSize()-1 << ":" << std::endl;
    std::cout << "given the relation between: " << next_var->getLinkedSet()->getSetName() << " and " << next_rel->getEndpointOppositeOf(next_var->getLinkedSet())->getSetName() << std::endl;
    std::cout << "computed: " << variable_to_return->getLinkedSet()->getSetName() << " as " << variable_to_return->getVariableValue() << std::endl;
    std::cout << "---------" << std::endl;



    // return
    return variable_to_return;
}



// performs BFS from set_to_determine towards known_set
    // ... in order to return a chain of sets to go through to reach known_set
    // VTD has to not be in the chain
Chain* pathfind(Set* set_to_determine, Set* known_set) {

    Chain* final_chain;

    set_to_determine->searchMarkAs(1);

    // make a queue for storing the chains
    std::queue<Chain*> search_chain_queue;

    bool known_set_found = 0;



    /// INITIAL CHECK/ADD LOOP

    // make chains for all the stuff in set_to_determine's relations & add them to the queue
    for(int start_rel_count = 0; start_rel_count < set_to_determine->getRelationsCount(); start_rel_count++)
    {
        Chain* temp_chain = new Chain();

        // sole components of the chain are the next variable and its relation with the VTD, WHICH IS NOT IN THE CHAIN
        temp_chain->addChainComponent(set_to_determine->getRelationAt(start_rel_count)->getEndpointB(), set_to_determine->getRelationAt(start_rel_count));

        // before adding them to the queue, check if they are actually the known_set
        if(set_to_determine->getRelationAt(start_rel_count)->getEndpointB()->getSetName() == known_set->getSetName())
        {
            return temp_chain;
        }

        // just add the chain to the query
        search_chain_queue.push(temp_chain);
    }



    /// CHAIN QUEUE CHECK LOOP

    // check whatever chain is in the queue until you find the set
    while(search_chain_queue.empty() == 0 && known_set_found == 0)
    {
        // copy of this set's chain so far
        Chain* node_chain = new Chain();
        *node_chain = *(search_chain_queue.front());		        // straight up copy the chain at the front
        search_chain_queue.pop();			    			    // dequeue the chain after we take it



        // the set whose relations we need to check,
            // found at the last position of the chain we're working with
        Set* this_set = node_chain->getSetAt(node_chain->getChainSize()-1);

        // mark the set as checked
        this_set->searchMarkAs(1);

        ///std::cout << "the size of this chain: " << node_chain->getChainSize() << std::endl;
        ///std::cout << "the first set of this chain: " << node_chain->getSetAt(0)->getSetName() << std::endl;
        ///std::cout << "--- checking for " << this_set->getSetName() << std::endl;



        /// SET CHECK/ADD LOOP

        // loop through this node's relations_vector
        for(int neighbor_cnt = 0; neighbor_cnt < this_set->getRelationsCount(); neighbor_cnt++)
        {
            Set* set_to_check = this_set->getRelationAt(neighbor_cnt)->getEndpointB();
            Relation* relation_to_check = this_set->getRelationAt(neighbor_cnt);

            ///std::cout << "------ neighbour " << set_to_check->getSetName() << std::endl;

            // if set_to_check has NOT been visited before,
            if(set_to_check->wasSearched() == 0)
            {
                // make a copy of the current chain
                Chain* new_chain = new Chain();
                *new_chain = *node_chain;

                // ...and add the "relation's other end" set to its last end, as well as the relation for it
                new_chain->addChainComponent(set_to_check, relation_to_check);

                ///std::cout << "copied the chain with the following elements:" << std::endl;

                /*for(int hj=0; hj < new_chain->getChainSize(); hj++)
                {
                    std::cout << new_chain->getSetAt(hj)->getSetName() << std::endl;
                }*/

                // if a "relation's other end" set has the name we're looking for
                if(set_to_check->getSetName() == known_set->getSetName())
                {
                    known_set_found = 1;
                    final_chain = new_chain;
                }

                ///std::cout << "adding " << set_to_check->getSetName() << std::endl;

                // if not, just keep looking
                // place the added-upon chain, with the new set, back in the queue
                search_chain_queue.push(new_chain);
            }
        }
    }

    /// error handler for not building a chain:
        /// std::cout << "insufficient data to establish relation between " << set_to_determine << " and " << known_set << std::endl;

    // and, after all the looping and queueing, you have the chain (final_chain)

    // go through all registered chains, including final_chain, and unmark all sets as visited, from the last node in the chain to the first
    /// this is currently patched by WMS_Clear_Marked_Sets() inside interpretQuery()

    // delete all other chains from memory

    // return the final chain
    return final_chain;
}





//
// Query handling function
//

// query example:
	// given - set A - set B - rel A B L 1 - var A 1 nevig determine B
void interpretQuery(std::vector<std::string> data_text) {

	int string_looped_counter = 0;

	// if there are still words to interpret in the query
	while(string_looped_counter != data_text.size())
	{
		// when data is to be given
		if(data_text[string_looped_counter] == "given")
		{
			// advance to the next word
			string_looped_counter++;

			// loop through the given until you find its end
			while(data_text[string_looped_counter] != "nevig")
			{
				// when something new is declared
				if(data_text[string_looped_counter] == "-")
				{
					// it's a set!
					if(data_text[string_looped_counter+1] == "set")
					{
						// make it an object
						Set* temp_set = new Set(data_text[string_looped_counter+2]);

						// to add relations faster, cache pointers to the sets you've
						// just added inside the working memory
						WorkingMemory.WMS_Add_Set(temp_set);
					}

					// it's a variable!
					if(data_text[string_looped_counter+1] == "var")
					{
						// variables are supposed to be linked to sets you've defined previously and which have been given values

						// integer alias for the string
						int variable_val = StringToInteger(data_text[string_looped_counter+3]);
						/// dunno if straight-up using StringToInteger is a good idea
						/// boolean alias for the integer
						///bool variable_val = aux_variable_val;

						// make a set with the same name
						Set* temp_set_var = new Set(data_text[string_looped_counter+2]);

						// find the set in the memory
                        Set* set_var_pointer = WorkingMemory.WMS_Find_Set(temp_set_var);

						// make it an object
						Variable* temp_var = new Variable(set_var_pointer, variable_val);

						// add the defined variable to the WorkingMemory
						WorkingMemory.WMS_Add_Variable(temp_var);
					}

					// it's a relation!
					if(data_text[string_looped_counter+1] == "rel")
					{
						// things are slightly more complicated with this
						// first, get pointers to its sets
						// you can find the pointers by looking backward from the last element
						// inside the set cache in the working memory

						// make sets with the same name
						Set* temp_set_A = new Set(data_text[string_looped_counter+2]);
						Set* temp_set_B = new Set(data_text[string_looped_counter+3]);

						// and other variables
						int rel_type = StringToInteger(data_text[string_looped_counter+4]);
						bool rel_order_LtR = StringToInteger(data_text[string_looped_counter+5]);
						/// dunno if straight-up using StringToInteger is a good idea

						// error checkers
						bool set_A_pointer_error_checker = 0;
						bool set_B_pointer_error_checker = 0;

						// get the pointers to the legit ones from WorkingMemory
						Set* set_A_pointer = WorkingMemory.WMS_Find_Set(temp_set_A);
						Set* set_B_pointer = WorkingMemory.WMS_Find_Set(temp_set_B);

						// adds the relation to BOTH ends with one call
						set_A_pointer->addRelation(set_B_pointer, rel_type, rel_order_LtR);
					}
				}

				// read the next thing
				string_looped_counter++;
			}

			/// won't clear the memory until you give it a determine query
		}

		// when something is to be determined
		else if(data_text[string_looped_counter] == "determine")
		{
			// if you wanna be picky with it,
			// you can make a queue of actions that are taken after the query
			// is read through, and call deduce() solely after that
			// though i will simplify it here and assume you're gonna call it at the end of everything that
			// was given, you know, like somebody who isn't braindead would do...

			// before calling deduce(), we need to have (at least) one chain.
			// im not making a vector of them though, ill just assume we can make do with a single one
			Chain* chain_to_use;


            // error checker
            bool set_to_determine_pointer_error_checker = 0;

			// looks for the pointer to the set_to_find inside your memory
			Set* set_to_find = new Set(data_text[string_looped_counter+1]);
			Set* set_to_determine_pointer = WorkingMemory.WMS_Find_Set(set_to_find);

			// this loop can potentially generate MULTIPLE GOOD CHAINS, but we're suppressing it to only make one
			for(int var_loop_counter = 0; var_loop_counter < WorkingMemory.getVariablesCount(); var_loop_counter++)
			{
				// obtain a pathfinding chain
				Chain* chain_buffer;
				chain_buffer = pathfind(set_to_determine_pointer, WorkingMemory.getVariableAt(var_loop_counter)->getLinkedSet());

				// if the chain is alright
				/// originally was meant to be "chain_buffer != NULL", depending on what pathfind can return when the result is bad
				if(chain_buffer->getChainSize() != 0)
				{
					chain_to_use = chain_buffer;

					break;
				}
			}

			// we're searching with BFS, so the known_set, as well as the known_set's relation to the next set,
			// are registered LAST in the chain
			int current_chain_pos = 0;

			// naturally the last thing this function will return will be the value of the var_to_determine (which is not supposed to be in the chain!)
			Variable* query_result;
            query_result = deduce(chain_to_use, current_chain_pos);

			// "return" var_to_be_returned
			std::cout << query_result->getLinkedSet()->getSetName() << " " << query_result->getVariableValue() << std::endl;

			/// clear the memory after you are done with the argument
            WorkingMemory.WMS_Clear_Marked_Sets();
			WorkingMemory.WMS_Clear_Variables_Vector();
		}

		// Template:
		// when ...
		// else if(data_text[string_looped_counter] == "...")

		/// mistyped query error handler
		/// ... that should be much more complex than this, and cover syntax as well!!
		else
        {
            //std::cout << "I cannot understand your query." << std::endl;
        }

		// read the next thing
		string_looped_counter++;
	}
}




