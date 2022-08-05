# include "JObject.h"
# include <iostream>
# include "Paser.h"

using namespace std;
using namespace Json;

int main() {
	//test sample:
	string src = "{\"employees\": [{ \"firstName\":\"Bill\", \"lastName\" : \"Gates\" },{ \"firstName\":\"George\" , \"lastName\" : \"Bush\" },{ \"firstName\":\"Thomas\" , \"lastName\" : \"Carter\" }]}";
	//deserialization
	Parser ps(src);
	JObject js = ps.parse();
	//serialization
	std::cout << js.to_string();
	return 0;
}