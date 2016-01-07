
#include "node.hh"

node::node(){
  
  this->input_signal = new double;
  this->output_signal = input_signal;
  *this->input_signal = 0;
  this->passive = true;
  
}

node::node(double (*act_func)(double), double (*act_deriv)(double)){
  
  this->input_signal = new double;
  *this->input_signal = 0;

  this->output_signal = new double;
  *this->output_signal = 0;

  this->activation_rate = new double;
  *this->activation_rate = 0;
  
  this->activation_function = act_func;
  this->activation_derivative = act_deriv;
  
  this->passive = false;
  
}

node::~node(){
  
  if (this->passive){
    delete this->input_signal;
  }else{
    delete this->input_signal;
    delete this->output_signal;
    delete this->activation_rate;
  }
  
}

void node::Activate(bool reset){
  
  if (this->passive) {return;}
  *this->output_signal = (*this->activation_function)(*this->input_signal);
  if (reset) {*this->input_signal = 0;}
  
}

void node::Fire(){

  for (int i = 0; i < this->output_synapses.size(); ++i){
    this->output_synapses->Transmit(false);
  }
  
}

void node::Rate(){
  
  if (this->passive) {return;}
  *this->activation_rate = (*this->activation_derivative)(*this->input_signal)
    
}
