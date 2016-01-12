#ifndef __CONVERT__
#define __CONVERT__

#include <string>
#include <sstream>
#include <vector>

class node;
class synapse;

std::string Convert(std::string name, synapse* synapsename);
std::string Convert(std::string name, node* nodename);
std::string Convert(synapse* synapsename);
std::string Convert(node* nodename);
std::string Convert(std::string name, int value);
std::string Convert(std::string name, std::string value);
std::string Convert(std::string name, bool value);
std::string Convert(std::string name, std::vector<double> values);
std::string Convert(std::string name, std::vector<double*> values);
std::string Convert(std::string name, double value);

#endif
