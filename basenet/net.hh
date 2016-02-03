#ifndef __NET__
#define __NET__

#include "node.hh"
#include "synapse.hh"
#include "savemethods.hh"

#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>

class net{
private:

  // This is the loss function the net will use if you don't specify
  static double StandardLoss(double true_value, double predicted_value);

  // This is the derivative for the standard loss function
  static double StandardLossDerivative(double true_value, double predicted_value);
  
protected:

  // Function to calculate loss
  double (*loss)(double, double);

  // Function to calculate the derivative of the loss
  double (*loss_derivative)(double, double);

  // Vector of layers of nodes
  std::vector< std::vector<node*> > nodes;

  // Vector of steps of synapses
  std::vector< std::vector<synapse*> > synapses;

public:

  // Basic constructor
  net();

  // Constructor with custom loss function
  net(double (*custom_loss)(double, double), double (*custom_loss_derivative)(double, double));

  // Destructor
  ~net();

  // Instert a passive node into the net
  virtual void AddNode(const unsigned int layer);

  // Instert an active node into the net
  virtual void AddNode(const unsigned int layer, double (*act_func)(double), double (*act_deriv)(double));

  // Insert a bunch of passive nodes into a layer
  virtual void AddNodes(const unsigned int layer, const unsigned int number);

  // Insert a buch of active nodes into a layer
  virtual void AddNodes(const unsigned int layer, const unsigned int number, double (*act_func)(double), double (*act_deriv)(double));

  // Insert a new synapse into the net
  virtual void AddSynapse(const unsigned int step, node* source_node, node* sink_node);

  // Insert synapses from a series of nodes to a specific node
  virtual void AddSynapses(const unsigned int step, std::vector<node*> sources, std::vector<node*> sinks);

  // Set the input to the net
  virtual void Input(const std::vector<double> input_values);

  // Propogate an input signal through the net
  virtual void Propogate();

  // Collect the output from the net
  virtual std::vector<double> Output();

  // Write 0 to the input of all nodes
  virtual void ClearInputs();

  // Calculate the total loss accross the output nodes
  virtual double TotalLoss(const std::vector<double> true_values);

  // The net can save its current state
  virtual std::string Save();

  // Load the net from a string generated by Save();
  virtual int Load(std::string file_name, double (*act_func)(double), double (*act_deriv)(double));
  
};

#endif