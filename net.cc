
#include "net.hh"

net::net(){}

net::~net(){}

void net::BuildNet(std::vector< std::vector<node*> > layer_nodes, std::vector< std::vector<synapse*> > synapse_sequence){

  this->nodes.resize(layer_nodes.size());
  for (int i = 0; i < layer_nodes.size(); ++i){
    for (int n = 0; n < layer_nodes[i].size(); ++n){
      this->nodes[i].push_back(layer_nodes[i][n]);
    }
  }

  this->synapses.resize(synapse_sequence.size());
  for (int i = 0; i < synapse_sequence.size(); ++i){
    for (int a = 0; a < synapse_sequence[i].size(); ++a){
      this->synapses[i].push_back(synapse_sequence[i][a]);
    }
  }

}

void net::AddNode(unsigned int layer, node* new_node){

  if (layer >= this->nodes.size()){
    this->nodes.resize(layer);
  }

  this->nodes[layer].push_back(new_node);
}

void net::AddSynapse(unsigned int step, synapse* new_synapse){
  
  if (step >= this->synapses.size()){
    this->synapses.resize(step);
  }

  this->synapses[step].push_back(new_synapse);
}

std::vector<node*>* net::GetNodes(unsigned int layer){
  if (layer >= this->nodes.size()){throw 1;}

  return &(this->nodes[layer])
}

std::vector<synapse*>* net::GetSynapses(unsigned int step){
  if (step >= this->synapses.size()){throw 1;}

  return &(this->synapses[step])
}

void net::Input(std::vector<double> input_values){

  if (input_values.size() != this->nodes[0].size()){throw 1;}

  for (int i = 0; i < this->nodes[0].size(); ++i){
    *(this->nodes[0][i]->input_signal) = input_values[i];
  }

}

std::vector<double> net::Output(){

  std::vector<double> output_signals(this->nodes[this->nodes.size() - 1].size());

  for (int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
    output_signals[i] = *(this->nodes[this->nodes.size() - 1][i]->output_signal);
  }

  return output_signals;
}

std::vector< std::vector<double> > net::Weights(){

  std::vector< std::vector<double> > return_weights;

  return_weights.resize(this->synapses.size());
  for (int i = 0; i < this->synapses.size(); ++i){
    return_weights[i].resize(this->synapses[i].size());
    
    for (int a = 0; a < this->synapses[i].size(); ++a){
      return_weights[i][a] = this->synapses[i][a]->weight;
    }
  }

  return return_weights;
  
}

void net::Propogate(){
  
  for (int i = 0; i < this->synapses.size(); ++i){    
    for (int a = 0; a < this->synapses[i].size(); ++a){      
      this->synapses[i][a]->Transmit(true);      
    }    
  }
  
}

void net::ClearInputs(){
  
  for (int i = 0; i < this->nodes.size(); ++i){    
    for (int n = 0; n < this->nodes[i].size(); ++n){
      *(this->nodes[i][n]->input_signal) = 0;
    }    
  }
  
}

double net::Loss(double true_value, double predicted_value){
  return pow(true_value - predicted_value, 2);
}
  
double net::LossDerivative(double true_value, double predicted_value){
  return 2*(predicted_value - true_value);
}

