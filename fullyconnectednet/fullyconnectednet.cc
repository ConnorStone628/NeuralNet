
#include "fullyconnectednet.hh"

//--------------------------------------------------------------------------------
fullyconnectednet::fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double)){

  this->learning_rate = 1;
  
  // Add the nodes to the net
  for (int i = 0; i < nodes_per_layer.size(); ++i){
    // Add the bias nodes for each layer
    if (i < nodes_per_layer.size() - 1){
      this->AddNode(i);
      *this->nodes[i][0]->input_signal = 1;
    }
    // Add input/active node
    if (i == 0){
      this->AddNodes(0, nodes_per_layer[0]);
    }else{
      this->AddNodes(i, nodes_per_layer[i], activation_function, activation_derivative);
    }
  }

  // Synapse the net
  for (int i = 0; i < this->nodes.size()-1; ++i){
    for (int n = 0; n < this->nodes[i].size(); ++ n){
      for (int s = 0; s < this->nodes[i+1].size(); ++s){
	// Do not synapse to any passive nodes
	if (this->nodes[i+1][s]->passive){continue;}
	// Add the synapse between the two nodes
	this->AddSynapse(i, this->nodes[i][n], this->nodes[i+1][s]);
      }
    }
  }

}

//--------------------------------------------------------------------------------
fullyconnectednet::~fullyconnectednet(){}

//--------------------------------------------------------------------------------
void fullyconnectednet::SetLearningRate(double rate){
  this->learning_rate = rate;
}
  
//--------------------------------------------------------------------------------
void fullyconnectednet::Input(std::vector<double> input_values){

  // Checks that you have given the righ tnumber of inputs
  if (input_values.size()+1 != this->nodes[0].size()){throw 1;}

  // Write the input value to each input node
  for (int i = 1; i < this->nodes[0].size(); ++i){
    *(this->nodes[0][i]->input_signal) = input_values[i-1];
  }

}

//--------------------------------------------------------------------------------
void fullyconnectednet::ClearInputs(){

  // Write 0 to the input of each node
  for (int i = 0; i < this->nodes.size(); ++i){    
    for (int n = 0; n < this->nodes[i].size(); ++n){
      // Skip bias nodes, which are the first at each level, except for the output
      if (i < this->nodes.size() - 1 && n == 0){continue;}
      *(this->nodes[i][n]->input_signal) = 0;
    }
  }

}

//--------------------------------------------------------------------------------
void fullyconnectednet::BackPropogate(std::vector<double> true_values){

  this->Rates();
  this->TopErrors(true_values);
  this->BackPropogateErrors();

  return;
  // // Holds the change in weight during calculation
  // double delta;
  
  // // Check that the true values are of the proper size
  // if (true_values.size() != this->nodes[this->nodes.size()-1].size()) {throw 1;}

  // // precalculate values on the nodes that will be useful, and add true values
  // for (int i = 1; i < this->nodes.size(); ++i){
  //   for (int n = 0; n < this->nodes[i].size(); ++n){
  //     // Calculate the derivative for the signal of each node
  //     this->nodes[i][n]->Rate();
  //     // Calculate the error at the output nodes
  //     if (i == this->nodes.size() - 1){
  // 	// (Learning rate)(derivative of loss function)(derivative of activation function)
  // 	delta = learning_rate*this->loss_derivative(true_values[n], *this->nodes[i][n]->output_signal)*(*this->nodes[i][n]->activation_rate);
  // 	for (int s = 0; s < this->nodes[i][n]->source_synapses.size(); ++s){
  // 	  // each synapse records its weight delta which is delta*(activation of the source node for this synapse)
  // 	  *this->nodes[i][n]->source_synapses[s]->weight_delta = delta*(*this->nodes[i][n]->source_synapses[s]->source_node->output_signal);
  // 	}
  //     }
  //   }
  // }  

  // // Backpropogate the error through the net
  // for (int i = this->nodes.size() - 2; i > 0; --i){
  //   for (int n = 0; n < this->nodes[i].size(); ++n){
  //     for (int s = 0; s < this->nodes[i][n]->source_synapses.size(); ++s){
  // 	delta = 0;//*this->nodes[i][n]->source_synapses[s]->weight_delta
  // 	for (int a = 0; a < this->nodes[i][n]->sink_synapses.size(); ++a){
  // 	  // Add the error from the above layer of weights. this is the weight on each synapse, multiplied by its error/delta
  // 	  delta += (*this->nodes[i][n]->sink_synapses[a]->weight_delta)*(*this->nodes[i][n]->sink_synapses[a]->weight);//*this->nodes[i][n]->source_synapses[s]->weight_delta
  // 	}
  // 	// Scale by the learning rate, the derivative of the activation function, and the activation that passed through the synapse
  // 	*this->nodes[i][n]->source_synapses[s]->weight_delta = delta*learning_rate*(*this->nodes[i][n]->activation_rate)*(*this->nodes[i][n]->source_synapses[s]->source_node->output_signal);
  //     }
  //   }
  // }
  
  // // Apply the change in weight to each synapse
  // for (int i = 0; i < this->synapses.size(); ++i){
  //   for (int a = 0; a < this->synapses[i].size(); ++a){
  //     *this->synapses[i][a]->weight -= *this->synapses[i][a]->weight_delta;
  //   }
  // }
}

