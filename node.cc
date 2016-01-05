
#include "node.hh"

node::node(){
  input_signal = new double;
  output_signal = input_signal;
  *input_signal = 0;
  true_value = 0;
  passive = true;
}

node::node(double (*act_func)(double), double (*act_deriv)(double)){
  input_signal = new double;
  *input_signal = 0;

  output_signal = new double;
  *output_signal = 0;

  true_value = 0;
  
  activation_function = act_func;
  activation_derivative = act_deriv;
  
  passive = false;
}

node::~node(){
  if (passive){
    delete input_signal;
  }else{
    delete input_signal;
    delete output_signal;
  }
}

void node::Activate(bool reset){
  if (passive) {return;}
  *output_signal = (*activation_function)(*input_signal);
  if (reset) {*input_signal = 0;}
}

void node::Rate(){
  if (passive) {return;}
  *activation_rate = (*activation_derivative)(*input_signal)
}
