
#include "net.hh"
  
#include <iostream>

//--------------------------------------------------------------------------------
net::net(){

  //Seed the random number generator
  srand(time(NULL));

  // Initialize the loss function with the standard loss
  this->loss = StandardLoss;

  // Initialize the dirivative for the standard loss function
  this->loss_derivative = StandardLossDerivative;
  
}

//--------------------------------------------------------------------------------
net::net(double (*custom_loss)(double, double), double (*custom_loss_derivative)(double, double)){

  //Seed the random number generator
  srand(time(NULL));
  
  // Initialize a custom loss function
  this->loss = custom_loss;

  // Initialize its derivative
  this->loss_derivative = custom_loss_derivative;
  
}

//--------------------------------------------------------------------------------
net::~net(){

  // Delete all te nodes
  for (int i = 0; i < this->nodes.size(); ++i){
    this->nodes[i].clear();
  }

  // Delete all the synapses
  for (int i = 0; i < this->synapses.size(); ++i){
    for (int s = 0; s < this->synapses[i].size(); ++s){
      // The weights must be deleted separately
      delete this->synapses[i][s]->weight;
      delete this->synapses[i][s]->weight_delta;
    }
    this->synapses[i].clear();
  }

}

//--------------------------------------------------------------------------------
void net::AddNode(const unsigned int layer){

  // Ensure there are enough layers to include this node, add a layer if necessary
  if (layer >= this->nodes.size()){
    this->nodes.resize(layer+1);
  }

  // Give the node a name. the "P" at the beginning means passive
  std::stringstream s;
  s.str("");
  s << "PL" << layer << "N" << this->nodes[layer].size();

  // Add the node
  this->nodes[layer].push_back(new node(s.str()));
  
}

//--------------------------------------------------------------------------------
void net::AddNode(const unsigned int layer, double (*act_func)(double), double (*act_deriv)(double)){

  // Ensure there are enough layers to include this node, add a layer if necessary
  if (layer >= this->nodes.size()){
    this->nodes.resize(layer+1);
  }

  // Give the node a name. the "A" at the beginning means active
  std::stringstream s;
  s.str("");
  s << "AL" << layer << "N" << this->nodes[layer].size();

  // Add the node
  this->nodes[layer].push_back(new node(s.str(), act_func, act_deriv));
  
}

//--------------------------------------------------------------------------------
void net::AddNodes(const unsigned int layer, const unsigned int number, double (*act_func)(double), double (*act_deriv)(double)){

  // Add the requested number of nodes
  for (int i = 0; i < number; ++i){
    this->AddNode(layer, act_func, act_deriv);
  }
  
}

//--------------------------------------------------------------------------------
void net::AddNodes(const unsigned int layer, const unsigned int number){

  // Add the requested number of nodes
  for (int i = 0; i < number; ++i){
    this->AddNode(layer);
  }
  
}

//--------------------------------------------------------------------------------
void net::AddSynapse(const unsigned int step, node* source, node* sink){

  // Ensure there are enough steps to include this synapse, add a step if necessary
  if (step + 1 >= this->synapses.size()){
    this->synapses.resize(step+1);
  }

  // Add the synapse to its step
  this->synapses[step].push_back(new synapse);

  // Initialize the synapse with its source node, sink node, and weight parameters
  this->synapses[step][this->synapses[step].size()-1]->source_node = source;
  this->synapses[step][this->synapses[step].size()-1]->sink_node = sink;
  this->synapses[step][this->synapses[step].size()-1]->weight = new double;
  this->synapses[step][this->synapses[step].size()-1]->weight_delta = new double;

  // Initialize a random weight, and a zero delta
  *this->synapses[step][this->synapses[step].size()-1]->weight = -1.0 + (rand() % 10000 + 0.5)/5000.0;
  *this->synapses[step][this->synapses[step].size()-1]->weight_delta = 0;

  // Inform the source and sink nodes of this synapse
  source->sink_synapses.push_back(this->synapses[step][this->synapses[step].size()-1]);
  sink->source_synapses.push_back(this->synapses[step][this->synapses[step].size()-1]);
  
}

//--------------------------------------------------------------------------------
void net::AddSynapses(const unsigned int step, std::vector<node*> sources, std::vector<node*> sinks){

  // Simply add each synapse individually
  for (int i = 0; i < sources.size(); ++i){
    for (int s = 0; s < sinks.size(); ++s){
      AddSynapse(step, sources[i], sinks[s]);
    }
  }
  
}

//--------------------------------------------------------------------------------
void net::Input(const std::vector<double> input_values){

  // Checks that you have given the righ tnumber of inputs
  if (input_values.size() != this->nodes[0].size()){throw 1;}

  // Write the input value to each input node
  for (int i = 0; i < this->nodes[0].size(); ++i){
    *(this->nodes[0][i]->input_signal) = input_values[i];
  }

}

//--------------------------------------------------------------------------------
std::vector<double> net::Output(){

  // Create a vector to hold the output values
  std::vector<double> output_signals(this->nodes[this->nodes.size() - 1].size());

  // collect the output signals from each output node
  for (int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
    output_signals[i] = *(this->nodes[this->nodes.size() - 1][i]->output_signal);
  }

  return output_signals;
  
}

//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
void net::ClearInputs(){

  // Write 0 to the input of each node
  for (int i = 0; i < this->nodes.size(); ++i){    
    for (int n = 0; n < this->nodes[i].size(); ++n){
      *(this->nodes[i][n]->input_signal) = 0;
    }
  }

}

