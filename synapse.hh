#ifndef __SYNAPSE__
#define __SYNAPSE__

#include "node.hh"

class node;

struct synapse{

  node* source_node;

  node* sink_node;

  double* weight;

  double* weight_delta;
  
};

#endif
