#ifndef __FULLYCONNECTEDNET__
#define __FULLYCONNECTEDNET__

#include "../basenet/net.hh"

class fullyconnectednet : public net {

protected:

  // Scaling parameter for weight updates
  double learning_rate;
  
  // Calculate activation rates on all nodes
  void Rates();

  // Calculate error along the top row
  void TopErrors(std::vector<double> true_values);

  // Back-Propogate errors through the net
  void BackPropogateErrors();
  
public:

  // Constructor
  fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double));

  // Destructor
  ~fullyconnectednet();

  // Set the constant factor that weight updates are scaled by
  void SetLearningRate(double rate);

  // Gradient descent backpropogation to learn weights 
  void BackPropogate(std::vector<double> true_values);

  // Override net Input function so that input is not written to the bias node at that level
  void Input(std::vector<double> input_values);

  // Override net ClearInputs so that bias nodes are not cleared
  void ClearInputs();
  
};

#endif


