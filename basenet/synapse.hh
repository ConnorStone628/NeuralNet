#ifndef __SYNAPSE__
#define __SYNAPSE__

#include "node.hh"

// Pre-defines the node class so that pointers can be used
class node;

struct synapse{

  // Node which sends the signal
  node* source_node;

  // Node which recieves the signal
  node* sink_node;

  // Scaling factor for the signal
  double* weight;
  
  // Holds the most recent change in the weight
  double* weight_delta;
  
};

#endif