//--------------------------------------------------------------------------------
double net::StandardLoss(double true_value, double predicted_value){

  // Squared difference
  return pow(true_value - predicted_value, 2);
  
}
  
//--------------------------------------------------------------------------------
double net::StandardLossDerivative(double true_value, double predicted_value){

  // Just the derivative
  return 2*(predicted_value - true_value);
  
}

//--------------------------------------------------------------------------------
double net::TotalLoss(const std::vector<double> true_values){

  // Check for valid vector
  if (this->nodes[this->nodes.size() - 1].size() != true_values.size()){throw 1;}

  // Sum the loss for each node
  double total = 0;
  for (int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
    total += this->loss(*this->nodes[this->nodes.size() - 1][i]->output_signal, true_values[i]);
  }

  return total;
}

//--------------------------------------------------------------------------------
std::string net::Save(){

  std::string data;
  std::stringstream ss;

  data += "nodes{\n";
  for (int i = 0; i < this->nodes.size(); ++i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      //      data += this->nodes[i][n]->name + "{\n";
      data += this->nodes[i][n]->Save();
      //      data += "}\n";
    }
  }
  data += "}\n";

  data += "synapses{\n";
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
  data += "}\n";

  return data;
  
}


//--------------------------------------------------------------------------------
int net::Load(std::string file_name, double (*act_func)(double), double (*act_deriv)(double)){

  std::string line, node_names;
  std::stringstream ss;
  std::ifstream f(file_name.c_str());
  // variables used to track the progress and store temporary data
  bool active, reading_nodes = false, reading_synapses = false, in_node = false, in_synapse = false;
  int layer, node_number, step = 0, step_in_synapse = 0;//jesse was here
  int source_layer, source_node_number, sink_layer, sink_node_number;
  double synapse_weight, synapse_weight_delta;

  // Clear the nodes so that it can refill everything
  for (int i = 0; i < this->nodes.size(); ++i ){
    this->nodes[i].clear();
  }
  // Clear the synapses
  for (int i = 0; i < this->synapses.size(); ++i){
    for (int s = 0; s < this->synapses[i].size(); ++s){
      delete this->synapses[i][s]->weight;
      delete this->synapses[i][s]->weight_delta;
    }
    this->synapses[i].clear();
  }
  this->nodes.clear();
  this->synapses.clear();

  if (f.is_open()){
    // loop through each line in the file
    while (getline(f, line)){
      // Begin reading in nodes
      if (!reading_nodes && !reading_synapses && line == "nodes{"){
	reading_nodes = true;
	// between nodes
      }else if (reading_nodes && !in_node){
	// if you close a brace while between nodes, clearly the node list has ended
	if (line == "}"){
	  reading_nodes = false;
	}else{
	  // Retrieve the name of the node, which tells you where it should go
	  ParseNodeName(line.substr(0,line.find("{")), &active, &layer, &node_number);
	  // check that the node will actually go in the right place
	  if (this->nodes[layer].size() == node_number){
	    // choose the type of node based on its name
	    if (active){
	      this->AddNode(layer, act_func, act_deriv);
	    }else{
	      this->AddNode(layer);
	    }
	  }else{
	    // freak out if it wont go in the right place
	    return 1;
	  }
	  in_node = true;
	  ss.str("");
	  // Start recording the node data
	  ss << line << "::";
	}
	// the bulk of reading in the nodes
      }else if(reading_nodes && in_node){
	// check for the end of the node
	if (line == "}"){
	  in_node = false;
	  ss << line << "::";
	  // when you've reached the end use the node's built in code for re-loading itself
	  this->nodes[layer][node_number]->Load(ss.str());
	  ss.str("");
	}else{
	  // Add each line that is part of the node data to this object, it will then be passed to the node
	  ss << line << "::";
	}
	// Start reading in the synapses
      }else if (!reading_nodes && !reading_synapses && line == "synapses{"){
	reading_synapses = true;
	// between synapses
      }else if (reading_synapses && !in_synapse){
	// The end of the synapses
	if (line == "}"){
	reading_synapses = false;
	// this orders the synapses into groups
	}else if (line.substr(0,4) == "step"){
	  Get(line, &step);
	}else{
	  // if you are now in a synapse, set th counter to 0
	  step_in_synapse = 0;
	  in_synapse = true;
	}
	// Read the info for a synapse
      }else if (reading_synapses && in_synapse){
	// check for the end of the synapse
	if (line == "}"){
	  in_synapse = false;
	  // Create the synapse with the data you collected
	  AddSynapse(step, this->nodes[source_layer][source_node_number], this->nodes[sink_layer][sink_node_number]);
	  // set its weight parameters
	  *this->synapses[step][this->synapses[step].size()-1]->weight = synapse_weight;
	  *this->synapses[step][this->synapses[step].size()-1]->weight_delta = synapse_weight_delta;
	}else{
	  // Read the appropriate info for the curent counter step
	  if (step_in_synapse == 0){
	    Get(line, &node_names);
	    ParseNodeName(node_names, &active, &source_layer, & source_node_number);
	  }else if (step_in_synapse == 1){
	    Get(line, &node_names);
	    ParseNodeName(node_names, &active, &sink_layer, & sink_node_number);
	  }else if (step_in_synapse == 2){
	    Get(line, &synapse_weight);
	  }else if (step_in_synapse == 3){
	    Get(line, &synapse_weight_delta);
	  }
	  step_in_synapse += 1;
	}
      }          
    }  
  }

  return 0;
}
