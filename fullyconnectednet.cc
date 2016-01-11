
#include "fullyconnectednet.hh"
#include <iostream>

fullyconnectednet::fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double)){

  
  // Initialize vectors to hold nodes and synapses
  this->nodes.resize(nodes_per_layer.size());
  this->bias_nodes.resize(nodes_per_layer.size() - 1);

  // Add the nodes to the net
  for (int i = 0; i < nodes_per_layer.size(); ++i){
    // Resize the layer to hold enough nodes
    this->nodes[i].resize(nodes_per_layer[i]);
    // Add the bias nodes for each layer
    if (i < nodes_per_layer.size() - 1){
      this->bias_nodes[i] = new node();
      *this->bias_nodes[i]->input_signal = 1;
    }
    // Add the regular nodes for each layer
    for (int n = 0; n < nodes_per_layer[i]; ++n){
      // Add input/active node
      if (i == 0){
	this->nodes[i][n] = new node();
      }else{
	this->nodes[i][n] = new node(activation_function, activation_derivative);
      }
    }
  }

  // Synapse the net
  for (int i = 0; i < this->nodes.size()-1; ++i){
    for (int n = 0; n < this->nodes[i].size(); ++ n){
      Synapse(i, this->nodes[i][n], this->nodes[i+1]);
    }
    Synapse(i, this->bias_nodes[i], this->nodes[i+1]);
  }

  //********************************************************************************
  for (int n = 0; n < this->nodes[this->nodes.size()-1].size(); ++n){
    this->nodes[this->nodes.size()-1][n]->extra_params.push_back(new double);
  }
  
}

fullyconnectednet::~fullyconnectednet(){

  // delete nodes
  for (int i = 0; i < this->nodes.size(); ++i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      delete this->nodes[i][n];
    }
    if (i < this->nodes.size() - 1){
      delete this->bias_nodes[i];
    }
  }
  
}


void fullyconnectednet::ForwardPropogate(std::vector<double> input_values){

  // Apply the inputs
  this->Input(input_values);

  // propogate the input signal through the net
  for (int i = 0; i < this->nodes.size(); ++i){    
    for (int n = 0; n < this->nodes[i].size(); ++n){
      this->nodes[i][n]->Activate();
      this->nodes[i][n]->Fire();
    }
    if (i < this->nodes.size() - 1){
      this->bias_nodes[i]->Fire();
    }
  }
  
}

void fullyconnectednet::BackPropogate(std::vector<double> true_values, double learning_rate){

  double delta;
  
  // Check that the true values are of the proper size
  if (true_values.size() != this->nodes[this->nodes.size()-1].size()) {throw 1;}

  // // precalculate values on the nodes that will be useful, and add true values
  // for (int i = 1; i < this->nodes.size(); ++i){
  //   for (int n = 0; n < this->nodes[i].size(); ++n){
  //     // Calculate the derivative for the signal of each node
  //     this->nodes[i][n]->Rate();
  //     // Give each output node its true value for storage
  //     if (i == this->nodes.size() - 1){
  // 	delta = this->loss_derivative(true_values[n], *this->nodes[i][n]->output_signal)*(*this->nodes[i][n]->activation_rate);
  // 	for (int s = 0; s < this->nodes[i][n]->source_synapses.size(); ++s){
  // 	  *this->nodes[i][n]->source_synapses[s]->weight_delta = learning_rate*delta*(*this->nodes[i][n]->source_synapses[s]->source_node->output_signal);
  // 	}
  //     }
  //   }
  // }  

  // for (int i = this->nodes.size() - 2; i > 0; --i){
  //   for (int n = 0; n < this->nodes[i].size(); ++n){
  //     for (int s = 0; s < this->nodes[i][n]->source_synapses.size(); ++s){
  // 	*this->nodes[i][n]->source_synapses[s]->weight_delta = 0;
  // 	for (int a = 0; a < this->nodes[i][n]->sink_synapses.size(); ++a){
  // 	  *this->nodes[i][n]->source_synapses[s]->weight_delta += (*this->nodes[i][n]->sink_synapses[a]->weight_delta)*(*this->nodes[i][n]->sink_synapses[a]->weight);
  // 	}
  // 	*this->nodes[i][n]->source_synapses[s]->weight_delta *= learning_rate*(*this->nodes[i][n]->activation_rate)*(*this->nodes[i][n]->source_synapses[s]->source_node->output_signal);
  //     }
  //   }
  // }

  // for (int i = 0; i < this->synapses.size() - 1; ++i){
  //   for (int n = 0; n < this->synapses[i].size(); ++n){
  //     *this->synapses[i][n]->weight -= *this->synapses[i][n]->weight_delta;
  //   }
  // }

  for (int i = 1; i < this->nodes.size(); ++i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      // Calculate the derivative for the signal of each node
      this->nodes[i][n]->Rate();
      // Give each output node its true value for storage
      if (i == this->nodes.size() - 1){
	*this->nodes[i][n]->extra_params[0] = true_values[n];
      }
    }
  }  

  // Go through each layer of synapses in reverse order
  for (int i = this->synapses.size() - 1; i >= 0; --i){
    for (int a = 0; a < this->synapses[i].size(); ++a){
      // Check for output synapses
      if (i == this->synapses.size() - 1){
  	// Start the backpropagation with the output synapses. This calculates the derivative in the loss due to this synapse
  	*this->synapses[i][a]->weight_delta = this->loss_derivative(*this->synapses[i][a]->sink_node->extra_params[0], *this->synapses[i][a]->sink_node->output_signal);
      }else{
  	// Reset the delta
        *this->synapses[i][a]->weight_delta = 0;
  	// backpropagate error from the layer above, whose error should already be known
  	for (int s = 0; s < this->synapses[i][a]->sink_node->sink_synapses.size(); ++s){
  	  *this->synapses[i][a]->weight_delta += (*this->synapses[i][a]->sink_node->sink_synapses[s]->weight_delta)*(*this->synapses[i][a]->sink_node->sink_synapses[s]->weight);
  	}
      }
      // scale the weight delta based on signal parameters and learning_rate
      *this->synapses[i][a]->weight_delta *= learning_rate*(*this->synapses[i][a]->sink_node->activation_rate)*(*this->synapses[i][a]->source_node->output_signal);      
    }
  }

  // Apply the change in weight to each synapse
  for (int i = 0; i < this->synapses.size(); ++i){
    for (int a = 0; a < this->synapses[i].size(); ++a){
      *this->synapses[i][a]->weight -= *this->synapses[i][a]->weight_delta;
    }
  }
}

