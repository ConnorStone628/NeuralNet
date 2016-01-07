#ifndef __SYNAPSE__
#define __SYNAPSE__

#include <vector>
#include "node.hh"

class synapse{
public:

  // The node with the source signal
  node* source_node;

  // The node which recieves the signal
  node* sink_node;

  // Weight to transform the signal
  double* weight;

  // Used to hold extra constants used by specialized nets
  std::vector<double> extra_params;

  // Constructor for the synapse class. Transmits signals between nodes
  synapse(node* source, node* sink);

  // Destructor
  ~synapse();

  // Performs the action of moving the signal to the new node
  void Transmit(bool activate_node);

};

#endif
