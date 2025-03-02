This project was an attempt to study the creation and inner workings of an expert system (an older type of AI architecture) that records an ontology of categories of objects and the relations between such objects. The knowledge base ontology is a graph that is populated with such data, and it is then possible to query the system for information regarding said data, which is done by traversing the graph and computing relations along the way (for instance, if told that "bird" is a species of the category "animal", and that "sparrow" is a species of "bird", the system's inference engine will be able to deduce that "sparrow" is a species of "animal"). The codebase uses many data structures and implements several language features of intermediate-level C++.

- the program holds information that is given to it in a set-relation format, making an ontology. The relationship patterns are, still, configurable -- this was a research project and I wanted to understand what was meant by medieval syllogistic formulations (see https://en.wikipedia.org/wiki/Syllogism#Examples).

- the memory can be edited directly in the file, or can be altered by memorizing information directly from the program.

- the program also remembers certain sets as having a true/false value w.r.t. another set belonging to them. This is used in the actual computation of the results, and is simply a quirky way of determining the truthfulness/falsehood of the premises.

- "solve" command query example:
	`given - set A - set B - rel A B L 1 - var A 1 nevig determine B`
	The "nevig" word is "given" written backwards, and is used to indicate the end of the given premises in the argument. The magic of the program is that it can hold background premises in its memory, and will integrate them with the premises given by the user, in order to determine whether the result is true or false. Answers that cannot be determined will result in a 17 value, meaning "undeterminable."
