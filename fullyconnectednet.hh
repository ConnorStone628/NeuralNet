#ifndef __FULLYCONNECTEDNET__
#define __FULLYCONNECTEDNET__

#include "net.hh"

class fullyconnectednet : public net {
  
private:

  
public:

  fullyconnectednet(std::vector<unsigned int> nodes_per_layer, double (*activation_function)(double), double (*activation_derivative)(double));

  ~fullyconnectednet();

  void ForwardPropogate(std::vector<double> input_values);

  void BackPropogate(std::vector<double> true_values, double learning_rate);
  
};

#endif


