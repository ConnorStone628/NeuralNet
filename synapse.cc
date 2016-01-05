
#include "synapse.hh"

synapse::synapse(node* source, node* sink, double initial_weight){

  source_node = source;

  sink_node = sink;

  weight = initial_weight;

  delta = 0;

}

synapse::~synapse(){}

void synapse::Transmit(bool activate_node){
  *(sink_node->input_signal) += weight*(*(source_node->output_signal));

  if (activate_node){
    sink_node->Activate(false);
  }
}
