#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include <iostream>
#include <deque>

//A specialized version of the 'heap' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class pairing_heap {
    public:
        typedef unsigned size_type;

        //Description: Construct a heap out of an iterator range 
        //              with an optional comparison functor.
        //Runtime: O(n) where n is number of elements in range.
        template<typename InputIterator>
        pairing_heap(InputIterator start, InputIterator end, 
                        COMP comp = COMP());

        //Description: Construct an empty heap with an optional 
        //              comparison functor.
        //Runtime: O(1)
        pairing_heap(COMP comp = COMP());

        //Description: Assumes that all elements inside the heap are 
        //              out of order and 'rebuilds' the heap by fixing 
        //              the heap invariant.
        //Runtime: O(n)
        virtual void update_priorities();

        //Description: Add a new element to the heap. This has been 
        //              provided for you, in that you should 
        //              implement push functionality in the add_node function.
        //Runtime: Amortized O(1)
        virtual void push(const TYPE& val) override { add_node(val); }

        //Description: Remove the most extreme (defined by 'compare') 
        //              element from the heap.
        //Runtime: Amortized O(log(n))
        virtual void pop() override;

        //Description: Return the most extreme (defined by 'compare') element of
        //             the heap.
        //Runtime: O(1)
        virtual const TYPE& top() const override;

        //Description: Get the number of elements in the heap.
        //Runtime: O(1)
        virtual size_type size() const { return sizeOf; }

        //Description: Return true if the heap is empty.
        //Runtime: O(1)
        virtual bool empty() const override { return !sizeOf;  }

        //Description: Copy constructor.
        //Runtime: O(n)
        pairing_heap(const pairing_heap& other);

        //Description: Copy assignment operator.
        //Runtime: O(n)
        pairing_heap& operator=(const pairing_heap& rhs);

        //Description: Destructor
        //Runtime: O(n)
        virtual ~pairing_heap();

        class Node {
            //This node class will allow external users to update the 
            //priority of
            //elements that are already inside the heap.
            public:
               
               //Node():elt(0), child(NULL), parent(NULL), next(NULL){}
                // Add any constructors and destructors you need here
			Node(const TYPE ielt):elt(ielt), child(NULL), parent(NULL), next(NULL){}
			
			
			
                //Description: Allows access to the element at that Node's 
                //position.
                //Runtime: O(1) - this has been provided for you.
                const TYPE &operator*() const { return elt; }

                //The following line allows you to access any private data 
                //members of this
                //Node class from within the pairing_heap class. 
                //(ie: myNode.elt is a legal
                //statement in pairing_heap's add_node() function).
                friend pairing_heap;

            private:
                TYPE elt;
                Node* child;
                Node* parent;
                Node* next;
                
                
                // Add any addtional member functions or data you require here
        };

        //Description: Add a new element to the heap. Returns a 
        //              Node* corresponding to the newly added element.
        //Runtime: Amortized O(1)
        Node* add_node(const TYPE& val);

        // Description: Updates the priority of an element already 
        //              in the heap by replacing the element referred 
        //              to the Node with new_value.
        //              Must maintain heap invariants.
        // PRECONDITION: The new priority, given by 'new_value' must 
        //                  be more extreme (as defined by comp)
        //                  than the old priority.
        //
        // Runtime: As discussed in reading material.
        void updateElt(Node * node, TYPE new_value);

    private:
    	int sizeOf;
     Node* root;
     //destroyer
     void destroyer(Node* root){
     	std::deque<Node*> tbd;
     	
     	if(root == NULL || sizeOf == 0) return;
     	tbd.push_back(root);
     	
     	while(!tbd.empty()){
     		
     		Node* next = tbd.back()->next;
     		Node* child = tbd.back()->child;
     		
     		tbd.back()->parent = NULL;
     		tbd.back()->child = NULL;
     		tbd.back()->next = NULL;
     		
     		delete tbd.back();
     		tbd.pop_back();
     	
     		if(next != NULL) tbd.push_back(next);
     		if(child != NULL) tbd.push_back(child);
     	}
     	
     }
     	
     //melds two heaps
     Node* meld(Node* &first, Node* &second){
     	if(first == NULL) return second;
     	if(second == NULL) return first;
     	
     	if(this->compare(first->elt, second->elt)){
     		first->parent = second;
     		first->next = second->child;
     		second->child = first;
     		return second;
     	}else{
     		second->parent = first;
     		second->next = first->child;
     		first->child = second;
     		return first;
     	}
     } 
     
     //Creates a deque of all Nodes below a
     void swapper(Node* a, std::deque<Node*> &total){

     	if(a->child != NULL) swapper(a->child, total);
     	if(a->next != NULL) swapper(a->next, total);
     	total.push_front(new Node(a->elt));	  
     }
          		
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
pairing_heap<TYPE, COMP>::pairing_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
)  {
    root = NULL;
    sizeOf = 0;
    this->compare = comp;
    while(start != end){
    	add_node(*start);
    	++start;
    }
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(COMP comp) {
    root = NULL;
    sizeOf = 0;
    this->compare = comp;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(const pairing_heap& other) {
        
        sizeOf = other.sizeOf;
        this->compare = other.compare;
        if(other.sizeOf == 0) return;
       
        std::deque<Node*> New;
	   
	   swapper(other.root, New);
	   while(New.size() != 1){
	    	Node* first = New.front();
	    	New.pop_front();
	    	Node* second = New.front();
	    	New.pop_front();
	    	first = meld(first, second);
	    	New.push_back(first);
        }
  	  Node* tot = New.front();
	   
	   root = tot;	 
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>&
pairing_heap<TYPE, COMP>::operator=(const pairing_heap& rhs) {
	 	
	 if(this == &rhs) return *this;
	 	
	   sizeOf = rhs.sizeOf;
	   this->compare = rhs.compare;
	   std::deque<Node*> New;
	   destroyer(root);
	  
	    if(rhs.sizeOf == 0){
	   	destroyer(root);
	   	root = NULL;
	   	return *this;
	   }
	   
	   
	   swapper(rhs.root, New);
	   
	   while(New.size() != 1){
	    	Node* first = New.front();
	    	New.pop_front();
	    	Node* second = New.front();
	    	New.pop_front();
	    	first = meld(first, second);
	    	New.push_back(first);
        }
    Node* tot = New.front();
	   
	   root = tot;	 
	   return *this;
	   
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::~pairing_heap() { destroyer(root); }


template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::update_priorities() {	    
	   if(sizeOf == 0) return;
	   std::deque<Node*> New;	   
	   
	   swapper(root, New);
	   destroyer(root);
	   
	   
	   while(New.size() != 1){
	    	Node* first = New.front();
	    	New.pop_front();
	    	Node* second = New.front();
	    	New.pop_front();
	    	first = meld(first, second);
	    	New.push_back(first);
        }
    Node* tot = New.front();
	   
	   root = tot;	 
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::pop() {
    if(sizeOf == 0) return;
    
    if(root->child == NULL){
	--sizeOf;
	delete root;
	root = NULL;
     return;
    }
    
    std::deque<Node*> children;
    
    Node* child = root->child;
    delete root;
    children.push_front(child);
    Node* next = child->next;
    while(next != NULL){
    	children.push_front(next);
    	next = next->next;
    }
    
    
    
    while(children.size() != 1){
    	Node* first = children.front();
    	children.pop_front();
    	Node* second = children.front();
    	children.pop_front();
    	first = meld(first, second);
    	children.push_back(first);
    }
    Node* out = children.front();
    --sizeOf;
    out->parent = NULL;
    out-> next = NULL;
    root = out;
}

template<typename TYPE, typename COMP>
const TYPE& pairing_heap<TYPE, COMP>::top() const { return root->elt; }


template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node* 
pairing_heap<TYPE, COMP>::add_node(const TYPE& val) {
 	
    Node* input = new Node(val);
    if(root == NULL) root = input;
    else root = meld(root, input);
    sizeOf++;
    return input;
    
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::updateElt(Node* node, TYPE new_value) {
   
    if(node == NULL) return;    
    
    node->elt = new_value;
    
    if(node->parent == NULL) return;
    if(this->compare(new_value, node->parent->elt)) return;
    
    Node* Parent = node->parent;
    Node* cNode = node;
    while(this->compare(Parent->elt, cNode->elt)){
    	TYPE hold_val = Parent->elt;
    	Parent->elt = cNode->elt;
    	
    	cNode->elt = hold_val;
    	cNode = Parent;
    	Parent = Parent->parent;
    	if(Parent == NULL) break;
   }

}

#endif //PAIRING_HEAP_H

