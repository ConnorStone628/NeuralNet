#ifndef __NET__
#define __NET__

#include "node.hh"
#include "synapse.hh"
#include "savemethods.hh"
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

class net{
private:

  static double StandardLoss(double true_value, double predicted_value);
  
  static double StandardLossDerivative(double true_value, double predicted_value);
  
public:

  // Function to calculate loss
  double (*loss)(double, double);

  // Function to calculate the derivative of the loss
  double (*loss_derivative)(double, double);

  // Vector of layers of nodes
  std::vector< std::vector<node*> > nodes;

  // Vector of steps of synapses
  std::vector< std::vector<synapse*> > synapses;

  // Basic constructor
  net();

  // Constructor with custom loss function
  net(double (*custom_loss)(double, double), double (*custom_loss_derivative)(double, double));

  // Destructor
  ~net();

  // User created net with pre-defined nodes and synapses
  void BuildNet(std::vector< std::vector<node*> > layer_nodes);

  // Instert a node into the net
  void AddNode(unsigned int layer);
  void AddNode(unsigned int layer, double (*act_func)(double), double (*act_deriv)(double));
  void AddNodes(unsigned int layer, unsigned int number);
  void AddNodes(unsigned int layer, unsigned int number, double (*act_func)(double), double (*act_deriv)(double));

  // Insert a new synapse into the net
  void AddSynapse(unsigned int step, node* source_node, node* sink_node);

  // Insert a vector of synapses
  void AddSynapses(unsigned int step, node* source_node, std::vector<node*> sink_nodes);

  // Get a vector of nodes at this layer
  std::vector<node*>* GetNodes(unsigned int layer);

  // Set the input to the net
  void Input(std::vector<double> input_values);

  // Collect the output from the net
  std::vector<double> Output();

  // Collect all the weights in the net
  std::vector< std::vector<double> > Weights();

  // Collect all the activity values (output) of each node
  std::vector< std::vector<double> > Activity();

  // Propogate an input signal through the net
  void Propogate();

  // Write 0 to the input of all nodes
  void ClearInputs();

  // Calculate the total loss accross the output nodes
  double TotalLoss(std::vector<double> true_values);

  // The net can save its current state
  virtual std::string Save();
  
};

#endif
