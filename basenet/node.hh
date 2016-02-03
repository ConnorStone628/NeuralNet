#ifndef __NODE__
#define __NODE__

#include <vector>
#include "synapse.hh"
#include "savemethods.hh"
#include <string>
#include <stdlib.h>

// Forward declaration to allow the vector of pointers to exist
class node;

////////////////////////////////////////////////////////////////////////////////
class node{
private:
  //----------------------------------------------------------------------------

  // Function that determines how the node responds to the input signal
  double (*activation_function)(double);

  // Function that determines the derivative of the nodes response
  double (*activation_derivative)(double);
  
public:
  //----------------------------------------------------------------------------

  // Represents if this node is an input node or an active node
  bool passive;

  // Represents the signal comming into the node, this is what is passed to the activation function
  double* input_signal;

  // Output of the activation function
  double* output_signal;

  // Derivative of the activation function for the current input_signal
  double* activation_rate;

  // Nodes which send signal to this node
  std::vector<synapse*> source_synapses;

  // Nodes which recieve signal from this node
  std::vector<synapse*> sink_synapses;

  // Used to hold extra constants used by specialized nets
  std::vector<double*> extra_params;

  // gives the node a name for easier reference when saving
  std::string name;

  // Initialize node as a passive/input node
  node(std::string node_name);
  
  // Initialization for a node that can be activated
  node(std::string node_name, double (*act_func)(double), double (*act_deriv)(double));

  // Destructor
  ~node();

  // Applies the activation function to the input and writes the output
  void Activate();

  // applies the output signal of this node to all of its output nodes
  void Fire();

  // Applies the derivative of the activation function
  void Rate();

  // Write internal parameters to a string
  std::string Save();

  // Read internal parameters back from the string
  void Load(std::string data);

};

#endif
