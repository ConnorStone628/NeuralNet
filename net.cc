
#include "net.hh"
#include <stdlib.h>
#include <time.h>

#include <iostream>

net::net(){

  // Initialize the loss function with the standard loss
  this->loss = StandardLoss;

  // Initialize the dirivative for the standard loss function
  this->loss_derivative = StandardLossDerivative;
  
}

net::net(double (*custom_loss)(double, double), double (*custom_loss_derivative)(double, double)){

  // Initialize a custom loss function
  this->loss = custom_loss;

  // Initialize its derivative
  this->loss_derivative = custom_loss_derivative;
  
}

net::~net(){

  for (int i = 0; i < this->synapses.size(); ++i){
    for (int s = 0; s < this->synapses[i].size(); ++s){
      delete this->synapses[i][s]->weight;
      delete this->synapses[i][s]->weight_delta;
      delete this->synapses[i][s];
    }
  }

}

void net::BuildNet(std::vector< std::vector<node*> > layer_nodes){

  // Add the nodes given by the user
  this->nodes.resize(layer_nodes.size());
  for (int i = 0; i < layer_nodes.size(); ++i){
    for (int n = 0; n < layer_nodes[i].size(); ++n){
      this->nodes[i].push_back(layer_nodes[i][n]);
    }
  }

}

void net::AddNode(unsigned int layer, double (*act_func)(double), double (*act_deriv)(double)){

  // Ensure there are enough layers to include this node, add a layer if necessary
  if (layer >= this->nodes.size()){
    this->nodes.resize(layer+1);
  }

  // Give the node a name. the "A" at the beginning means active
  std::stringstream s;
  s.str("");
  s << "AL" << layer << "N";
  if (this->nodes[layer].empty()){
    s << "0";
  }else{
    s << this->nodes[layer].size();
  }

  // Add the node
  this->nodes[layer].push_back(new node(s.str(), act_func, act_deriv));
  
}

void net::AddNode(unsigned int layer){

  // Ensure there are enough layers to include this node, add a layer if necessary
  if (layer >= this->nodes.size()){
    this->nodes.resize(layer+1);
  }

  // Give the node a name. the "P" at the beginning means passive
  std::stringstream s;
  s.str("");
  s << "PL" << layer << "N";
  if (this->nodes[layer].empty()){
    s << "0";
  }else{
    s << this->nodes[layer].size();
  }
  // Add the node
  this->nodes[layer].push_back(new node(s.str()));
  
}

void net::AddNodes(unsigned int layer, unsigned int number, double (*act_func)(double), double (*act_deriv)(double)){

  // Add the requested number of nodes
  for (int i = 0; i < number; ++i){
    this->AddNode(layer, act_func, act_deriv);
  }
  
}

void net::AddNodes(unsigned int layer, unsigned int number){

  // Add the requested number of nodes
  for (int i = 0; i < number; ++i){
    this->AddNode(layer);
  }
  
}

void net::AddSynapse(unsigned int step, node* source, node* sink){

  // Ensure there are enough steps to include this synapse, add a step if necessary
  if (step + 1 >= this->synapses.size()){
    this->synapses.resize(step+1);
  }

  // Add the synapse to its step
  this->synapses[step].push_back(new synapse);
  unsigned int newest_synapse = this->synapses[step].size()-1;

  // Initialize the synapse with its source node, sink node, and weight parameters
  this->synapses[step][newest_synapse]->source_node = source;
  this->synapses[step][newest_synapse]->sink_node = sink;
  this->synapses[step][newest_synapse]->weight = new double;
  this->synapses[step][newest_synapse]->weight_delta = new double;

  // Initialize a random weight, and a zero delta
  *this->synapses[step][newest_synapse]->weight = -1.0 + (rand() % 10000 + 0.5)/5000.0;
  *this->synapses[step][newest_synapse]->weight_delta = 0;

  // Inform the source and sink nodes of this synapse
  source->sink_synapses.push_back(this->synapses[step][newest_synapse]);
  sink->source_synapses.push_back(this->synapses[step][newest_synapse]);
  
}

void net::AddSynapses(unsigned int step, node* source, std::vector<node*> sinks){

  // Simply add each synapse individually
  for (int i = 0; i < sinks.size(); ++i){
    AddSynapse(step, source, sinks[i]);
  }
  
}