//--------------------------------------------------------------------------------
void fullyconnectednet::Rates(){
  
  for (int i = 1; i < this->nodes.size(); ++i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      this->nodes[i][n]->Rate();
    }
  }
  
}

void fullyconnectednet::TopErrors(std::vector<double> true_values){

  double delta; // Holds the change in weight during calculation
  int final_layer = this->nodes.size() - 1; // last layer in the net
  
  // Calculate the error at the output nodes
  for (int n = 0; n < this->nodes[final_layer].size(); ++n){
    // (Learning rate)(derivative of loss function)(derivative of activation function)
    delta = this->learning_rate*this->loss_derivative(true_values[n], *this->nodes[final_layer][n]->output_signal)*(*this->nodes[final_layer][n]->activation_rate);
    for (int s = 0; s < this->nodes[final_layer][n]->source_synapses.size(); ++s){
      // each synapse records its weight delta which is delta*(activation of the source node for this synapse)
      *this->nodes[final_layer][n]->source_synapses[s]->weight_delta = delta*(*this->nodes[final_layer][n]->source_synapses[s]->source_node->output_signal);
    }
  }
  
}

void fullyconnectednet::BackPropogateErrors(){

  double delta; // Holds the change in weight during calculation
  
  // Backpropogate the error through the net
  for (int i = this->nodes.size() - 2; i > 0; --i){
    for (int n = 0; n < this->nodes[i].size(); ++n){
      for (int s = 0; s < this->nodes[i][n]->source_synapses.size(); ++s){
	delta = 0;
  	for (int a = 0; a < this->nodes[i][n]->sink_synapses.size(); ++a){
	  // Add the error from the above layer of weights. this is the weight on each synapse, multiplied by its error/delta
	  delta += (*this->nodes[i][n]->sink_synapses[a]->weight_delta)*(*this->nodes[i][n]->sink_synapses[a]->weight);
  	}
	// Scale by the learning rate, the derivative of the activation function, and the activation that passed through the synapse
  	*this->nodes[i][n]->source_synapses[s]->weight_delta = delta*this->learning_rate*(*this->nodes[i][n]->activation_rate)*(*this->nodes[i][n]->source_synapses[s]->source_node->output_signal);
      }
    }
  }
  
  // Apply the change in weight to each synapse
  for (int i = 0; i < this->synapses.size(); ++i){
    for (int a = 0; a < this->synapses[i].size(); ++a){
      *this->synapses[i][a]->weight -= *this->synapses[i][a]->weight_delta;
    }
  }

}
