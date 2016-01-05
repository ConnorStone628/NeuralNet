
#include "net.hh"

net::net(){
  self_built = false;
}

net::~net(){
  if (self_built){
    for (int i = 0; i < nodes.size(); ++i){
      for (int n = 0; n < nodes[i].size(); ++n){
	delete nodes[i][n];
      }
    }
    for (int i = 0; i < synapses.size(); ++i){
      for (int a = 0; a < synapses[i].size(); ++a){
	delete synapses[i][a];
      }
    }    
  }
  
}

void net::BuildFullyConnectedNet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double)){

  self_built = true;

  nodes.resize(nodes_per_layer.size());
  synapses.resize(nodes_per_layer.size() - 1);
  
  for (int i = 0; i < nodes.size(); ++i){
    nodes[i].resize(nodes_per_layer[i]);

    // Add the nodes for each layer
    for (int n = 0; n < nodes_per_layer[i]; ++n){
      if (i == 0){
	// input node
	nodes[i][n] = new node();
      }else{
	// active node
	nodes[i][n] = new node(activation_function);

	// Add the synapses connecting to this node
	for (int a = 0; a < nodes_per_layer[i-1]; ++a){
	  synapses[i-1].push_back(new synapse(nodes[i-1][a], nodes[i][n]));
	}
      }
    }

  }
  
}

void net::BuildNet(std::vector< std::vector<node*> > layer_nodes, std::vector< std::vector<synapse*> > synapse_sequence){

  self_built = false;

  nodes.resize(layer_nodes.size());
  for (int i = 0; i < layer_nodes.size(); ++i){
    for (int n = 0; n < layer_nodes[i].size(); ++n){
      nodes[i][n].push_back(layer_nodes[i][n]);
    }
  }

  synapses.resize(synapse_sequence.size());
  for (int i = 0; i < synapse_sequence.size(); ++i){
    for (int a = 0; a < synapse_sequence[i].size(); ++a){
      synapses[i][a].push_back(synapse_sequence[i][a]);
    }
  }

}

void net::AddNode(unsigned int layer, node* new_node){

  self_built = false;

  if (layer >= nodes.size()){
    nodes.resize(layer);
  }

  nodes[layer].push_back(new_node);
}

void net::AddSynapse(unsigned int step, synapse* new_synapse){
  
  self_built = false;

  if (step >= synapses.size()){
    synapses.resize(step);
  }

  synapses[step].push_back(new_synapse);
}

std::vector<node*>* net::GetNodes(unsigned int layer){
  if (layer >= nodes.size()){throw 1;}

  return &(nodes[layer])
}

std::vector<synapse*>* net::GetSynapses(unsigned int step){
  if (step >= synapses.size()){throw 1;}

  return &(synapses[step])
}

void net::Input(std::vector<double> input_values){

  if (input_values.size() != nodes[0].size()){throw 1;}

  for (int i = 0; i < nodes[0].size(); ++i){
    *(nodes[0][i]->input_signal) = input_values[i];
  }

}

std::vector<double> net::Output(){

  std::vector<double> output_signals(nodes[nodes.size() - 1].size());

  for (int i = 0; i < nodes[nodes.size() - 1].size(); ++i){
    output_signals[i] = *(nodes[nodes.size() - 1][i]->output_signal);
  }

  return output_signals;
}

std::vector< std::vector<double> > net::Weights(){

  std::vector< std::vector<double> > return_weights;

  return_weights.resize(synapses.size());
  for (int i = 0; i < synapses.size(); ++i){
    return_weights[i].resize(synapses[i].size());
    
    for (int a = 0; a < synapses[i].size(); ++a){
      return_weights[i][a] = synapses[i][a]->weight;
    }
  }

  return return_weights;
  
}

void net::GeneralPropogate(){
  
  for (int i = 0; i < synapses.size(); ++i){    
    for (int a = 0; a < synapses[i].size(); ++a){      
      synapses[i][a]->Transmit(true);      
    }    
  }
  
}

void net::ForwardPropogate(){
  
  for (int i = 0; i < synapses.size(); ++i){    
    for (int a = 0; a < synapses[i].size(); ++a){
      synapses[i][a]->Transmit(false);      
    }
    for (int n = 0; n < nodes[i+1].size(); ++n){
      nodes[i+1][n]->Activate(true);
    }    
  }
  
}

void net::ClearInputs(){
  
  for (int i = 0; i < nodes.size(); ++i){    
    for (int n = 0; n < nodes[i].size(); ++n){
      *(nodes[i][n]->input_signal) = 0;
    }    
  }
}

double net::Loss(double true_value, double predicted_value){
  return pow(true_value - predicted_value, 2);
}
  
double net::LossDerivative(double true_value, double predicted_value){
  return 2*(predicted_value - true_value);
}

// Only to be used for fully connected nets
void net::BackPropogate(std::vector<double> true_values, double learning_rate, double (*activation_derivative)(double)){
  
  if (true_values.size() != nodes[nodes.size()-1].size()) {throw 1;}

  for (int i = 1; i < nodes.size(); ++i){
    for (int n = 0; n < nodes[i].size(); ++n){
      nodes[i][n]->Rate();
      if (i == nodes.size() - 1){nodes[i][n]->true_value = true_values[n];}
    }
  }
  
  for (int i = synapses.size() - 1; i >= 0; --i){
    for (int a = 0; a < synapses[i].size(); ++a){
      if (i == synapses.size() - 1){
	synapses[i][a]->delta = LossDerivative(synapses[i][a]->sink_node->true_value, *synapses[i][a]->sink_node->output_signal)*(*synapses[i][a]->sink_node->activation_rate);
      }else{
        synapses[i][a]->delta = 0;
	for (int s = 0; s < synapses[i+1].size(); ++s){
	  synapses[i][a]->delta += synapses[i+1][s]->delta*synapses[i+1][s]->weight;
	}
	synapses[i][a]->delta *= *synapses[i][a]->sink_node->activation_rate;
      }
      synapses[i][a].weight -= learning_rate*(*synapses[i][a]->source_node->output_signal)*synapses[i][a]->delta;      
    }
  }
  
}
