#include "loggable_component.h"

LoggableComponent::LoggableComponent(std::string component_name)
    : component_name_(std::move(component_name)) {}

void LoggableComponent::PrintLogMessage(std::string message) const {
  std::cout << "[" << component_name_ << "] " << message << std::endl;
}
