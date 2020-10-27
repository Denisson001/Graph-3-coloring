#pragma once

#include "loggable_component.h"

#include <fstream>
#include <vector>

class CSVWriter : private LoggableComponent {
public:
  using Value = std::string;

  CSVWriter(const std::string& file_name);
  ~CSVWriter();

  void AddValue(const Value& value);
  void FlushValues();

private:
  std::string CreateCSVStringFrom(const std::vector<std::string>& values) const;

private:
  std::ofstream csv_file_;
  std::vector<Value> unflushed_values_;
};