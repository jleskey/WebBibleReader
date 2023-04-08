#include <unordered_map>
#include <sstream>
#include <string>

using namespace std;

typedef unordered_map<string, string> QueryMap;

QueryMap extractParams(string queryString)
{
  QueryMap out;

  istringstream queryStream(queryString);

  string paramString;
  while (getline(queryStream, paramString, '&')) {
    size_t seperatorPosition = paramString.find('=');
    string key = paramString.substr(0, seperatorPosition);
    string value = paramString.substr(seperatorPosition + 1);
    out[key] = value;
  }

  return out;
}

int toInteger(string str, int defaultValue)
{
  istringstream stream(str);
  int n;
  return (stream >> n) && stream.eof() ? n : defaultValue; 
}
