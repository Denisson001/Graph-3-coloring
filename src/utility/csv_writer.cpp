#include "csv_writer.h"

CSVWriter::CSVWriter(const std::string &file_name)
    : LoggableComponent("CSVWriter"),
      csv_file_(file_name + ".csv")
{
  PrintLogMessage("Start writing to " + file_name + ".csv");
}

CSVWriter::~CSVWriter() {
  csv_file_.close();
}

void CSVWriter::AddValue(const Value& value) {
  unflushed_values_.emplace_back(value);
}

void CSVWriter::FlushValues() {
  csv_file_ << CreateCSVStringFrom(unflushed_values_) << '\n';
  unflushed_values_.clear();
}

std::string CSVWriter::CreateCSVStringFrom(const std::vector<std::string>& values) const {
  std::string csv_string;
  for (const auto& value : values) {
    csv_string += value + ",";
  }
  if (!csv_string.empty()) {
    csv_string.pop_back();
  }
  return csv_string;
}
