
#include "synapse.hh"

synapse::synapse(node* source, node* sink){

  this->source_node = source;

  this->source_node->output_synapses.push_back(this);

  this->sink_node = sink;

  this->sink_node->input_synapses.push_back(this);

  this->weight = 0;

}

synapse::~synapse(){}

void synapse::Transmit(bool activate_node){
  *(this->sink_node->input_signal) += this->weight*(*(this->source_node->output_signal));

  if (activate_node){
    this->sink_node->Activate(false);
  }
}
