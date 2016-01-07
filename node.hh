#ifndef __NODE__
#define __NODE__

#include <vector>
#include "synapse.hh"

////////////////////////////////////////////////////////////////////////////////
class node{
private:
  //----------------------------------------------------------------------------

  // Function that determines how the node responds to the input signal
  double (*activation_function)(double);

  // Function that determines the derivative of the nodes response
  double (*activation_derivative)(double);
  
  // Represents if this node is an input node or an active node
  bool passive;

public:
  //----------------------------------------------------------------------------

  // Represents the signal comming into the node, this is what is passed to the activation function
  double* input_signal;

  // Output of the activation function
  double* output_signal;

  // Derivative of the activation function for the current input_signal
  double* activation_rate;

  // All synapses that send signal to this node
  std::vector<synapse*> input_synapses;

  // All synapse that send signals from this node
  std::vector<synapse*> output_synapses;
  
  // Used to hold extra constants used by specialized nets
  std::vector<double> extra_params;

  // Initialize node as a passive/input node
  node();
  
  // Initialization for a node that can be activated
  node(double (*act_func)(double), double (*act_deriv)(double));

  // Destructor
  ~node();

  // Applies the activation function to the input and writes the output
  void Activate(bool reset);

  // Applies the derivative of the activation function
  void Rate();

};

#endif
