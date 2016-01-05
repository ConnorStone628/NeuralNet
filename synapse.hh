
#include <vector>
#include "node.hh"

class synapse{
public:

  // The node with the source signal
  node* source_node;

  // The node which recieves the signal
  node* sink_node;

  // Weight to transform the signal
  double weight;

  // Most recent change in weight
  double delta;

  // Constructor for the synapse class. Transmits signals between nodes
  synapse(node* source, node* sink, double initial_weight = 0);

  // Destructor
  ~synapse();

  // Performs the action of moving the signal to the new node
  void Transmit(bool activate_node);
  
};
