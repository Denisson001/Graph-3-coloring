#pragma once

#include <iostream>
#include <string>

class LoggableComponent {
public:
  LoggableComponent(std::string component_name);

  void PrintLogMessage(std::string message) const;

private:
  std::string component_name_;
};
