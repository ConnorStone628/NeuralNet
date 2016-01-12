
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

