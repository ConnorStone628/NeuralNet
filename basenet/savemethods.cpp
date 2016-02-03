
#include "savemethods.hh"

std::string Convert(std::string name, double value){
  
  std::stringstream s;
  s.str("");
  s << name << "{" << value << "}\n";
  return s.str();
}

std::string Convert(node* nodename){
  std::stringstream s;
  s.str("");
  s << nodename;
  return s.str();
}

std::string Convert(synapse* synapsename){
  std::stringstream s;
  s.str("");
  s << synapsename;
  return s.str();
}

std::string Convert(std::string name, node* nodename){
  std::stringstream s;
  s.str("");
  s << nodename;
  return name + "{" + s.str() + "}\n";
}

std::string Convert(std::string name, synapse* synapsename){
  std::stringstream s;
  s.str("");
  s << synapsename;
  return name + "{" + s.str() + "}\n";
}


std::string Convert(std::string name, int value){
  std::stringstream s;
  s.str("");
  s << name << "{" << value << "}\n";
  return s.str();
}

std::string Convert(std::string name, std::string value){
  return name + "{" + value + "}\n";
}


std::string Convert(std::string name, bool value){
  if (value){
    return name + "{1}\n";
  }else{
    return name + "{0}\n";
  }
}

std::string Convert(std::string name, std::vector<double> values){
  std::stringstream s;
  s.str("");
  s << name + "{";
  for (int i = 0; i < values.size(); ++i){
    if (i > 0){s << ",";}
    s << values[i];
  }
  s << "}\n";
  return s.str();
}

std::string Convert(std::string name, std::vector<double*> values){
  std::stringstream s;
  s.str("");
  s << name + "{";
  for (int i = 0; i < values.size(); ++i){
    if (i > 0){s << ",";}
    s << *values[i];
  }
  s << "}\n";
  return s.str();
}

std::string GetName(std::string line){

  return line.substr(0, line.find("{"));
  
}

std::string GetValue(std::string line){

  return line.substr(line.find("{")+1,line.find("}")-line.find("{")-1);
  
}


void Get(std::string line, int* value){
  *value = atoi(GetValue(line).c_str());
}

void Get(std::string line, bool* value){
  if (GetValue(line).c_str() == "0"){
    *value = false;
  }else{
    *value = true;
  }
}

void Get(std::string line, std::string* value){
  *value = GetValue(line);
}

void ParseVector(std::string csv, std::vector<double>* value){
  int loc = csv.find(",");

  (*value).push_back(atof(csv.substr(0,loc).c_str()));

  if ((csv.substr(loc+2,std::string::npos)).find(",") != std::string::npos){
    ParseVector(csv.substr(loc+2,std::string::npos), value);
  }
  
}

void Get(std::string line, std::vector<double>* value){

  ParseVector(GetValue(line), value);
  
}

void Get(std::string line, std::vector<double*>* value){

  std::vector<double> temp_vector;

  ParseVector(GetValue(line), &temp_vector);

  for (int i = 0; i < temp_vector.size(); ++i){
    (*value).push_back(new double);
    *(*value)[(*value).size() - 1] = temp_vector[i];
  }
  
}

void Get(std::string line, double* value){
  *value = atof(GetValue(line).c_str());
}

void ParseNodeName(std::string line, bool* active, int* layer, int* node){
  if (line.substr(0,1) == "A"){
    *active = true;
  }else{
    *active = false;
  }

  *layer = atoi(line.substr(2,line.find("N")).c_str());

  *node = atoi(line.substr(line.find("N")+1, std::string::npos).c_str());
    
}
