#ifndef __CONVERT__
#define __CONVERT__

#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>

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

std::string GetName(std::string line);
std::string GetValue(std::string line);
void Get(std::string line, int* value);
void Get(std::string line, bool* value);
void Get(std::string line, std::string* value);
void Get(std::string line, std::vector<double>* value);
void Get(std::string line, std::vector<double*>* value);
void Get(std::string line, double* value);
void ParseVector(std::string csv, std::vector<double>* value);
void ParseNodeName(std::string line, bool* active, int* layer, int* node);


#endif
