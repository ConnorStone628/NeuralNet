#ifndef __FULLYCONNECTEDNET__
#define __FULLYCONNECTEDNET__

#include "net.hh"

class fullyconnectednet : public net {
  
public:

  // Nodes with constant output
  //  std::vector<node*> bias_nodes;

  // Constructor
  fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double));

  // Destructor
  ~fullyconnectednet();

  // Propogation for this type of net
  void ForwardPropogate(std::vector<double> input_values);

  // Gradient descent backpropogation to learn weights 
  void BackPropogate(std::vector<double> true_values, double learning_rate);

  void Input(std::vector<double> input_values);

  void ClearInputs();
};

#endif


