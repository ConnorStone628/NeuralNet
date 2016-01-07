#ifndef __NET__
#define __NET__

#include "node.hh"
#include "synapse.hh"
#include <vector>

class net{
private:

  std::vector< std::vector<node*> > nodes;

  std::vector< std::vector<synapse*> > synapses;

  double (*loss)(double, double);

  double (*loss_derivative)(double, double);

  static double StandardLoss(double true_value, double predicted_value);
  
  static double StandardLossDerivative(double true_value, double predicted_value);
  
public:

  net();

  net(double (*custom_loss)(double, double), double (*custom_loss_derivative)(double, double));

  ~net();

  void BuildNet(std::vector< std::vector<node*> > layer_nodes, std::vector< std::vector<synapse*> > synapse_sequence);

  void AddNode(unsigned int layer, node* new_node);

  void AddSynapse(unsigned int step, synapse* new_synapse);

  std::vector<node*>* GetNodes(unsigned int layer);

  std::vector<synapse*>* GetSynapses(unsigned int step);

  void Input(std::vector<double> input_values);

  std::vector<double> Output();

  std::vector< std::vector<double> > Weights();

  void Propogate(bool reset);

  void ClearInputs();
  
};

#endif
