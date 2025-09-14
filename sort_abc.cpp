#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class Mode {
  Chars,
  Words
};

enum class CaseMode {
  Keep,
  Lower,
  Upper
};

struct Options {
  std::string inputPath;
  Mode mode = Mode::Chars;
  CaseMode caseMode = CaseMode::Keep;
  bool uniqueOnly = false;
};

static void printUsage(const char* programName) {
  std::cerr
    << "Usage: " << programName << " <input_file> [--mode=chars|words] [--unique] [--case=keep|lower|upper]\n"
    << "\n"
    << "- input_file: Path to the file to read.\n"
    << "- --mode: Extract single alphabetic letters (chars) or words (words). Default: chars.\n"
    << "- --unique: Output unique results only. Default: off.\n"
    << "- --case: How to normalize case before sorting: keep, lower, upper. Default: keep.\n";
}

static bool startsWith(const std::string& s, const std::string& prefix) {
  return s.rfind(prefix, 0) == 0;
}

static bool parseArgs(int argc, char** argv, Options& out) {
  if (argc < 2) {
    return false;
  }

  out.inputPath = argv[1];

  for (int i = 2; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      return false;
    }
    if (startsWith(arg, "--mode=")) {
      std::string value = arg.substr(std::string("--mode=").size());
      if (value == "chars") out.mode = Mode::Chars;
      else if (value == "words") out.mode = Mode::Words;
      else {
        std::cerr << "Unknown mode: " << value << "\n";
        return false;
      }
    } else if (arg == "--unique") {
      out.uniqueOnly = true;
    } else if (startsWith(arg, "--case=")) {
      std::string value = arg.substr(std::string("--case=").size());
      if (value == "keep") out.caseMode = CaseMode::Keep;
      else if (value == "lower") out.caseMode = CaseMode::Lower;
      else if (value == "upper") out.caseMode = CaseMode::Upper;
      else {
        std::cerr << "Unknown case mode: " << value << "\n";
        return false;
      }
    } else {
      std::cerr << "Unknown argument: " << arg << "\n";
      return false;
    }
  }
  return true;
}

static std::string readFileToString(const std::string& path) {
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (!in) {
    throw std::runtime_error("Failed to open file: " + path);
  }
  std::ostringstream buffer;
  buffer << in.rdbuf();
  return buffer.str();
}

static char applyCase(char ch, CaseMode mode) {
  switch (mode) {
    case CaseMode::Lower: return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    case CaseMode::Upper: return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    case CaseMode::Keep: default: return ch;
  }
}

static std::string applyCaseToString(const std::string& s, CaseMode mode) {
  if (mode == CaseMode::Keep) return s;
  std::string out;
  out.reserve(s.size());
  for (unsigned char ch : s) {
    out.push_back(applyCase(static_cast<char>(ch), mode));
  }
  return out;
}

static std::vector<std::string> extractChars(const std::string& content, CaseMode caseMode) {
  std::vector<std::string> result;
  result.reserve(content.size());
  for (unsigned char ch : content) {
    if (std::isalpha(ch)) {
      char cased = applyCase(static_cast<char>(ch), caseMode);
      result.emplace_back(1, cased);
    }
  }
  return result;
}

static std::vector<std::string> extractWords(const std::string& content, CaseMode caseMode) {
  std::vector<std::string> result;
  std::string current;
  current.reserve(32);
  auto flush = [&]() {
    if (!current.empty()) {
      result.push_back(applyCaseToString(current, caseMode));
      current.clear();
    }
  };
  for (unsigned char ch : content) {
    if (std::isalpha(ch)) {
      current.push_back(static_cast<char>(ch));
    } else {
      flush();
    }
  }
  flush();
  return result;
}

int main(int argc, char** argv) {
  Options options;
  if (!parseArgs(argc, argv, options)) {
    printUsage(argv[0]);
    return argc < 2 ? 1 : 0;
  }

  try {
    std::string content = readFileToString(options.inputPath);
    std::vector<std::string> items;
    if (options.mode == Mode::Chars) {
      items = extractChars(content, options.caseMode);
    } else {
      items = extractWords(content, options.caseMode);
    }

    std::sort(items.begin(), items.end());

    if (options.uniqueOnly) {
      items.erase(std::unique(items.begin(), items.end()), items.end());
    }

    for (const std::string& s : items) {
      std::cout << s << '\n';
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << "\n";
    return 2;
  }
  return 0;
}

