
#include "synapse.hh"

#include <iostream>

synapse::synapse(node* source, node* sink){

  this->source_node = source;

  // Include self in the nodes list of synapses
  this->source_node->output_synapses.push_back(this);

  this->sink_node = sink;

  // Include self in the nodes list of synapses
  this->sink_node->input_synapses.push_back(this);

  // Initialize random weight
  this->weight = new double;
  *this->weight = -1.0 + (rand() % 10000 + 0.5)/5000.0;

}

synapse::~synapse(){}

void synapse::Transmit(){

  // push signal through this synapse, with the weight applied
  *this->sink_node->input_signal += (*this->weight)*(*(this->source_node->output_signal));
  
}