std::vector<node*>* net::GetNodes(unsigned int layer){

  // Check for valid layer
  if (layer >= this->nodes.size()){throw 1;}

  // Return a pointer to the vector of nodes
  return &(this->nodes[layer]);
    
}

void net::Input(std::vector<double> input_values){

  // Checks that you have given the righ tnumber of inputs
  if (input_values.size() != this->nodes[0].size()){throw 1;}

  // Write the input value to each input node
  for (int i = 0; i < this->nodes[0].size(); ++i){
    *(this->nodes[0][i]->input_signal) = input_values[i];
  }

}

std::vector<double> net::Output(){

  // Create a vector to hold the output values
  std::vector<double> output_signals(this->nodes[this->nodes.size() - 1].size());

  // collect the output signals from each output node
  for (int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
    output_signals[i] = *(this->nodes[this->nodes.size() - 1][i]->output_signal);
  }

  return output_signals;
  
}

std::vector< std::vector<double> > net::Weights(){

  // Create a vector to hold the weights
  std::vector< std::vector<double> > return_weights;

  return_weights.resize(this->synapses.size());
  // Loop through each layer of nodes
  for (int i = 0; i < this->synapses.size(); ++i){
    // Loop through each node in the layer
    for (int a = 0; a < this->synapses[i].size(); ++a){
      return_weights[i].push_back(*this->synapses[i][a]->weight);
    }
  }

  return return_weights;
  
}

std::vector< std::vector<double> > net::Activity(){

  // Create vector to hold the activity values on each node
  std::vector< std::vector<double> > return_values;

  return_values.resize(this->nodes.size());
  // loop through each layer
  for (int i = 0; i < this->nodes.size(); ++i){
    return_values[i].resize(this->nodes[i].size());
    // loop through each node
    for (int a = 0; a < this->nodes[i].size(); ++a){
      // Colllect the output signal from that node
      return_values[i][a] = *this->nodes[i][a]->output_signal;
    }
  }

  return return_values;
  
}

void net::Propogate(){

  // Fire each node to send its signal through its synapses
  for (int i = 0; i < this->nodes.size(); ++i){    
    for (int n = 0; n < this->nodes[i].size(); ++n){
      // Apply the activation function
      this->nodes[i][n]->Activate();
      // Send the signal to its output nodes
      this->nodes[i][n]->Fire();
    }
  }

}

void net::ClearInputs(){

  // Write 0 to the input of each node
  for (int i = 0; i < this->nodes.size(); ++i){    
    for (int n = 0; n < this->nodes[i].size(); ++n){
      *(this->nodes[i][n]->input_signal) = 0;
    }
  }

}

double net::StandardLoss(double true_value, double predicted_value){

  // Squared difference
  return pow(true_value - predicted_value, 2);
  
}
  
double net::StandardLossDerivative(double true_value, double predicted_value){

  // Just the derivative
  return 2*(predicted_value - true_value);
  
}

double net::TotalLoss(std::vector<double> true_values){

  // Check for valid vector
  if (this->nodes[this->nodes.size() - 1].size() != true_values.size()){throw 1;}

  // Sum the loss for each node
  double total = 0;
  for (int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
    total += this->loss(*this->nodes[this->nodes.size() - 1][i]->output_signal, true_values[i]);
  }

  return total;
}

std::string net::Save(){

  std::string data;
  std::stringstream s;

  data += "nodes\n";
  for (int i = 0; i < this->nodes.size(); ++i){
    data += Convert("layer",i);
    for (int n = 0; n < this->nodes[i].size(); ++n){
      data += this->nodes[i][n]->name + "{\n";
      data += this->nodes[i][n]->Save();
      data += "}\n";
    }
  }

  data += "synapses\n";
  for (int i = 0; i < this->synapses.size(); ++i){
    data += Convert("step",i);
    for (int s = 0; s < this->synapses[i].size(); ++s){
      data += "SYNAPSE{\n";
      data += Convert("source_node", this->synapses[i][s]->source_node->name);
      data += Convert("sink_node", this->synapses[i][s]->sink_node->name);
      data += Convert("weight", *this->synapses[i][s]->weight);
      data += Convert("weight_delta", *this->synapses[i][s]->weight_delta);
      data += "}\n";
    }
  }

  return data;
  
}
