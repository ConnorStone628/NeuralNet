#ifndef __FULLYCONNECTEDNET__
#define __FULLYCONNECTEDNET__

#include "net.hh"

class fullyconnectednet : public net {
  
public:

  // Constructor
  fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double));

  // Destructor
  ~fullyconnectednet();

  // Gradient descent backpropogation to learn weights 
  void BackPropogate(std::vector<double> true_values, double learning_rate);

  // Override net Input function so that input is not written to the bias node at that level
  void Input(std::vector<double> input_values);

  // Override net ClearInputs so that bias nodes are not cleared
  void ClearInputs();
};

#endif


