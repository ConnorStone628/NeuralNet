
#include "node.hh"

node::node(){

  this->input_signal = new double;
  
  // Input node has no activation, thus the output is the same as the input
  this->output_signal = input_signal;
  *this->input_signal = 0;

  // Record that this is a passive node
  this->passive = true;
  
}

node::node(double (*act_func)(double), double (*act_deriv)(double)){

  // Holds the total signal comming in from input synapses
  this->input_signal = new double;
  *this->input_signal = 0;

  // Holds the result of activation on the input_signal
  this->output_signal = new double;
  *this->output_signal = 0;

  // Derivative of output_signal at the current input_signal
  this->activation_rate = new double;
  *this->activation_rate = 0;

  // Set the functions
  this->activation_function = act_func;
  this->activation_derivative = act_deriv;

  // Record that this is an active node
  this->passive = false;
  
}

node::~node(){

  // Eliminate all newly created variables
  if (this->passive){
    delete this->input_signal;
  }else{
    delete this->input_signal;
    delete this->output_signal;
    delete this->activation_rate;
  }

  for (int i = 0; i < this->extra_params.size(); ++i){
    delete this->extra_params[i];
  }
  
}

void node::Activate(){
  
  if (this->passive) {return;}
  // Apply the activation function to the input_signal
  *this->output_signal = (*this->activation_function)(*this->input_signal);
  
}

void node::Fire(){
  
  // Send the output signal through all output synapses
  for (int i = 0; i < this->sink_synapses.size(); ++i){
    *this->sink_synapses[i]->sink_node->input_signal += (*this->sink_synapses[i]->weight)*(*this->output_signal);
  }
  
}

void node::Rate(){
  
  if (this->passive) {return;}
  // calculate the derivative of the activation_function at the current input
  *this->activation_rate = (*this->activation_derivative)(*this->input_signal);
    
}
