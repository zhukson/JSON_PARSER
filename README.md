# JSON_PARSER

This is a C++ based Json Paser which supports serialization and deserialization of Json object.

# How to use:
### deserialzation: create a new Paser object and put the serialized version of Json into the constructor, then call parse() to get the deserialized Json object.
   
   string Js_string = "{\"employees\": [{ \"firstName\":\"Bill\", \"lastName\" : \"Gates\" },{ \"firstName\":\"George\" , \"lastName\" : \"Bush\" },{ \"firstName\":\"Thomas\" , \"lastName\" : \"Carter\" }]}";
   
   Parser paser(Js_string);
   
   JObject json = paser.parse();
   
### serialization: call to_string() to get the serialzed version from the deserialized Json object.

   string js = json.to_string();

  
