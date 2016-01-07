
#include "fullyconnectednet.hh"

fullyconnectednet::fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double)){

  this->nodes.resize(nodes_per_layer.size());
  this->synapses.resize(nodes_per_layer.size() - 1);
  
  for (int i = 0; i < this->nodes.size(); ++i){
    this->nodes[i].resize(nodes_per_layer[i]);
    // Add the nodes for each layer
    for (int n = 0; n < nodes_per_layer[i]; ++n){
      if (i == 0){
	// input node
	this->nodes[i][n] = new node();
      }else{
	// active node
	this->nodes[i][n] = new node(activation_function, activation_derivative);
	// Add the synapses connecting to this node
	for (int a = 0; a < nodes_per_layer[i-1]; ++a){
	  this->synapses[i-1].push_back(new synapse(this->nodes[i-1][a], this->nodes[i][n]));
	  // Use extra_params[0] as the weight delta (change in weight between backpropagation steps)
	  this->synapses[i-1][this->synapses[i-1].size() - 1]->extra_params.push_back(0);
	}
      }

    }

  }

  for (int i = 0; i < this->nodes[this->nodes.size() - 1].size(); ++i){
    // add true value for output nodes
    this->nodes[this->nodes.size() - 1][i]->extra_params.push_back(0);
  }
  
}

fullyconnectednet::~fullyconnectednet(){

  // delete nodes
  for (int i = 0; i < this->nodes.size(); ++i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      delete this->nodes[i][n];
    }
  }

  // delete synapses
  for (int i = 0; i < this->synapses.size(); ++i){
    for (int a = 0; a < this->synapses[i].size(); ++a){
      delete this->synapses[i][a];
    }
  }
  
}


void fullyconnectednet::ForwardPropogate(std::vector<double> input_values){

  // Apply the inputs
  this->Input(input_values);

  // Loop through each layer
  for (int i = 0; i < this->synapses.size(); ++i){
    // Transmit the signal through the synapses in this layer
    for (int a = 0; a < this->synapses[i].size(); ++a){
      this->synapses[i][a]->Transmit(false);      
    }
    // Apply each nodes activation function for this layer
    for (int n = 0; n < this->nodes[i+1].size(); ++n){
      this->nodes[i+1][n]->Activate(false);
    }    
  }
  
}

void fullyconnectednet::BackPropogate(std::vector<double> true_values, double learning_rate){

  // Check that the true values are of the proper size
  if (true_values.size() != this->nodes[this->nodes.size()-1].size()) {throw 1;}

  for (int i = 1; i < this->nodes.size(); ++i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      // Calculate the derivative for the signal of each node
      this->nodes[i][n]->Rate();
      // Give each output node its true value for storage
      if (i == this->nodes.size() - 1){this->nodes[i][n]->extra_params[0] = true_values[n];}
    }
  }

  // Go through each layer of synapses in reverse order
  for (int i = this->synapses.size() - 1; i >= 0; --i){
    for (int a = 0; a < this->synapses[i].size(); ++a){
      // Check for output synapses, against all the others
      if (i == this->synapses.size() - 1){
	// Start the backpropagation with the output synapses. This calculates the derivative in the loss due to this synapse
	this->synapses[i][a]->extra_params[0] = this->LossDerivative(this->synapses[i][a]->sink_node->extra_params[0], *this->synapses[i][a]->sink_node->output_signal);
      }else{
	// Reset the delta
        this->synapses[i][a]->extra_params[0] = 0;
	for (int s = 0; s < this->synapses[i+1].size(); ++s){
	  // backpropagate error from the layer above, whose error should already be known
	  this->synapses[i][a]->extra_params[0] += this->synapses[i+1][s]->extra_params[0]*synapses[i+1][s]->weight;
	}
      }
      // scale the weight delta based on signal parameters and learning_rate
      this->synapses[i][a]->extra_params[0] *= learning_rate*(*this->synapses[i][a]->source_node->output_signal)*(*this->synapses[i][a]->sink_node->activation_rate);      
    }
  }

  // Apply the change in weight to each synapse
  for (int i = 0; i < this->synapses.size(); ++i){
    for (int a = 0; a < this->synapses[i].size(); ++a){
      this->synapses[i][a]->weight -= this->synapses[i][a]->extra_params[0];
    }
  }
}

